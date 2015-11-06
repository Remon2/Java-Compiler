/*
 * postfixToNFA.cpp
 *
 *  Created on: Mar 13, 2015
 *      Author: Naruto
 */

#include "PostfixToNFA.h"
#include <exception>
static stack<Expression> s;
static map<string, Expression> m;

void doOperation(string token, char c, bool special) {
	Expression exper1, exper2, exper;
	State* ss;
	if (!special) {
		if (c == '|') {
			if (s.empty()) {
				cout << "*********** STACK EMPTY1 ***********" << endl;
			}
			exper2 = s.top();  //       --e2.start
			s.pop();           //   SS--|
			if (s.empty()) {   //       --e1.start
				cout << "*********** STACK EMPTY2 ***********" << endl;
			}
			exper1 = s.top();
			s.pop();
			ss = new State((char) 257, exper1.start, (&exper2)->start, true);
//			ss->token = "Dummy OR";
			exper = Expression(ss, &exper1, &exper2);
			s.push(exper);
		} else if (c == '.') {
			exper2 = s.top();   //   [start]e1.next->e2.start[next]
			s.pop();
			exper1 = s.top();
			s.pop();
			for (unsigned int i = 0; i < exper1.next.size(); ++i) {
				*(exper1.next[i]) = (exper2.start);
			}
			exper = Expression(exper1.start, (exper2.next));
			exper.size = exper1.size + exper2.size;
			s.push(exper);
		} else if (c == '*') {	//	 .-->e1
			exper1 = s.top();	//   SS<--|
			s.pop();			//    .------>
			ss = new State((char) 257, exper1.start, NULL, true);
//			ss->token = "Dummy";
			for (unsigned int i = 0; i < exper1.next.size(); ++i) {
				*(exper1.next[i]) = ss;
			}
			exper = Expression(ss, 1);
			exper.size = exper1.size + 1;
			s.push(exper);
		} else if (c == '+') {	//	   .---*
			exper1 = s.top();	//    e1->SS->
			s.pop();			//
			ss = new State((char) 257, exper1.start, NULL, true);
//			ss->token = "+++++";
			for (unsigned int i = 0; i < exper1.next.size(); ++i) {
				*(exper1.next[i]) = ss;
			}
			exper = Expression(exper1.start, ss);
			exper.size = exper1.size + 1;
			s.push(exper);
//		} else if (c == '?') {	//	   .---*
//			exper1 = s.top();	//    e1->SS->
//			s.pop();			//
//			ss = new State((char) 257, exper1.start, NULL, true);
//			ss->epsilon = true;
//			ss->split = true;
//			exper = Expression(ss, &exper1);
//			s.push(exper);
		} else {
			State* e = new State(c, NULL, NULL, false);
//			e->token = "Old";
			Expression expression = Expression(e, 0);
			m.insert(pair<string, Expression>(token, expression));
			exper = expression.copy(expression);
			exper.size = 1;
			s.push(exper);
		}
	} else {
		State* e = new State(c, NULL, NULL, false);
//		e->token = "Old";
		Expression expression = Expression(e, 0);
		m.insert(pair<string, Expression>(token, expression));
		exper = expression.copy(expression);
		exper.size = 1;
		s.push(exper);
	}
}

Expression postfixToNFA(vector<string> postfix, int priority) {
	string token;
	Expression tokenE;

	for (unsigned int i = 2; i < postfix.size(); ++i) {
		token = postfix[i];
		if (m.find(token) != m.end()) { // found -> get the expression from the map
			tokenE = m[token].copy(m[token]);
			s.push(tokenE);
		} else { // not found -> make the expression and add it to the map (not found = new char)
			if (token.length() == 1) {
				doOperation(token, token[0], false); // new Letter or Operator
			} else if (token.length() == 2 && token[0] == '\\') {
				doOperation(token, token[1], true); // new Letter or Operator
			} else {
//				doOperation(token, token[1], false); // new Letter or Operator
				cout << "!" << token << "! " << token.length() << "!" << endl;
				cout << "Wrong Token postfixToNFA() !!!" << endl;
				return Expression(NULL, 0);
			}
		}

	}
	tokenE = s.top();
	s.pop();
	if (!s.empty()) {
		cout << "*********** STACK NOOOOOOT EMPTY ***********" << endl;
	}
	State* accepted;
	if (postfix[1] == ":") {
		accepted = new State((char) 257, NULL, NULL, false, true, postfix[0]);
		accepted->epsilon = true;
		accepted->priority = priority;
	} else {
		accepted = new State((char) 257, NULL, NULL, false, false, postfix[0]);
		accepted->epsilon = true;
	}
	Expression z = Expression(accepted, 0);
	z.size = 1;
	for (unsigned int i = 0; i < tokenE.next.size(); ++i) {
		*(tokenE.next[i]) = (z.start);
	}
	Expression a = Expression(tokenE.start, z.next);
	a.size = tokenE.size + z.size;
	return a;
}

void printState(State *state) {
	string z = "";
	string zz = "";
	string zzz = "";
	string zzzz = "";
	string x = "";
	string xx = "";
	if (state->next != NULL) {
		z = "Has Next";
//		ostringstream convert;   // stream used for the conversion
//		convert << state->next->token;
		x = " ----- Next1= " + state->next->id;
//		x = " ----- Next1= " + convert.str();
	} else {
		z = "No Next";
		x = "";
	}
	if (state->split) {
		zz = "Has Split";
//		ostringstream convert;   // stream used for the conversion
//		convert << state->next2->token;
		xx = " ----- Next2= " + state->next2->id;
//		xx = " ----- Next2= " + convert.str();
	} else {
		zz = "No Split";
	}
	zzzz = state->token;
	if (zzzz != "") {
		zzzz = " ----- " + zzzz;
	}
	if (state->accepted) {
		zzz = " ----- ACCEPTED";
	}
	cout << "char:" << state->id << zzzz << " ----- " << z << " ----- " << zz
			<< x << xx << zzz << " ----- " << state->id << endl;
	if (state->next != NULL) {
		printState(state->next);
	}
	if (state->split) {
		printState(state->next2);
	}

}

State* convertToNFA(vector<vector<string> > postfix) {
	vector<Expression> expressionVector;
	Expression collectedNFAs;
	Expression expression;

	State* e = new State((char) 257, NULL, NULL, false);
	e->epsilon = true;
	Expression epsilon = Expression(e, 0);
	m.insert(pair<string, Expression>("\\L", epsilon));

	for (unsigned int i = 0; i < postfix.size(); ++i) {
		if (postfix[i][0] == "reserved") {
			expression = postfixToNFA(postfix[i], 0);
		} else {
			expression = postfixToNFA(postfix[i], i + 1);
		}
		m.insert(pair<string, Expression>(postfix[i][0], expression));
		if (postfix[i][1] == ":") {
			expressionVector.push_back(expression);
		}
	}
	for (unsigned int i = 0; i < expressionVector.size(); ++i) {
		s.push(expressionVector[i]);
	}

	while (s.size() > 1) {
		doOperation("Connecting", '|', false);
	}
	collectedNFAs = s.top();
	s.pop();

//	getID = 0;
	cout << "************ NFA Size= " << collectedNFAs.size << " ************"
			<< endl;
	return collectedNFAs.start;
}
