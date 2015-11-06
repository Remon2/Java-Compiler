/*
 * LeftRecursionEleminationAndLeftFactoring.cpp
 *
 *  Created on: Apr 8, 2015
 *      Author: Naruto
 */

#include "LeftRecursionEleminationAndLeftFactoring.h"
#include "CFGParser.h"

vector<vector<Rule*>*>* rules;
vector<pair<string, int> > repetitions;
map<string, set<int> > places;
map<string, int> subRulesLength;
map<Rule*, vector<vector<Rule*> *>*>* grammarr;
vector<Rule*>* terminalss;
vector<Rule*>* nonTerminalss;

bool ok;
bool ok2;
bool pairCompare(const pair<string, int>& left,
		const pair<string, int>& right) {
	if (left.second == right.second) {
		return left.first.length() > right.first.length();
	} else {
		return left.second > right.second;
	}
}

bool check(Rule*a, Rule* b) {
	cout << "--- Check= " << a->name << " " << b->name;
	return a->name == b->name && a->type == b->type;
}
void print(vector<Rule*> *v, bool x) {
	cout << "<";
	for (int i = 0; i < (signed) v->size(); ++i) {
		if (i != 0) {
			cout << ", ";
		}
		cout << v->at(i)->name;
	}
	if (!x) {
		cout << ">";
	} else {
		cout << ">" << endl;
	}
}
void printVecor(vector<vector<Rule*>*>*v) {
	for (int i = 0; i < (signed) v->size(); ++i) {
		if (i != 0) {
			cout << ", ";
		}
		print(v->at(i), false);
	}
	cout << endl;
}

bool cmpRules(int i, int j, int length) {
	cout << "*** i= " << i << " *** k= " << j << " *** length= " << length
			<< endl;
	cout << "*** i = ";
	vector<Rule*> *v = rules->at(i);
	print(v, true);
	cout << "*** j = ";
	v = rules->at(j);
	print(v, true);
	if ((signed) rules->at(i)->size() < length
			|| (signed) rules->at(j)->size() < length) {
		cout << "=== NOT Matched" << endl;
		return false;
	} else {
		for (int k = 0; k < length; ++k) {
			if (!check(rules->at(i)->at(k), rules->at(j)->at(k))) {
				cout << " => NOT Matched" << endl;
				return false;
			}
		}
	}
	cout << " => Matched" << endl;
	return true;
}

bool leftFactor(Rule* nonTerminal, int n) {
	repetitions.clear();
	places.clear();
	subRulesLength.clear();
	rules = grammarr->at(nonTerminal);
	for (unsigned int i = 0; i < rules->size(); ++i) {     // vv length   i=rule
		for (unsigned int j = 1; j <= rules->at(i)->size(); ++j) { // v  j=rule length
			for (unsigned int k = i + 1; k < rules->size(); ++k) { // vv length
				if (cmpRules(i, k, j)) { // i = k
					cout << "************ Main Matched ************" << endl;
					cout << "+++ i= ";
					print(rules->at(i), true);
					cout << "+++ j= ";
					print(rules->at(k), true);

					// make vector string
					string key = "";
					for (unsigned int m = 0; m < j; ++m) {
						key += rules->at(i)->at(m)->name + " ";
					}
					cout << "----- Key= " << key << endl;
					if (places.find(key) != places.end()) {
						cout << "*** Found in Map" << endl;
						places[key].insert(k);
						cout << "*** Places vector= ";
						for (set<int>::iterator it = places[key].begin();
								it != places[key].end(); ++it) {
							cout << *it << endl; // Note the "*" here
						}
						cout << endl;
						for (int m = 0; m < (signed) repetitions.size(); ++m) {
							if (repetitions[m].first == key) {
								cout << "*** Increment" << endl;
								repetitions[m].second = places[key].size();
								break;
							}
						}
					} else {
						cout << "*** Not Found in Map" << endl;
						set<int> v;
						v.insert(i);
						v.insert(k);
						places.insert(pair<string, set<int> >(key, v));
						cout << "*** Places vector= ";
						for (set<int>::iterator it = places[key].begin();
								it != places[key].end(); ++it) {
							cout << *it << endl; // Note the "*" here
						}
						cout << endl;
						subRulesLength.insert(pair<string, int>(key, j));
						repetitions.push_back(
								pair<string, int>(key, places[key].size()));
					}
					cout << "****************" << endl;
					ok = false;
					cout << "*** Repetition Size= " << repetitions.size()
							<< endl;
					cout << "****************" << endl;

				}
			}
		}
	}

	// if repetitions ==1 then no repetitions
	if (repetitions.size() == 0) {
		cout << "============== Hasn't Repetition ==============" << endl;
		return true;
	} else {
		cout << "============== Has Repetition ==============" << endl;

		// sort to take the most repeated part

		cout << "******* Repetitions Before Sorting *******" << endl;
		for (int i = 0; i < (signed) repetitions.size(); ++i) {
			cout << repetitions[i].first << " *** " << repetitions[i].second
					<< endl;
		}
		sort(repetitions.begin(), repetitions.end(), pairCompare);

		cout << "******* Repetitions After Sorting *******" << endl;
		for (int i = 0; i < (signed) repetitions.size(); ++i) {
			cout << repetitions[i].first << " *** " << repetitions[i].second
					<< endl;
		}
		cout << "**********************************" << endl;
		if (repetitions[0].second < 2) {
			cout << "---------- ERROR ----------" << endl;
		}

		// make new rule
		Rule* newKey = new Rule(nonTerminal->name + "`", 0);
		// take the rules after the repeated part
		vector<vector<Rule*>*>* newRules = new vector<vector<Rule*>*>;
		// get the vectors of the repeated part
		string key = repetitions[0].first;
		int subRLength = subRulesLength[key];
//		vector<int> index = places[key];
		vector<int> index;
		copy(places[key].begin(), places[key].end(), back_inserter(index));
		cout << "==== New Rules ====" << endl;
		for (int i = 0; i < (signed) index.size(); ++i) {
			vector<Rule*> x(rules->at(index[i])->begin() + subRLength,
					rules->at(index[i])->end());
			cout << " Rule" << i << "= ";
			print(&x, true);
			vector<Rule*>* xx = new vector<Rule*>;
//			xx = &x;
			copy(rules->at(index[i])->begin() + subRLength,
					rules->at(index[i])->end(), back_inserter(*xx));
			if (xx->size() == 0) {
				cout << "--- EMPTY New Rule Vector" << endl;
				xx->push_back(new Rule("\\L", 2));
			}
			newRules->push_back(xx);
		}
		cout << "==== New Rules = ";
		for (int i = 0; i < (signed) newRules->size(); ++i) {
			for (int j = 0; j < (signed) newRules->at(i)->size(); ++j) {
				if (j != 0) {
					cout << "-";
				}
				cout << newRules->at(i)->at(j)->name;
			}
			if (i != (signed) newRules->size() - 1) {
				cout << ", ";
			}
		}
		cout << endl;

		/*
		 *	remove Rules after repeated part
		 *  add new Rule after the repeated part
		 */
		for (int i = 0; i < (signed) index.size(); ++i) {
			rules->at(index[i])->erase(
					rules->at(index[i])->begin() + subRLength,
					rules->at(index[i])->end());
			rules->at(index[i])->push_back(newKey);
		}
		// remove repeated vectors
		for (int i = index.size() - 1; i > 0; i--) {
			rules->erase(rules->begin() + index[i]);
		}

		// add new Rule to the map
		grammarr->insert(
				pair<Rule*, vector<vector<Rule*>*>*>(newKey, newRules));
		// add new Rule to the nonTerminal vector under the main Rule
		nonTerminalss->insert(nonTerminalss->begin() + n + 1, newKey);
		vector<Rule*>::iterator x = nonTerminalss->begin() + n;

//		cout << "+++ New Rule Index= " <<  << endl;
	}
	cout << "++++++++++++++++++++++ VECTOR ++++++++++++++++++" << endl;

	for (int i = 0; i < (signed) rules->size(); ++i) {
		for (int j = 0; j < (signed) rules->at(i)->size(); ++j) {
			if (j != 0) {
				cout << "-";
			}
			cout << rules->at(i)->at(j)->name;
		}
		cout << ", ";
	}
	cout << endl;
	cout << "++++++++++++++++++++++ MAP ++++++++++++++++++" << endl;
	typedef map<Rule*, vector<vector<Rule*> *>*>::iterator it_type;
	for (it_type iterator = grammarr->begin(); iterator != grammarr->end();
			iterator++) {
		cout << "Key= " << iterator->first->name << " Value=";
		printVecor(iterator->second);
	}
	cout << "++++++++++++++++++++++++++++++++++++++++" << endl;
	return false;
}
void allLeftFactor() {
	int i = 0;
	int nonTerminalsSize = nonTerminalss->size();
	while (i < nonTerminalsSize) {
		cout << "************************** i= " << i
				<< " **************************" << endl;
		ok2 = true;
		while (ok2) {
			if (leftFactor(nonTerminalss->at(i), i)) {
				ok2 = false;
			}
		}
		nonTerminalsSize = nonTerminalss->size();
		i++;
		cout << "*** nonTerminal Size= " << nonTerminalss->size() << endl;
	}
	cout << "********************* FINALLY **********************" << endl;
	cout << "*********************** MAP ************************" << endl;

	typedef map<Rule*, vector<vector<Rule*> *>*>::iterator it_type;
	for (it_type iterator = grammarr->begin(); iterator != grammarr->end();
			iterator++) {
		cout << "Key= " << iterator->first->name << " Value= ";
		printVecor(iterator->second);
	}
	cout << "******************** TERMINALS VECTOR *******************" << endl;
	print(terminalss, true);
	cout << "***************** NON TERMINALS VECTOR ******************" << endl;
	print(nonTerminalss, true);
	cout << "************* FINAL LEFT FACTORING RESULT *************" << endl;
	for (int k = 0; k < (signed) nonTerminalss->size(); k++) {
		cout << "Key= " << nonTerminalss->at(k)->name << " Value= ";
		printVecor(grammarr->at(nonTerminalss->at(k)));
	}
}
/** As Example "exp" == "expression" */
/** return true in case s1 substring from s2 and s begin to substring s1. */
bool CompareTwoString(string s1, string s2) {
	if (s1.length() > s2.length())
		return false;
	else {
		for (int i = 0; i < (signed) s1.length(); i++) {
			if (s1[i] != s2[i])
				return false;
		}
	}
	return true;
}

void leftRecursionElemination() {
	map<Rule*, vector<vector<Rule*>*>*> *g = getGrammarMap();
	for (std::map<Rule*, vector<vector<Rule*>*>*>::iterator it = g->begin();
			it != g->end(); ++it) {
		//cout << "/____________________________________/" << endl;
		Rule *key = it->first;
		int lhs_type = key->type;
		string str_key = key->name;
		bool leftRecursion = false;
//        cout << "Key_node:" <<str_key << ", " << lhs_type << endl;

		vector<vector<Rule*>*> *new_vector = new vector<vector<Rule*>*>;
		string A = str_key + "`";
		Rule *new_Rule = new Rule(A, lhs_type);

		/** Check Existance of Left Recursion. */
		for (unsigned i = 0; i < it->second->size() && !leftRecursion; i++) {
			Rule *elemant = it->second->at(i)->at(0);
			int rhs_type = elemant->type;
			string str_elemant = elemant->name;

			if (lhs_type == rhs_type && str_key == str_elemant) {
				leftRecursion = true;
			}
//            cout << "there is left Recursion:" << leftRecursion << endl;
		}

//        if (leftRecursion) cout << "Remove Left Recursion: " << endl;
		/** Remove Left Recursion. */
		for (unsigned i = 0; i < it->second->size() && leftRecursion; i++) {
			Rule *elemant = it->second->at(i)->at(0);
//			int rhs_type = elemant->type;
			string str_elemant = elemant->name;

//            cout << str_elemant << endl;
			if (str_elemant == "\\L" || str_elemant == "\\l") {
			} else {
				if (CompareTwoString(str_key, str_elemant)) {
					Rule *new_Rule1 = new Rule(A, lhs_type);
					it->second->at(i)->erase(it->second->at(i)->begin());
					it->second->at(i)->push_back(new_Rule1);
					new_vector->push_back(it->second->at(i));
					it->second->erase(it->second->begin() + i);
					i--;
				} else {
					Rule *new_Rule1 = new Rule(A, lhs_type);
					it->second->at(i)->push_back(new_Rule1);
				}
			}
		}
		/** Print new _vector */
		if (new_vector->size() > 0) {
			Rule *new_Rule1 = new Rule("\\L", 2);
			vector<Rule*> *vec = new vector<Rule*>;
			vec->push_back(new_Rule1);
			new_vector->push_back(vec);
		}
		for (unsigned i = 0; i < new_vector->size(); i++) {
			for (unsigned k = 0; k < new_vector->at(i)->size(); k++) {
				Rule *elemant1 = new_vector->at(i)->at(k);
//				int type1 = elemant1->type;
				string name1 = elemant1->name;
//                cout << "i, j, type, name: " << i<< "," << k << ","<< type1 << "," << name1 << endl;
			}
		}

		/** Insert new elemant to map grammer */
		if (new_vector->size() > 0)
			g->insert(
					std::pair<Rule*, vector<vector<Rule*>*>*>(new_Rule,
							new_vector));

		/** Insert new object for nonterminals Vector */
		bool finish = false;
		vector<Rule*> *nonterminals = getNonTerminals();
		for (int i = 0;
				i < (signed) nonterminals->size()
						&& (signed) new_vector->size() > 0 && !finish; i++) {
			if (nonterminals->at(i)->name == str_key) {
				nonterminals->insert(nonterminals->begin() + i + 1, new_Rule);
				finish = true;
				cout << "Added in nonterminal vector" << new_Rule->name << endl;
			}
		}
	}
}

void leftRecursionEleminationAndLeftFactoring() {
	grammarr = getGrammarMap();
	terminalss = getTerminals();
	nonTerminalss = getNonTerminals();
	cout << "################## FIRST LEFT FACTORING #################" << endl;
	allLeftFactor();
	cout << "################ LEFT RECURSION ELEMINATION #############" << endl;
	leftRecursionElemination();
	cout << "################## SECOND LEFT FACTORING ################" << endl;
	allLeftFactor();
}
