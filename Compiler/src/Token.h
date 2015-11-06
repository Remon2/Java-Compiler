/*
 * Token.h
 *
 *  Created on: Mar 17, 2015
 *      Author: Naruto
 */

#ifndef TOKEN_H_
#define TOKEN_H_
#include <string>
using namespace std;

class Token {
public:
	Token();
	Token(string identifier, string lexeme);

	virtual ~Token();

	string identifier;
	string lexeme;
};

#endif /* TOKEN_H_ */
