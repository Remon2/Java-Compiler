#ifndef CFGPARSER_H
#define CFGPARSER_H

#include <vector>
#include<map>
#include <fstream>
#include <stack>
#include <iomanip>
#include <math.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>
#include <algorithm>
#include <cctype>
#include "Rule.h"



using namespace std;
void parseLine2(string line);
void readFromFile2(char*pathOfFile);
void printTheMap();
void parse();
map<Rule*, vector<vector<Rule*> *>*>*  getGrammarMap();
vector<Rule*>*  getNonTerminals();
vector<Rule*>*  getTerminals();
bool isPushed(string name);
void removeDuplicatesOfTerminals();


void print_vector_22d(vector<vector<string> > *v2d);
void print_vector_22d_Rule(vector<vector<Rule> > *v2d);
void print_vector_11d_Rule(vector<Rule*> *v1d);
void print_vector_11d(vector<string> *v1d);
vector<string>& split(const string &s, char delim, vector<string> &elems);
vector<string> split(const string &s, char delim);
void getGrammar();


#endif // CFGPARSER_H
