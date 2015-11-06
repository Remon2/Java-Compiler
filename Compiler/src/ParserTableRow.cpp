/*
 * ParserTableRow.cpp
 *
 *  Created on: Apr 9, 2015
 *      Author: Beto
 */

#include "ParserTableRow.h"
#include <map>
#include <vector>
#include <string>
#include "Rule.h"
using namespace std;

ParserTableRow::ParserTableRow(string name){
	this->name = name;
	this->tableProduction = new map <Rule*,vector<Rule*>* >;

}

ParserTableRow::~ParserTableRow() {
}

