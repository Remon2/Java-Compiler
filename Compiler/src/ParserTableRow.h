/*
 * ParserTableRow.h
 *
 *  Created on: Apr 9, 2015
 *      Author: Beto
 */

#ifndef PARSERTABLEROW_H_
#define PARSERTABLEROW_H_
#include <map>
#include <vector>
#include <string>
#include "Rule.h"
using namespace std;

class ParserTableRow {
public:
	string name;
	map <Rule*,vector<Rule*>* >* tableProduction;
	ParserTableRow(string name);
	virtual ~ParserTableRow();
};

#endif /* PARSERTABLEROW_H_ */
