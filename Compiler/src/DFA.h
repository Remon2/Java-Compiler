#ifndef DFA_H
#define DFA_H
#include "State.h"
#include <vector>
#include <map>
class DFA {
public:
	bool accepted;
	int name;
	DFA * from;
	map<char, DFA*> *to;
	string token;
	int priority;

	DFA();
	void getEpsillonTransion(vector<State*>* dfaTemp, State* start);
	void addEpsilonTransions(State * start, vector<State*>* in);
	void setAccepted();
	vector<DFA*>* getAllDFA();
	void setUnVisited(State* start);
	DFA* nfaToDFA(State * start);
	State* getCTransion(char c, State* start);
	void buildDFA(State * start);
	void toTransions(DFA * dState);
    void min_DFA();
	void getMiniDFA();
    void printDFA();
	void printNFA(State* start);
	virtual ~DFA();

protected:
private:
};

#endif // DFA_H
