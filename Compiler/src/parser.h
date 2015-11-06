#ifndef POSTFIX_AND_PARSING
#define POSTFIX_AND_PARSING

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
#include <vector>
#include <algorithm>
#include <cctype>

using namespace std;
vector<string> parseLine(string line);
string checkSpace(string str);
void expand();
void readFromFile(char*pathOfFile);
void print_vector_2d(vector<vector<string> > *v2d);
void print_vector_1d(vector<string> *v1d);
vector<string> toPostFix(vector<string> inFix);
int find_def(string st);
void check_for_previous_definitions();
vector<string> add_dots(vector<string> *exp);
void remove_spaces(vector<vector<string> > *v);
vector<string> remove_spaces_1d(vector<string> old);
void run_parsing();
vector<vector<string> >* get_Postfix();
void expand_keywords_and_punctuations();
bool isReserved(string st);
bool isPunctuation(string st);
void add_dots_to_parsed_vector();
void remove_hash(vector<vector<string> > *vect);
bool isAlphabetic(string st);
void seperate(vector<vector<string> > *vect);
void removeSlash(vector<vector<string> > *vect);
void addAssignmant(vector<vector<string> > *v);
void repairPunc(vector<vector<string> > *v);
#endif // POSTFIX_AND_PARSING
