#include "parser.h"
vector<string> add_dots(vector<string> *exp);
vector<string> toPostFix(vector<string> inFix);
vector<string> input_File2;
vector<string> punctuations;
vector<string> reserved;
vector<vector<string> > parsed;
vector<vector<string> > parsed_with_dots;
vector<vector<string> > postfix;
vector<string> addAssignment;

int search_for(string st) {
	for (int i = 0; i < parsed.size(); i++) {
		if (parsed.at(i).at(0) == st)
			return i;
	}
	return -1;
}

vector<string> parseLine(string line) {
	//	cout<<"line size = "<<line.size()<<endl;
	for (int i = 0; i < line.size(); ++i) {
		if (line[i] == '.') {
			line[i] = '#';
		}
	}
	string buf;
	stringstream ss(line);
	vector<string> token;
	vector<string> newToken;
	char firstChar;
	int x = 0;
	while (line[x] == ' ' || line[x] == '\t') {
		x++;
	}
	firstChar = line[x];
	if (firstChar == '{') {
		int index = search_for("reserved");
		token.push_back("reserved");
		while (ss >> buf) {
			if (!(buf == "{" || buf == "}")) {
				token.push_back(buf);
			}
		}
		newToken.push_back(token.at(0));
		for (int i = 1; i < token.size() - 1; i++) {
			newToken.push_back(token.at(i));
			reserved.push_back(token.at(i));
			newToken.push_back("|");
		}
		newToken.push_back(token.at(token.size() - 1));
		reserved.push_back(token.at(token.size() - 1));
		if (index != -1) {
			parsed.at(index).push_back("|");
			for (int i = 1; i < newToken.size(); i++) {
				parsed.at(index).push_back(newToken.at(i));
			}
		} else {
			parsed.push_back(newToken);
		}
	} else if (firstChar == '[') {
		int index = search_for("punctuations");
		token.push_back("punctuations");
		while (ss >> buf) {
			if (buf != "[" && buf != "]")
				token.push_back(buf);
		}
		newToken.push_back(token.at(0));
		for (int i = 1; i < token.size() - 1; i++) {
			newToken.push_back(token.at(i));
			punctuations.push_back(token.at(i));
			newToken.push_back("|");
		}
		newToken.push_back(token.at(token.size() - 1));
		punctuations.push_back(token.at(token.size() - 1));
		if (index != -1) {
			parsed.at(index).push_back("|");
			for (int i = 1; i < newToken.size(); i++) {
				parsed.at(index).push_back(newToken.at(i));
			}
		} else {
			parsed.push_back(newToken);
		}
	} else {
		string prev_token;
		while (ss >> buf) {
			if ((buf != "=" && buf != ":") || (buf == "=" && prev_token == ":")) {
				token.push_back(buf);
			} else {
				//to handle = or :
				addAssignment.push_back(prev_token);
				addAssignment.push_back(buf);
			}

			prev_token = buf;
		}
		int index = search_for(token.at(0));
		if (index != -1) {
			parsed.at(index).push_back("|");
			for (int i = 1; i < token.size(); i++) {
				parsed.at(index).push_back(token.at(i));
			}
		} else {
			parsed.push_back(token);
		}
	}
	vector<string> to_return(token);
	return to_return;
}

/* This function handle space for some needed position */
string checkSpace(string str) {
	string res = "";
	for (int i = 0; i < str.size(); i++) {
		switch (str[i]) {
		case '\\':
			if (i < (str.size() - 1) && (str[i + 1] == '\\' || str[i + 1]
					== '=' || str[i + 1] == '+' || str[i + 1] == '?' || str[i
					+ 1] == '*' || str[i + 1] == ')' || str[i + 1] == '(')
					|| str[i + 1] == '-')
				;
			else
				res += str[i];
			break;
		case '=':
			if (i > 0 && str[i - 1] != '\\') {
				res += " ";
				res += str[i];
				res += " ";
			} else if (i < (str.size() - 1) && str[i + 1] != '\\') {
				res += str[i];
				res += " ";
			} else {
				res += str[i];
			}
			break;
		case ':':
			res += " ";
			res += str[i];
			res += " ";
			break;
		case '-':
			if (i > 0 && str[i - 1] != '\\') {
				res += " ";
				res += str[i];
				res += " ";
			} else
				res += str[i];
			break;
		case '|':
			if (i > 0 && str[i - 1] != '\\') {
				res += " ";
				res += str[i];
				res += " ";
			} else
				res += str[i];
			break;
		case '(':
			res += " ";
			res += str[i];
			res += " ";
			break;
		case ')':
			res += " ";
			res += str[i];
			res += " ";
			break;
		case '{':
			res += " ";
			res += str[i];
			res += " ";
			break;
		case '}':
			res += " ";
			res += str[i];
			res += " ";
			break;
		case '[':
			res += " ";
			res += str[i];
			res += " ";
			break;
		case ']':
			res += " ";
			res += str[i];
			res += " ";
			break;
		default:
			res += str[i];
		}
	}
	return res;
}

void expand() {
	for (int i = 0; i < parsed.size(); i++) {
		vector<string> newParsed;
		stringstream ss;
		string s;
		for (int j = 0; j < parsed.at(i).size(); j++) {
			if (parsed.at(i).at(j) == "-") {

				if (j + 1 < parsed.at(i).size()) {
					for (int k = (int) parsed.at(i).at(j - 1)[0] + 1; k
							< (int) parsed.at(i).at(j + 1)[0]; k++) {
						char c = (char) k;
						ss << c;
						ss >> s;
						newParsed.push_back(" |");
						newParsed.push_back(s);
						ss.clear();
					}
					newParsed.push_back(" |");
					newParsed.push_back(parsed.at(i).at(j + 1));
					j++;
				} else {
					newParsed.push_back(parsed.at(i).at(j));
				}
			} else {
				newParsed.push_back(parsed.at(i).at(j));
			}
		}
		parsed.at(i) = newParsed;
	}
}

void readFromFile(char*pathOfFile = "lexical_rules.txt") {

	ifstream in_stream;
	string line;
	in_stream.open(pathOfFile);
	while (std::getline(in_stream, line)) {
		if (line.size() == 0)
			continue;
		line = checkSpace(line);
		input_File2.push_back(line);
		parseLine(line);
	}
	in_stream.close();
}

void print_vector_2d(vector<vector<string> > *v2d) {
	for (int i = 0; i < v2d->size(); i++) {
		for (int j = 0; j < v2d->at(i).size(); j++) {
			cout << v2d->at(i).at(j) << " ";
		}
		cout << endl;
	}
}

void print_vector_1d(vector<string> *v1d) {
	for (int j = 0; j < v1d->size(); j++) {
		cout << v1d->at(j) << " ";
	}
	cout << endl;
}

vector<string> toPostFix(vector<string> inFix) {
//	cout << "inFIX" << endl;
//	print_vector_1d(&inFix);
//	cout << "\n\n";

	stack<string> stack;
	vector<string> postFix_vector;
	string postFix = "";
	for (int i = 0; i < inFix.size(); i++) {
		if (inFix[i] != "?" && inFix[i] != "." && inFix[i] != "|" && inFix[i]
				!= "+" && inFix[i] != "*" && inFix[i] != "(" && inFix[i] != ")"
				&& !((i - 1 >= 0 && (inFix[i] == "(" || inFix[i] == ")")
						&& inFix[i - 1] != "|"))) {
			postFix.append(" " + inFix[i]);
			postFix_vector.push_back(" " + inFix[i]);
			//            cout<<"1) inFix[i] = "<< inFix[i]<<endl;
			//            cout<<"   postFix = "<<postFix <<endl;
		} else if (inFix[i] == "|") {
			if (stack.empty() || stack.top() == "(") {
				stack.push(inFix[i]);
			} else if (!stack.empty() && (stack.top() == "|")) {
				while (!stack.empty() && (stack.top() == "|")) {
					postFix.append(" " + stack.top());
					postFix_vector.push_back(" " + stack.top());
					stack.pop();
				}
				stack.push(inFix[i]);
			} else if (!stack.empty() && (stack.top() == "*" || stack.top()
					== "+" || stack.top() == "?" || stack.top() == ".")) {
				while (!stack.empty() && (stack.top() == "*" || stack.top()
						== "+" || stack.top() == "?" || stack.top() == ".")) {
					postFix.append(" " + stack.top());
					postFix_vector.push_back(" " + stack.top());
					stack.pop();
				}
				stack.push(inFix[i]);
			}
		} else if (inFix[i] == "*" || inFix[i] == "+" || inFix[i] == "?"
				|| inFix[i] == ".") {
			if (stack.empty() || stack.top() == "(") {
				stack.push(inFix[i]);
			} else if (stack.top() == "|") {
				stack.push(inFix[i]);
			} else if (stack.top() == "*" || stack.top() == "+" || stack.top()
					== "?" || stack.top() == ".") {
				while (!stack.empty() && (stack.top() == "*" || stack.top()
						== "+" || stack.top() == "?" || stack.top() == ".")) {
					postFix.append(" " + stack.top());
					postFix_vector.push_back(" " + stack.top());
					stack.pop();
				}
				stack.push(inFix[i]);
			}
		} else if (inFix[i] == "(") {
			stack.push(inFix[i]);
		} else if (inFix[i] == ")") {
			while (!stack.empty() && stack.top() != "(") {
				postFix.append(" " + stack.top());
				postFix_vector.push_back(" " + stack.top());
				stack.pop();
			}
			stack.pop();
		}
		if (i + 1 < inFix.size() && inFix[i] == ")" && (inFix[i + 1] == "*"
				|| inFix[i + 1] == "+"|| inFix[i + 1] == "?")) {
			postFix.append(" " + inFix[i + 1]);
			postFix_vector.push_back(" " + inFix[i + 1]);
			i++;
		}
	}
	while (!stack.empty()) {
		postFix.append(" " + stack.top());
		postFix_vector.push_back(" " + stack.top());
		stack.pop();
	}
//	cout << "postfix vector" << endl;
//	print_vector_1d(&postFix_vector);
//	cout << "\n\n";
	return postFix_vector;
}

int find_def(string st) {
	for (int i = 0; i < parsed.size(); i++) {
		if (parsed.at(i).at(0).find(st) != std::string::npos) {
			return i;
		}
	}
	return -1;
}

void check_for_previous_definitions() {
	for (int i = 0; i < parsed.size(); i++) {
		vector<string> newParsed;
		stringstream ss;
		string s;
		if (isReserved(parsed.at(i).at(0)) || isPunctuation(parsed.at(i).at(0))) {
			newParsed = parsed.at(i);
		} else {
			for (int j = 0; j < parsed.at(i).size(); j++) {
				int index = find_def(parsed.at(i).at(j));
				if (index != -1) {
					newParsed.push_back(parsed.at(i).at(j));
				} else {
					if (parsed.at(i).at(j)[parsed.at(i).at(j).length() - 1]
							!= '*'
							&& parsed.at(i).at(j)[parsed.at(i).at(j).length()
									- 1] != '+') {
						for (int k = 0; k < parsed.at(i).at(j).length(); k++) {

							char c = parsed.at(i).at(j)[k];
							ss << c;
							ss >> s;
							newParsed.push_back(s);
							ss.clear();

						}
					} else {
						newParsed.push_back(parsed.at(i).at(j));
					}
					ss.clear();
				}

			}
		}
		parsed.at(i) = newParsed;
	}
}

vector<string> add_dots(vector<string> *exp) {
	vector<string> temp;
	if (exp->size() == 2 && isPunctuation(exp->at(1))) {
		temp.push_back(exp->at(0));
		return temp;
	} else if (exp->size() >= 2 && isReserved(exp->at(1))) {
		stringstream ss;
		char c;
		string s;
		for (int j = 0; j < exp->at(0).length() - 1; j++) {
			c = exp->at(1)[j];
			ss << c;
			ss >> s;
			temp.push_back(s);
			temp.push_back(".");
			ss.clear();
		}
		c = exp->at(1)[exp->at(1).length() - 1];
		ss << c;
		ss >> s;
		temp.push_back(s);

	} else {
		for (int i = 0; i < exp->size(); i++) {

			if (exp->at(i).find("(") != std::string::npos || (i + 1
					< exp->size() && exp->at(i + 1).find(")")
					!= std::string::npos) || (i + 1 < exp->size()
					&& exp->at(i).find(")") != std::string::npos && (exp->at(i
					+ 1).find("*") != std::string::npos || exp->at(i + 1).find(
					"+") != std::string::npos || exp->at(i + 1).find("?")
					!= std::string::npos)) || (i + 1 < exp->size() && (exp->at(
					i).find("+") != std::string::npos || exp->at(i).find("*")
					!= std::string::npos) && exp->at(i + 1).find("?")
					!= std::string::npos) || (i + 1 < exp->size() && exp->at(i
					+ 1).find("|") != std::string::npos)) {
				temp.push_back(exp->at(i));
				continue;
			} else if (exp->at(i) != "|") {
				if (i + 1 < exp->size() && exp->at(i + 1) != "|") {
					temp.push_back(exp->at(i));
					temp.push_back(".");
				} else {
					temp.push_back(exp->at(i));
				}
			} else {
				temp.push_back(exp->at(i));
			}
		}
	}
	return temp;
}

void remove_spaces(vector<vector<string> > *v) {
	for (int i = 0; i < v->size(); i++) {
		for (int j = 0; j < v->at(i).size(); j++) {
			string st = v->at(i).at(j);
			st.erase(std::remove(st.begin(), st.end(), ' '), st.end());
			v->at(i).at(j) = st;
		}
	}
}

vector<string> remove_spaces_1d(vector<string> old) {

	for (int j = 0; j < old.size(); j++) {
		string st = old.at(j);
		st.erase(std::remove(st.begin(), st.end(), ' '), st.end());
		old.at(j) = st;
	}
	return old;

}

void run_parsing() {
	readFromFile();
	expand();
	remove_spaces(&parsed);
	check_for_previous_definitions();
	add_dots_to_parsed_vector();
}

vector<vector<string> >* get_Postfix() {
	run_parsing();
	for (int i = 0; i < parsed_with_dots.size(); i++) {
		vector<string> temp;
		char c;
		c = parsed.at(i).at(0)[0];
		stringstream ss;
		ss << c;
		string s;
		ss >> s;
		if (isPunctuation(s)) {
			temp.push_back(s);
			temp.push_back(s);
		} else {
			vector<string>::const_iterator begin =
					parsed_with_dots.at(i).begin();
			vector<string>::const_iterator last =
					parsed_with_dots.at(i).begin()
							+ parsed_with_dots.at(i).size();
			vector<string> new_arr(begin + 1, last);
			vector<string> postfixed = toPostFix(new_arr);
			temp.push_back(parsed_with_dots.at(i).at(0));
			for (int z = 0; z < postfixed.size(); z++) {
				temp.push_back(postfixed.at(z));
			}
		}
		postfix.push_back(temp);
	}
	remove_spaces(&postfix);
	seperate(&postfix);
	remove_hash(&postfix);
	addAssignmant(&postfix);
	repairPunc(&postfix);
	vector<vector<string> >* toReturn = &postfix;
	return toReturn;
}

void repairPunc(vector<vector<string> > *v) {
	bool x = true;
	int index = -1;
	vector<string> temp_vector;
	for (int i = 0; i < v->size(); i++) {
		if (v->at(i).at(0) == "punctuations") {
			index = i;
			temp_vector.push_back("punctuations");
			for (int j = 1; j < v->at(i).size(); j++) {
				if (j + 1 < v->at(i).size() && v->at(i).at(j) == "|"
						&& v->at(i).at(j + 1) == "|") {
					temp_vector.push_back(v->at(i).at(j));
					if (x) {
						temp_vector.push_back("(");
						x = !x;
					} else {
						temp_vector.push_back(")");
						x = !x;
					}

				} else {
					temp_vector.push_back(v->at(i).at(j));
				}
			}
		}

	}
	if (index != -1)
		v->at(index) = temp_vector;
}

void expand_keywords_and_punctuations() {
	vector<vector<string> > temporary;
	for (int i = 0; i < parsed.size(); i++) {
		if (parsed[i][0] != "punctuations" && parsed[i][0] != "reserved") {
			temporary.push_back(parsed[i]);
		} else {

			for (int z = 1; z < parsed[i].size(); z += 2) {
				vector<string> temp;
				temp.push_back(parsed[i][z]);
				temp.push_back(parsed[i][z]);
				temporary.push_back(temp);
			}
		}
	}
	parsed = temporary;
}

bool isReserved(string st) {
	for (int i = 0; i < reserved.size(); i++) {
		if (reserved[i] == st)
			return true;
	}
	return false;
}

bool isPunctuation(string st) {
	for (int i = 0; i < punctuations.size(); i++) {
		if (punctuations[i] == st) {
			//			cout << "punc = " << st << endl;
			return true;
		}
	}
	return false;
}

void add_dots_to_parsed_vector() {
	for (int i = 0; i < parsed.size(); i++) {
		vector<string> temp2;
		vector<string> temp;

		if (isReserved(parsed[i][0])) {
			stringstream ss;
			string s;
			char c;
			temp.push_back(parsed[i][0]);
			for (int j = 0; j < parsed[i][0].size() - 1; j++) {
				c = parsed[i][0][j];
				ss << c;
				ss >> s;
				temp.push_back(s);
				temp.push_back(".");
				ss.clear();
			}
			c = parsed[i][0][parsed[i][0].size() - 1];
			ss << c;
			ss >> s;
			temp.push_back(s);
			parsed_with_dots.push_back(temp);
			continue;
		}

		vector<string>::const_iterator begin = parsed.at(i).begin();
		vector<string>::const_iterator last = parsed.at(i).begin() + parsed.at(
				i).size();
		vector<string> new_arr(begin + 1, last);

		temp = add_dots(&new_arr);

		temp2.push_back(parsed[i][0]);
		for (int j = 0; j < temp.size(); j++) {
			temp2.push_back(temp[j]);
		}
		parsed_with_dots.push_back(temp2);
	}
}

void remove_hash(vector<vector<string> > *vect) {
	for (int i = 0; i < vect->size(); i++) {
		for (int j = 0; j < vect->at(i).size(); j++) {
			if (vect->at(i).at(j) == "#") {
				vect->at(i).at(j) = "\\.";
			}
		}
	}
}

bool isAlphabetic(string st) {
	if (st == "")
		return false;
	for (int i = 0; i < st.length(); ++i) {
		stringstream ss;
		ss << st[i];
		string tempString;
		ss >> tempString;
		if ((st[i] >= 'a' && st[i] <= 'z') || (st[i] >= 'A' && st[i] <= 'Z')
				|| (st[i] >= '0' && st[i] <= '9') || st[i] == '=' || st[i]
				== '<' || st[i] == '>' || st[i] == '+' || st[i] == '-' || st[i]
				== '*' || st[i] == '/' || isPunctuation(tempString)) {
			return true;
		}
	}
	return false;
}

void seperate(vector<vector<string> > *vect) {
	//
	//	cout << "parsed before seperate:" << endl;
	//	print_vector_2d(&parsed);
	//	cout << "\n\n";
	//
	//	cout << "postfix before seperate:" << endl;
	//	print_vector_2d(&postfix);
	//	cout << "\n\n";

	for (int i = 0; i < vect->size(); i++) {
		vector<string> temp_vector;
		for (int j = 0; j < vect->at(i).size(); j++) {

			if (j + 1 < vect->at(i).size() && vect->at(i).at(j) == "\\"
					&& vect->at(i).at(j + 1) == "L") {
				temp_vector.push_back("\\L");
				j += 2;
			} else if (j - 1 >= 0 && vect->at(i).at(j).length() == 1
					&& vect->at(i).at(j)[0] == '+' && vect->at(i).at(j - 1)
					!= "|") {
				temp_vector.push_back("\\+");
			} else if (j - 1 >= 0 && vect->at(i).at(j).length() == 1
					&& vect->at(i).at(j)[0] == '*' && vect->at(i).at(j - 1)
					!= "|") {
				temp_vector.push_back("\\*");
			} else if (j - 1 >= 0 && vect->at(i).at(j).length() == 1
					&& !isAlphabetic(vect->at(i).at(j - 1))
					&& vect->at(i).at(j)[0] == '|' && vect->at(i).at(j - 1)
					!= "|" && vect->at(i).at(j - 1) != ")" && vect->at(i).at(j
					- 1) != "(" && vect->at(i).at(j - 1) != ".") {
				temp_vector.push_back("\\|");
			} else if ((vect->at(i).at(j)[vect->at(i).at(j).length() - 1]
					== '+' || vect->at(i).at(j)[vect->at(i).at(j).length() - 1]
					== '*') && vect->at(i).at(j).length() > 1) {
				string st = "";
				for (int k = 0; k < vect->at(i).at(j).length() - 1; k++) {
					st += vect->at(i).at(j)[k];
				}
				temp_vector.push_back(st);
				stringstream ss;
				char c = vect->at(i).at(j)[vect->at(i).at(j).length() - 1];
				ss << c;
				string tempString;
				ss >> tempString;
				temp_vector.push_back(tempString);
			} else {
				temp_vector.push_back(vect->at(i).at(j));
			}
		}
		vect->at(i) = temp_vector;
	}
	//	cout << "parsed after seperate:" << endl;
	//	print_vector_2d(&parsed);
	//	cout << "\n\n";
	//
	//	cout << "postfix after seperate:" << endl;
	//	print_vector_2d(&postfix);
	//	cout << "\n\n";
	removeSlash(&postfix);
}

void removeSlash(vector<vector<string> > *vect) {
	for (int i = 0; i < vect->size(); i++) {
		vector<string> temp_vector;
		for (int j = 0; j < vect->at(i).size(); j++) {
			if (vect->at(i).at(j).length() == 1 && vect->at(i).at(j) == "\\")
				continue;
			temp_vector.push_back(vect->at(i).at(j));
		}
		vect->at(i) = temp_vector;
	}
}

//this method to add = or :
void addAssignmant(vector<vector<string> > *v) {
	for (int i = 0; i < v->size(); ++i) {
		string def = v->at(i).at(0);

		vector<string> temp_vector;
		temp_vector.push_back(def);
		string assign;
		for (int j = 0; j < addAssignment.size(); j++) {
			if (addAssignment.at(j) == def) {
				assign = addAssignment.at(j + 1);
				break;
			}
		}
		if (def == "reserved" || def == "punctuations") {
			temp_vector.push_back(":");
		} else {
			temp_vector.push_back(assign);
		}
		for (int k = 1; k < v->at(i).size(); k++) {
			string st = v->at(i).at(k);
			if (st == "=")
				st = "\\=";
			else if (st == "-")
				st = "\\-";
			temp_vector.push_back(st);
		}
		v->at(i) = temp_vector;
	}
}
//
//int main() {
//	vector<vector<string> >* POSTFIX = get_Postfix();
//	cout << "parsed finally:" << endl;
//	print_vector_2d(&parsed);
//	cout << "\n\n";
//	cout << "Postfix vector:" << endl;
//	print_vector_2d(POSTFIX);
//	return 0;
//}
