/*
 * Rule.h
 *
 *  Created on: Apr 8, 2015
 *      Author: Naruto
 */

#ifndef RULE_H_
#define RULE_H_
#include <string>
using namespace std;

class Rule {
public:
	string name;
	int type;
	bool operator <(const Rule& c) const {
		return name == c.name && type == c.type;
	}

	Rule();
	Rule(string name, int type);
	virtual ~Rule();
};

#endif /* RULE_H_ */
