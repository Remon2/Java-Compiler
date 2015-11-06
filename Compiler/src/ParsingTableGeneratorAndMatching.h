/*
 * ParsingTableGeneratorAndMatching.h
 *
 *  Created on: Apr 11, 2015
 *      Author: Shaimaa Mousa
 */

#ifndef PARSINGTABLEGENERATORANDMATCHING_H_
#define PARSINGTABLEGENERATORANDMATCHING_H_

#include "CFGParser.h"
#include "ParserTableRow.h"
#include <map>
#include <vector>
#include <string>
#include "Rule.h"
using namespace std;

void getFirst();
void getFollow();
void generateParsingTable();
vector<ParserTableRow*>* getParsingTable();

void match() ;


class ParsingTableGeneratorAndMatching {
public:
	ParsingTableGeneratorAndMatching();
	virtual ~ParsingTableGeneratorAndMatching();
};

#endif /* PARSINGTABLEGENERATORANDMATCHING_H_ */
