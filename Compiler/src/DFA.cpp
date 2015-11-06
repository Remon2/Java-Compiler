#include "DFA.h"
#include <vector>
#include <map>
#include <iostream>
#include <fstream>
#include <deque>
#include <queue>
#include <stack>
#include <string>
#include <sstream>
using namespace std;
#include <algorithm>

static int counter = 0;
map<int, int> saved;
vector<int>* minimizedDFAStates;
static vector<DFA*>*allDFAStates = new vector<DFA*>;
static queue<DFA*> storedDFA;
static queue<char> storedC;
static map<int, vector<State*>*>* equalStates = new map<int, vector<State*>*>;
static vector<vector<int> > implicat;
static int allDFASize;
static vector<char> possibleInputs;

ofstream outfile("DFA.txt");
ofstream outfile2("NFA.txt");
ofstream outfileTransion("Transition.txt");

//to get the max clique in the graph
bool removable(vector<int> neighbor, vector<int> cover);
int max_removable(vector<vector<int> > neighbors, vector<int> cover);
vector<int> procedure_1(vector<vector<int> > neighbors, vector<int> cover);
vector<int> procedure_2(vector<vector<int> > neighbors, vector<int> cover,
		int k);
int cover_size(vector<int> cover);

//methods to help us to get the mininmum dfa
void bubbleSort(vector<State*>* num);
int getIndex(DFA * temp);
int getIndexByName(int temp);
void getMaxEqualState();
void makeTransionTable();

DFA::DFA() {
	this->from = NULL;
	this->name = 0;
	this->to = new map<char, DFA*>;
	this->accepted = false;
	this->token = "";
	this->priority = 10000000;
}

DFA::~DFA() {
	//dtor
}

//****************this method searches whether this char input in the saved array of inputs or not**********************
bool searchForInput(char c) {
	for (int i = 0; i < possibleInputs.size(); i++) {
		if (possibleInputs.at(i) == c)
			return true;
	}
	return false;
}

//****************this method searches whether this char input in the transion table of the given DFA or not**********************

bool searchInMap(char c, map<char, DFA*>*to) {
	for (int i = 0; i < to->size(); i++) {
		if (to->find(c) != to->end())
			return true;
	}
	return false;
}

//****************this method add the epsilon transiontions to each state**********************

void DFA::addEpsilonTransions(State * start, vector<State*>* in) {
	getEpsillonTransion(in, start);
}

//****************this method get the epsilon transiontions to each state and save it in the vector of dfatemp**********************

void DFA::getEpsillonTransion(vector<State*>* dfaTemp, State* start) {

	if (start == NULL || start->visited == true)
		return;
	start->visited = true;
	if (start->split == true || start->epsilon == true
			|| start->c == (char) 257) {
		getEpsillonTransion(dfaTemp, start->next);
		getEpsillonTransion(dfaTemp, start->next2);

	}
	dfaTemp->push_back(start);

}

//****************this method used to print NFA states in fil named "NFA.txt"**********************

void DFA::printNFA(State* start) {

	if (start == NULL || start->visited == true)
		return;
	start->visited = true;
	printNFA(start->next);
	printNFA(start->next2);
	if (start->next != NULL && start->next2 != NULL)
		outfile2 << start->id << "    ------->   " << start->next->id
				<< "      |" << start->next2->id << "      |" << start->token
				<< "            |" << start->c << "   |" << start->accepted
				<< "       |" << start->epsilon << "       |" << start->split
				<< endl;
	else if (start->next != NULL && start->next2 == NULL)
		outfile2 << start->id << "    ------->   " << start->next->id
				<< "      |" << start->next2 << "      |" << start->token
				<< "            |" << start->c << "   |" << start->accepted
				<< "       |" << start->epsilon << "       |" << start->split
				<< endl;
	else if (start->next == NULL && start->next2 == NULL)
		outfile2 << start->id << "    ------->   " << start->next << "      |"
				<< start->next2 << "      |" << start->token << "            |"
				<< start->c << "  |" << start->accepted << "       |"
				<< start->epsilon << "       |" << start->split << endl;

}

//****************this method used to make all the states unvisited after getting the epsilon transions"**********************

void DFA::setUnVisited(State* start) {
	if (start == NULL || start->visited == false)
		return;
	if (start->visited == true) {
		start->visited = false;
		setUnVisited(start->next);
		setUnVisited(start->next2);
	}
}

//****************this method used to get the char transions from a given state"**********************

State* DFA::getCTransion(char c, State* start) {
	if (start->c == c) {
		return start->next;
	}
	return NULL;
}

//****************this method used to convert the NFA to DFA **********************

void DFA::buildDFA(State * start) {
	outfile2 << "name " << "------->   " << "next " << "   " << "next2" << "   "
			<< "token" << "        " << "c" << "   " << "acceptd" << "   "
			<< "epsilon" << "   " << "split" << endl;
	printNFA(start);
	setUnVisited(start);
	counter++;
	DFA* dState = new DFA();
	dState->name = counter;
	dState->from = NULL;
	vector<State*>* dfaTemp = new vector<State*>;
	getEpsillonTransion(dfaTemp, start);
	setUnVisited(start);
	equalStates->insert(pair<int, vector<State*>*>(counter, dfaTemp));
	allDFAStates->push_back(dState);
	dState->to = new map<char, DFA*>;
	toTransions(dState);
	while (!storedDFA.empty()) {
		DFA * temp = storedDFA.front();
		storedDFA.pop();
		char cTemp = storedC.front();
		storedC.pop();
		vector<State*>* current1 = new vector<State*>;
		current1 = equalStates->at(temp->name);
		bubbleSort(current1);
		bool testEqual = true;
		for (int i = 0; i < allDFAStates->size(); i++) {
			testEqual = true;
			vector<State*>* current2 = new vector<State*>;
			current2 = equalStates->at(allDFAStates->at(i)->name);
			bubbleSort(current2);
			if (current1->size() == current2->size()) {
				for (int j = 0; j < current1->size(); j++) {
					if (current1->at(j) != current2->at(j)) {
						testEqual = false;
						break;
					}
				}
				if (testEqual) {
					temp->from->to->erase(cTemp);
					temp->from->to->insert(
							pair<char, DFA*>(cTemp, allDFAStates->at(i)));
					break;
				}
			} else {
				testEqual = false;
			}
		}
		if (!testEqual) {
			allDFAStates->push_back(temp);
			toTransions(temp);
		}
	}
	setAccepted();
	printDFA();
	for (int i = 0; i < allDFAStates->size(); i++) {
		saved.insert(pair<int, int>(i, allDFAStates->at(i)->name));
	}
	cout
			<< "========================== after minimization ==============================="
			<< endl;
	min_DFA();
	getMiniDFA();
	printDFA();
	makeTransionTable();
//	cout << "*** " << allDFAStates->size() << " ***" << endl;
//	for (int i = 0; i < allDFAStates->size(); i++) {
//		cout << "*** " << (*allDFAStates->at(i)).name << endl;
////		for (map<char, DFA*>::iterator it = (*allDFAStates->at(i)).to->begin();
////				it != (*allDFAStates->at(i)).to->end(); it++) {
//////			cout << it->first << endl;
//////			cout << it->second->name << endl;
////			cout << "--- Char= " << it->first << " --- toState= "
////					<< it->second->name << endl;
////			cout << "----------------------------" << endl;
////		}
//
//	}
}

//****************this method used to return the minimum DFA states **********************

vector<DFA*>* DFA::getAllDFA() {
	return allDFAStates;
}
void DFA::toTransions(DFA * dState) {
	vector<State*>* current = new vector<State*>;
	current = equalStates->at(dState->name);
	for (int i = 0; current != NULL && i < current->size(); i++) {
		if (current->at(i)->c != (char) 257) {
			vector<State*>* current2 = new vector<State*>;

			bool existed = searchForInput(current->at(i)->c);
			bool existedInMap = false;
			char ch = current->at(i)->c;
			for (int j = 0; j < dState->to->size(); j++) {
				if (dState->to->find(ch) != dState->to->end())
					existedInMap = true;
			}
			if (!existed)
				possibleInputs.push_back(ch);
			State* s = getCTransion(ch, current->at(i));
			if (existedInMap == false) {
				DFA* dStateTemp = new DFA();
				counter++;
				vector<State*>* dfaTemp = new vector<State*>;
				dStateTemp->name = counter;
				dStateTemp->from = dState;
				addEpsilonTransions(s, current2);
				setUnVisited(s);
				equalStates->insert(
						pair<int, vector<State*>*>(counter, current2));
				dStateTemp->to = new map<char, DFA*>;
				dState->to->insert(pair<char, DFA*>(ch, dStateTemp));
				storedDFA.push(dStateTemp);
				storedC.push(ch);

			} else {
				DFA* temp = (dState->to)->find(ch)->second;
				current2 = equalStates->at(temp->name);
				addEpsilonTransions(s, current2);
				setUnVisited(s);
			}

		}
	}
}

//****************this method used to sort vectors of states **********************

void bubbleSort(std::vector<State*> *num) {
	int i, j, flag = 1;
	State * temp;
	int numLength = num->size();
	for (i = 1; (i <= numLength) && flag; i++) {
		flag = 0;
		for (j = 0; j < (numLength - 1); j++) {
			if (num->at(j + 1)->id < num->at(j)->id) {
				temp = num->at(j);
				num->at(j) = num->at(j + 1);
				num->at(j + 1) = temp;
				flag = 1;
			} else if (num->at(j + 1)->id == num->at(j)->id) {
				num->erase(num->begin() + j);
				numLength--;
			}
		}
	}
	return;
}

//****************this method used to set the accepted and the token variable for every DFA state **********************

void DFA::setAccepted() {
	vector<State*>* current = new vector<State*>;
	for (int i = 0; i < allDFAStates->size(); i++) {
		current = equalStates->at(allDFAStates->at(i)->name);
		for (int j = 0; j < current->size(); j++) {
			if (current->at(j)->accepted == true
					&& current->at(j)->priority
							< allDFAStates->at(i)->priority) {
				allDFAStates->at(i)->accepted = true;
				allDFAStates->at(i)->priority = current->at(j)->priority;
				allDFAStates->at(i)->token = current->at(j)->token;
			}
		}
	}

}

//****************this method used to print Transion table in file named "transion.txt"**********************

void makeTransionTable() {
	int row = allDFAStates->size() + 1;
	int colm = possibleInputs.size() + 1;
	string arr[row][colm];
	for (int k = 1; k < colm; k++) {
		arr[0][k] = possibleInputs.at(k - 1) + " ";
	}
	for (int i = 1; i < row; i++) {
		stringstream ss;
		ss << allDFAStates->at(i - 1)->name;
		arr[i][0] = ss.str();
		for (int j = 1; j < colm; j++) {
			char ch = possibleInputs.at(j - 1);
			if (allDFAStates->at(i - 1)->to->find(ch)
					!= allDFAStates->at(i - 1)->to->end()) {
				stringstream sss;
				sss << allDFAStates->at(i - 1)->to->at(ch)->name;
				arr[i][j] = sss.str();
			} else {
				arr[i][j] = "-1";
			}
		}
	}

	for (int j = 0; j < colm - 1; j++) {
		outfileTransion << "    " << possibleInputs.at(j);
	}
	outfileTransion << endl;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < colm; j++) {
			outfileTransion << arr[i][j] << "    ";
		}
		outfileTransion << endl;
	}
}

//****************this method used to get the index of a state exists in the DFA states **********************

int getIndex(DFA * temp) {
	for (int i = 0; i < allDFAStates->size(); i++) {
		if (allDFAStates->at(i)->name == temp->name) {
			return i;
		}
	}
}

//****************this method used to make the implicant table of the redused DFA state **********************

void DFA::min_DFA() {
	for (int i = 0; i < allDFAStates->size(); i++) {
		vector<int> row;
		for (int j = 0; j < allDFAStates->size(); j++) {
			if (i == j || i < j) {
				row.push_back(1);
			} else if (allDFAStates->at(i)->token
					!= allDFAStates->at(j)->token) {
				row.push_back(1);
			} else {
				row.push_back(0);
			}
			if (allDFAStates->at(i)->accepted
					!= allDFAStates->at(j)->accepted) {
				row[j] = 1;
			}
		}
		implicat.push_back(row);
	}

	int noOfChanges = 0;
	do {
		noOfChanges = 0;
		for (int i = 0; i < allDFAStates->size(); i++) {
			for (int j = 0; j < allDFAStates->size(); j++) {
				if (implicat[i][j] != 1) {
					for (int z = 0; z < possibleInputs.size(); z++) {
						char temp = possibleInputs.at(z);
						DFA * temp1 = new DFA();
						DFA * temp2 = new DFA();
						if ((allDFAStates->at(i))->to->find(temp)
								== (allDFAStates->at(i))->to->end())
							temp1 = NULL;
						else
							temp1 = (allDFAStates->at(i))->to->at(temp);

						if ((allDFAStates->at(j))->to->find(temp)
								== (allDFAStates->at(j))->to->end())
							temp2 = NULL;
						else
							temp2 = (allDFAStates->at(j))->to->at(temp);
						if ((temp1 != NULL && temp2 == NULL)
								|| (temp1 == NULL && temp2 != NULL)) {
							implicat[i][j] = 1;
							noOfChanges++;
						} else if (temp1 != NULL && temp2 != NULL) {
							int i1 = getIndex(temp1);
							int j1 = getIndex(temp2);
							if (i1 != j1) {
								int i2 = max(j1, i1);
								int j2 = min(j1, i1);
								if (implicat[i2][j2] == 1) {
									implicat[i][j] = 1;
									noOfChanges++;
								}
							}
						}
					}
				}
			}
		}
	} while (noOfChanges > 0);
	allDFASize = allDFAStates->size();
}

//****************this method used to print DFA states in file named "DFA.txt"**********************

void DFA::printDFA() {
	vector<DFA*>* v = new vector<DFA*>;
	v = getAllDFA();
	vector<State*>* current = new vector<State*>;
	outfile << "-----------------------------------------------------------"
			<< endl;
	for (int i = 0; i < v->size(); i++) {
		if (i == 0)
			outfile << "the " << i + 1 << "st DFA state is (" << v->at(i)->name
					<< "), accepted is " << v->at(i)->accepted << " ---- "
					<< v->at(i)->token << endl;
		else if (i == 1)
			outfile << "the " << i + 1 << "nd DFA state is (" << v->at(i)->name
					<< "), accepted is " << v->at(i)->accepted << " ---- "
					<< v->at(i)->token << endl;

		else if (i == 2)
			outfile << "the " << i + 1 << "rd DFA state is (" << v->at(i)->name
					<< "), accepted is " << v->at(i)->accepted << " ---- "
					<< v->at(i)->token << endl;

		else
			outfile << "the " << i + 1 << "th DFA state is (" << v->at(i)->name
					<< "), accepted is " << v->at(i)->accepted << " ---- "
					<< v->at(i)->token << endl;

		outfile << "-----------------------------------------------------------"
				<< endl;
		current = equalStates->at(v->at(i)->name);
		outfile << "Its equal states are:" << endl;
		for (int j = 0; j < current->size(); j++) {
			outfile << current->at(j)->id << " , ";
		}
		outfile << endl;
		outfile << "Its transion table is: " << endl;
		for (map<char, DFA*>::iterator it = v->at(i)->to->begin();
				it != v->at(i)->to->end(); ++it)
			outfile << it->first << " => " << it->second->name << '\n';
		outfile << "-----------------------------------------------------------"
				<< endl;

	}
}

//****************this method used to make the minimum DFA states **********************

void DFA::getMiniDFA() {
	minimizedDFAStates = new vector<int>;
	getMaxEqualState();
	while (minimizedDFAStates->size() > 1) {
		int ii = minimizedDFAStates->at(0);
		int replacedState = saved.at(ii);
		for (int i = 1; i < minimizedDFAStates->size(); i++) {
			int jj = minimizedDFAStates->at(i);
			int index = saved.at(jj);
			allDFAStates->erase(
					allDFAStates->begin() + (getIndexByName(index) - 1));
			for (int j = 0; j < allDFAStates->size(); j++) {
				for (map<char, DFA*>::iterator it =
						allDFAStates->at(j)->to->begin();
						it != allDFAStates->at(j)->to->end(); ++it) {
					if (it->second->name == index) {
						it->second->name = replacedState;
					}

				}
			}
			for (int k = 0; k < implicat.size(); k++) {
				implicat[minimizedDFAStates->at(i)][k] = 1;
				implicat[k][minimizedDFAStates->at(i)] = 1;
			}

		}
		getMaxEqualState();
	}

}

//****************this method used to return the index of the given DFA state by name **********************

int getIndexByName(int temp) {
	for (int i = 0; i < allDFAStates->size(); i++) {
		if (allDFAStates->at(i)->name == temp) {
			return i;
		}
	}
}

//****************this method used to get the maximum clique of the implicant table  **********************

void getMaxEqualState() {
	int n, i, j, k, K = allDFASize, p, q, r, s, min, edge, counter = 0;
	n = allDFASize;
	int maxLength = 0;
//Find Neighbors
	vector<vector<int> > neighbors;
	for (i = 0; i < implicat.size(); i++) {
		vector<int> neighbor;
		for (j = 0; j < implicat[i].size(); j++)
			if (implicat[i][j] == 1)
				neighbor.push_back(j);
		neighbors.push_back(neighbor);
	}
	k = n - K;
//Find Cliques
	bool found = false;
	min = n + 1;
	vector<vector<int> > covers;
	vector<int> allcover;
	for (i = 0; i < implicat.size(); i++)
		allcover.push_back(1);
	for (i = 0; i < allcover.size(); i++) {
		if (found)
			break;
		counter++;
		vector<int> cover = allcover;
		cover[i] = 0;
		cover = procedure_1(neighbors, cover);
		s = cover_size(cover);
		if (s < min)
			min = s;
		if (s <= k) {
			if ((n - s) > maxLength) {
				minimizedDFAStates = new vector<int>;
				maxLength = (n - s);
				for (j = 0; j < cover.size(); j++) {
					if (cover[j] == 0)
						minimizedDFAStates->push_back(j);
				}
			}

			covers.push_back(cover);
			found = true;
			break;
		}
		for (j = 0; j < n - k; j++)
			cover = procedure_2(neighbors, cover, j);
		s = cover_size(cover);
		if (s < min)
			min = s;
		if ((n - s) > maxLength) {
			minimizedDFAStates = new vector<int>;
			maxLength = (n - s);
			for (j = 0; j < cover.size(); j++) {
				if (cover[j] == 0)
					minimizedDFAStates->push_back(j);
			}
		}
		covers.push_back(cover);
		if (s <= k) {
			found = true;
			break;
		}
	}
//Pairwise Intersections
	for (p = 0; p < covers.size(); p++) {
		if (found)
			break;
		for (q = p + 1; q < covers.size(); q++) {
			if (found)
				break;
			vector<int> cover = allcover;
			for (r = 0; r < cover.size(); r++)
				if (covers[p][r] == 0 && covers[q][r] == 0)
					cover[r] = 0;
			cover = procedure_1(neighbors, cover);
			s = cover_size(cover);
			if (s < min)
				min = s;
			if (s <= k) {
				if ((n - s) > maxLength) {
					minimizedDFAStates = new vector<int>;
					maxLength = (n - s);
					for (j = 0; j < cover.size(); j++) {
						if (cover[j] == 0)
							minimizedDFAStates->push_back(j);
					}
				}
				found = true;
				break;
			}
			for (j = 0; j < k; j++)
				cover = procedure_2(neighbors, cover, j);
			s = cover_size(cover);
			if (s < min)
				min = s;
			if ((n - s) > maxLength) {
				minimizedDFAStates = new vector<int>;
				maxLength = (n - s);
				for (j = 0; j < cover.size(); j++) {
					if (cover[j] == 0)
						minimizedDFAStates->push_back(j);
				}
			}
			if (s <= k) {
				found = true;
				break;
			}
		}

	}
}

//****************this method used to help getting the maximum clique of the implicant table  **********************

bool removable(vector<int> neighbor, vector<int> cover) {
	bool check = true;
	for (int i = 0; i < neighbor.size(); i++)
		if (cover[neighbor[i]] == 0) {
			check = false;
			break;
		}
	return check;
}

//****************this method used to help getting the maximum clique of the implicant table  **********************

int max_removable(vector<vector<int> > neighbors, vector<int> cover) {
	int r = -1, max = -1;
	for (int i = 0; i < cover.size(); i++) {
		if (cover[i] == 1 && removable(neighbors[i], cover) == true) {
			vector<int> temp_cover = cover;
			temp_cover[i] = 0;
			int sum = 0;
			for (int j = 0; j < temp_cover.size(); j++)
				if (temp_cover[j] == 1
						&& removable(neighbors[j], temp_cover) == true)
					sum++;
			if (sum > max) {
				max = sum;
				r = i;
			}
		}
	}
	return r;
}

//****************this method used to help getting the maximum clique of the implicant table  **********************

vector<int> procedure_1(vector<vector<int> > neighbors, vector<int> cover) {
	vector<int> temp_cover = cover;
	int r = 0;
	while (r != -1) {
		r = max_removable(neighbors, temp_cover);
		if (r != -1)
			temp_cover[r] = 0;
	}
	return temp_cover;
}

//****************this method used to help getting the maximum clique of the implicant table  **********************

vector<int> procedure_2(vector<vector<int> > neighbors, vector<int> cover,
		int k) {
	int count = 0;
	vector<int> temp_cover = cover;
	int i = 0;
	for (int i = 0; i < temp_cover.size(); i++) {
		if (temp_cover[i] == 1) {
			int sum = 0, index;
			for (int j = 0; j < neighbors[i].size(); j++)
				if (temp_cover[neighbors[i][j]] == 0) {
					index = j;
					sum++;
				}
			if (sum == 1 && cover[neighbors[i][index]] == 0) {
				temp_cover[neighbors[i][index]] = 1;
				temp_cover[i] = 0;
				temp_cover = procedure_1(neighbors, temp_cover);
				count++;
			}
			if (count > k)
				break;
		}
	}
	return temp_cover;
}

//****************this method used to help getting the maximum clique of the implicant table  **********************

int cover_size(vector<int> cover) {
	int count = 0;
	for (int i = 0; i < cover.size(); i++)
		if (cover[i] == 1)
			count++;
	return count;
}
//****************this method used to call the DFA builder **********************

DFA * DFA::nfaToDFA(State * start) {
	buildDFA(start);
	return allDFAStates->at(0);
}
