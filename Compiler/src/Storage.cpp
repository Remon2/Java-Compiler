#include "Storage.h"

/*
 * bool accepted;
 * int name;
 * string token;
 * int priority;
 * DFA * from;
 * map<char, DFA*> *to;
 */

/*vector size
 *
 *
 * bool accepted;
 * int name;
 * string token;
 * int priority;
 * DFA * from;
 * map size
 * map<char, DFA*> *to;
 */
map<int, DFA**> nameToPointer;
vector<DFA*> newDFAVector;
void storeGraph() {
	cout << "********************* STORE DFA ********************" << endl;
	DFA* state;
	vector<DFA*> dfaVector = *(state->getAllDFA());
	ofstream myfile("graph.txt");

	myfile << dfaVector.size() << "\n"; // size of dfa
	cout << "*** VectorSize= " << dfaVector.size() << " ***" << endl;
	for (int i = 0; i < dfaVector.size(); i++) {
		cout << "********************* i= " << i << " ********************"
				<< endl;
		state = dfaVector[i];
		if (state->accepted) { // accepted
			myfile << "true" << endl;
		} else {
			myfile << "false" << endl;
		}
		cout << "--- Accepted= " << state->accepted << endl;
		myfile << state->name << endl; // name
		cout << "--- Name= " << state->name << endl;
		if (state->token == "") { // token
			myfile << "-" << endl;
			cout << "--- Token= -" << endl;
		} else {
			myfile << state->token << endl;
			cout << "--- Token= " << state->token << endl;
		}
		myfile << state->priority << endl; // priority
		cout << "--- Priority= " << state->priority << endl;
		/* Store (DFA * from) */
		if (state->from == NULL) { // from
			myfile << "-1" << endl;
			cout << "--- From= -1" << endl;
		} else {
			myfile << state->from->name << endl;
			cout << "--- From= " << state->from->name << endl;
		}
		/* Store (map<char, DFA*> *to) */
		int length = state->to->size();
		myfile << length << endl; // mapSize
		map<char, DFA*> m = *(state->to);
		cout << "--- MapSize= " << m.size() << endl;
		for (map<char, DFA*>::iterator it = m.begin(); it != m.end(); it++) {
			myfile << it->first << endl;
			myfile << it->second->name << endl;
			cout << "--- Char= " << it->first << " --- toState= "
					<< it->second->name << endl;
			cout << "----------------------------" << endl;
		}
	}
	myfile.close();
	cout << "*** " << dfaVector.size() << " ***" << endl;
	for (int i = 0; i < dfaVector.size(); i++) {
		cout << "*** " << dfaVector[i]->name << endl;
	}
}

vector<DFA*>* getStorageGraph() {
	cout << "********************* First Loop ********************" << endl;
	/* Will use throught function and finall will return return*/
	/* For read every line in file. */
	string line;
	/* Open file. */
	char* pathOfFile = "graph.txt";
	ifstream in_stream;
	in_stream.open(pathOfFile);

	/* Length of Vector element. */
	int length;
	if (getline(in_stream, line)) {
		istringstream buffer9(line);
		buffer9 >> length;
	}
	cout << "********************* Vector Size= " << length
			<< " ********************" << endl;
	for (int i = 0; i < length; i++) {
		cout << "********************* i= " << i << " ********************"
				<< endl;
		/* Object will push in vector of pointer object. */
		DFA* newState = new DFA();
		/* Set first three variable : boolean, integer, string */
		std::getline(in_stream, line);
		if (line == "true") { // accepted
			newState->accepted = true;
		} else if (line == "false") {
			newState->accepted = false;
		} else {
			cout << "*** Reading Error ***" << endl;
		}
		cout << "--- Accepted= " << newState->accepted << endl;
		int name;
		getline(in_stream, line);
		istringstream buffer(line);
		buffer >> name;
		newState->name = name; //name
		cout << "--- Name= " << newState->name << endl;

		getline(in_stream, line);
		if (line == "-") { // token
			newState->token = "";
		} else {
			newState->token = line;
		}
		cout << "--- Token= " << newState->token << endl;

		getline(in_stream, line);
//		getline(in_stream, line)
		istringstream buffer22(line);
		buffer22 >> name;
		newState->priority = name; //priority
		cout << "--- Priority= " << newState->priority << endl;

		getline(in_stream, line); // from

		/* Push object in vector (object is set except map element. ) */
		cout << "@@@@@@ " << newState->name << endl;
		newDFAVector.push_back(newState);
		nameToPointer.insert(pair<int, DFA**>(newState->name, &newState));
		cout << "####### " << newDFAVector[newDFAVector.size() - 1]->name
				<< endl;
		/* use map but it set in next loop. */
		int mapLength;
		std::getline(in_stream, line);
		istringstream buffer2(line);
		buffer2 >> mapLength;
		for (int j = 0; j < mapLength; j++) {
			getline(in_stream, line);
			getline(in_stream, line);
		}
	}
	in_stream.close();

	/*
	 * Second Loop
	 */
	cout << "********************* Second Loop ********************" << endl;
	ifstream in_stream1;
	in_stream1.open(pathOfFile);
	getline(in_stream1, line);
	for (int i = 0; i < length; i++) {
		cout << "********************* i= " << i << " ********************"
				<< endl;
		for (int j = 0; j < 4; j++) {
			getline(in_stream1, line);
			cout << "--- RRR= " << line << endl;
		}
		getline(in_stream1, line);
		int name;
		istringstream buffer22(line);
		buffer22 >> name;
		cout << "--- From= " << name << endl;
		if (name == -1) {
			cout << "@@@ From= " << name << endl;
			newDFAVector[i]->from = NULL;
		} else {
			cout << "$$$ From= " << name << endl;
			newDFAVector[i]->from = *nameToPointer[name];
		}
//		if (name != 983) {
//		}

		map<char, DFA*> *toVector = new map<char, DFA*>;
		/* Get length of map element. */
		int mapLength;
		std::getline(in_stream1, line);
		istringstream buffer2(line);
		buffer2 >> mapLength;
//		cout << "********************* TEST *********************" << endl;
		cout << "--- MapSize= " << mapLength << endl;
		for (int j = 0; j < mapLength; j++) {
			char ch;
			int toState;
			getline(in_stream1, line);
			ch = line[0];
			getline(in_stream1, line);
			istringstream buffer3(line);
			buffer3 >> toState;
			cout << "--- Char= " << ch << " --- toState= " << toState << endl;
			if (nameToPointer.find(toState) != nameToPointer.end()) {

				toVector->insert(
						pair<char, DFA*>(ch, *(nameToPointer[toState])));
			} else {
				cout << "--- State= " << toState << " --- DOT FOUND ---"
						<< endl;
			}
			cout << "----------------------------" << endl;
		}
		/* Edit in vector i. */
		newDFAVector.at(i)->to = toVector;
	}
	in_stream1.close();

	return &newDFAVector;
}
/**
 graph.txt formate:
 __________________
 -length of vector
 object i:
 -bool accepted;
 -int name;
 -string token;
 DFA * from;
 - from (id) and start=-1
 map<char, DFA*> *to;
 -length
 -id of next i
 */
