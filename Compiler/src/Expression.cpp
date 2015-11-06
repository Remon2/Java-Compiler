/*
 * Expression.cpp
 *
 *  Created on: Mar 13, 2015
 *      Author: Naruto
 */

#include "Expression.h"
static vector<State*> oldStates;
static vector<State*> newStates;
static map<State**, State*> p2s; // old pointer to its old state
static map<State*, int> s2n;    // old state to number
static map<State**, int> p2n;    // old pointer to its number in its state

Expression::Expression() {
	start = NULL;
	size = 1;
}

Expression::Expression(State* state, int type) {
	this->start = state;
	if (type == 0) {
		next.push_back(&(state->next));
	} else {
		next.push_back(&(state->next2));
	}
	this->size = 1;
}
Expression::Expression(State* start, Expression* end1, Expression* end2) {
	this->start = start;
	this->next.insert(this->next.end(), end1->next.begin(), end1->next.end());
	this->next.insert(this->next.end(), end2->next.begin(), end2->next.end());
	this->size = end1->size + end2->size + 1;
}

Expression::Expression(State* state, vector<State**> next) {
	this->start = state;
	this->next.insert(this->next.end(), next.begin(), next.end());
	this->size = 1;
}

Expression::Expression(State* state, State* state2) {
	this->start = state;
	this->next.push_back(&(state2->next2));
	this->size = 2;
}

Expression::Expression(State* state, Expression* end) {
	this->start = state;
	this->start->next = end->start;
	this->next.insert(this->next.end(), end->next.begin(), end->next.end());
	this->next.push_back(&(this->start->next2));
	this->size = end->size + 1;

}
Expression Expression::copy(Expression e) {
	oldStates.clear();
	newStates.clear();
	s2n.clear();
	p2n.clear();
	p2s.clear();

	vector<State**> newNext;

	traverse(e.start); //get the old states in oldStates
	/*
	 * make new states
	 * map old states to new states
	 * map old pointers to its state    (for vector next)
	 * map old pointers to its number   (for vector next)
	 */

	/*
	 * add new states to newStates
	 */
	for (unsigned int i = 0; i < oldStates.size(); ++i) {
		State* x = new State((*oldStates[i]).c, NULL, NULL,
				(*oldStates[i]).split);
//		x->reserved = oldStates[i]->reserved;
		x->accepted = oldStates[i]->accepted;
		x->token = oldStates[i]->token;
		x->epsilon = oldStates[i]->epsilon;
		x->priority = oldStates[i]->priority;
//		ostringstream convert;   // stream used for the conversion
//		convert << e.start->c;
//		convert << " ";
//		convert << q++; // insert the textual representation of 'Number' in the characters in the stream
//		x->token = "State " + convert.str();
		newStates.push_back(x);
	}
	/*
	 * map old pointers to its state    (for vector next)
	 * map old pointers to its number   (for vector next)
	 */
	for (unsigned int i = 0; i < oldStates.size(); ++i) {
		p2n.insert(pair<State**, int>(&((oldStates[i])->next), 1));
		p2n.insert(pair<State**, int>(&((oldStates[i])->next2), 2));

		p2s.insert(
				pair<State**, State*>(&((oldStates[i])->next), oldStates[i]));
		p2s.insert(
				pair<State**, State*>(&((oldStates[i])->next2), oldStates[i]));
	}
	/*
	 * make every new state pointers (next, next2)
	 * points to the new mapped state
	 */
	for (unsigned int i = 0; i < oldStates.size(); ++i) {
		if ((oldStates[i])->next != NULL) {
			(newStates[i])->next = newStates[s2n[(oldStates[i])->next]];
		}
		if ((oldStates[i])->next2 != NULL) {
			(newStates[i])->next2 = newStates[s2n[(oldStates[i])->next2]];
		}
	}

	/*
	 * Add new Pointers to the vector
	 */
	for (unsigned int i = 0; i < e.next.size(); ++i) {
		int pn = p2n[(e.next[i])]; // get number of pointer
		State* ps = p2s[(e.next[i])]; // get state of pointer
		int sn = s2n[ps]; // get number of state

		if (pn == 1) {
			newNext.push_back(&((newStates[sn])->next));
		} else if (pn == 2) {
			newNext.push_back(&((newStates[sn])->next2));
		} else {
			cout << "********** Error at Expression.copy() **********" << endl;
		}
	}
	Expression a = Expression(newStates[s2n[e.start]], newNext);
	a.size = e.size;
	return a;
}

void Expression::traverse(State *state) {
	if (!state->visited) {
		state->visited = true;
		oldStates.push_back(state);
		s2n.insert(pair<State*, int>(state, oldStates.size() - 1));
		if (state->next != NULL) {
			traverse(state->next);
		}
		if (state->split) {
			traverse(state->next2);
		}
		state->visited = false;
	}
}

void Expression::deleteExpression(State *state) {
	if (!state->visited) {
		state->visited = true;
		if (s2n.find(state) == s2n.end()) {
			oldStates.push_back(state);
		}
		s2n.insert(pair<State*, int>(state, oldStates.size() - 1));
		if (state->next != NULL) {
			deleteExpression(state->next);
		}
		if (state->split) {
			deleteExpression(state->next2);
		}
		state->visited = false;
//		delete (state);
	}
}
Expression::~Expression() {
//	oldStates.clear();
//	deleteExpression(this->start);
//	for (unsigned int i = 0; i < oldStates.size(); ++i) {
//		delete (oldStates[i]);
//	}

}
