/*
 * Token.cpp
 *
 *  Created on: Mar 17, 2015
 *      Author: Naruto
 */

#include "Token.h"

Token::Token() {
	// TODO Auto-generated constructor stub
}

Token::Token(string identifier, string lexeme) {
	this->identifier = identifier;
	this->lexeme = lexeme;
}
Token::~Token() {
	// TODO Auto-generated destructor stub
}

