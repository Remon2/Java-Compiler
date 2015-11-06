/*
 * ParsingTableGeneratorAndMatching.cpp
 *
 *  Created on: Apr 11, 2015
 *      Author: Shaimaa Mousa
 */

#include "ParsingTableGeneratorAndMatching.h"
#include "CFGParser.h"
#include "table_printer.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "Matching.h"

#if defined(USE_BOOST_KARMA)
#include <boost/spirit/include/karma.hpp>
namespace karma = boost::spirit::karma;
#endif

using bprinter::TablePrinter;
#include <fstream>
ofstream leftDrivation("leftDrivation.txt");
ofstream firstSets("first.txt");
ofstream followSets("follow.txt");
ofstream table("parsing_table.txt");

map<Rule*, vector<Rule*>*>* follow = new map<Rule*, vector<Rule*>*>;
map<Rule*, vector<Rule*>*>* first = new map<Rule*, vector<Rule*>*>;
map<Rule*, vector<vector<Rule*>*>*> * productionFirst = new map<Rule*,
vector<vector<Rule*>*>*>;
vector<ParserTableRow*>* parsingTable = new vector<ParserTableRow*>;
vector<Rule*>* terminals = new vector<Rule*>;
vector<Rule*>* nonTerminals = new vector<Rule*>;
map<Rule*, vector<vector<Rule*> *>*>* grammar = new map<Rule*,
vector<vector<Rule*> *>*>;
vector<string>* inputTokens = new vector<string>;
bool found;

vector<Rule*>* getTheValue(map<Rule*, vector<Rule*>*>* m, Rule* rule)
{
    for (std::map<Rule*, vector<Rule*>*>::iterator it = m->begin();
            it != m->end(); ++it)
    {
        found = false;
        Rule *key = it->first;
        string keyName = key->name;
        if (rule->name == keyName)
        {
            found = true;
            return it->second;

        }

    }
    return NULL;

}

void printTheMap2(map<Rule*, vector<Rule*>*>* m)
{
//    cout<<"=========================MAP========================="<<endl;
//    cout<<"Size of map = "<<m->size()<<endl;
    for (std::map<Rule*, vector<Rule*>*>::iterator it = m->begin();
            it != m->end(); ++it)
    {
        Rule *key = it->first;
        int lhs_type = key->type;
        string str_key = key->name;
        if (m == first)
            firstSets << "Key: " << str_key << "," << lhs_type << endl;
        if (m == follow)
            followSets << "Key: " << str_key << "," << lhs_type << endl;
//        cout << "Key: " << str_key << "," << lhs_type <<endl;

        /** Check Existance of Left Recursion. */

        for (unsigned j = 0; j < it->second->size(); j++)
        {
//            cout << "Value: " << it->second->at(j)->name << "," << it->second->at(j)->type <<"    ";
            if (m == first)
                firstSets << "Value: " << it->second->at(j)->name << ","
                          << it->second->at(j)->type << "    ";
            if (m == follow)
                followSets << "Value: " << it->second->at(j)->name << ","
                           << it->second->at(j)->type << "    ";
        }
        if (m == first)
        {
            firstSets << endl;
            firstSets
                    << "---------------------------------------------------------------------"
                    << endl;

        }
        if (m == follow)
        {
            followSets << endl;
            followSets
                    << "---------------------------------------------------------------------"
                    << endl;

        }
//        cout <<endl;
//        cout <<"---------------------------------------------------------------------"<<endl;
    }
//    cout<<"=================================================="<<endl;
}

void getFirst()
{
    bool recur = true ;
    terminals = getTerminals();
    nonTerminals = getNonTerminals();
    vector <bool> recurOrNot (nonTerminals->size(), false);
    grammar = getGrammarMap();
    while (recur)
    {
        recur = false;
        for (int i = nonTerminals->size() - 1; i >= 0; i--)
        {

            if(recurOrNot.at(i) == false)
            {
                recurOrNot.at(i) = true;
                Rule * nonTerm = nonTerminals->at(i);
                vector<Rule*>* expectedFirst = new vector<Rule*>;
                vector<vector<Rule*>*>* expectedProd = new vector<vector<Rule*>*>;
                vector<vector<Rule*>*>* termProduct = new vector<vector<Rule*>*>;
                termProduct = grammar->at(nonTerm);
                for (unsigned int j = 0; j < termProduct->size(); j++)
                {
                    bool eps = false;
                    vector<Rule*>* termSingleProduct = termProduct->at(j);
                    for (unsigned int k = 0; k < termSingleProduct->size(); k++)
                    {
                        eps = false;
                        Rule * temp = termSingleProduct->at(k);
                        if (temp->type == 1)
                        {
                            expectedFirst->push_back(temp);
                            expectedProd->push_back(termSingleProduct);
                            break;
                        }
                        else if (temp->type == 2)
                        {
                            Rule * epsilon = new Rule("\L", 2);
                            expectedFirst->push_back(epsilon);
                            expectedProd->push_back(termSingleProduct);
                            break;
                        }
                        else if (temp->type == 0)
                        {
                            vector<Rule*>* firstTerm = new vector<Rule*>;
                            firstTerm = getTheValue(first, temp);
                            if (firstTerm == NULL)
                            {
                                recur = true;
                                recurOrNot.at(i) = false;
                            }
                            else
                            {
                                for (unsigned int v = 0; v < firstTerm->size(); v++)
                                {
                                    Rule * firstTermRule = firstTerm->at(v);
                                    if (firstTermRule->type != 2)
                                    {
                                        expectedFirst->push_back(firstTermRule);
                                        expectedProd->push_back(termSingleProduct);
                                    }
                                    else
                                    {
                                        eps = true;
                                    }
                                }
                                if (!eps)
                                    break;
                            }
                        }
                    }
                    if (eps)
                    {
                        Rule * epsilon = new Rule("\L", 2);
                        expectedFirst->push_back(epsilon);
                        expectedProd->push_back(termSingleProduct);
                    }

                }
                if (recurOrNot.at(i) == true)
                {
                    first->insert(pair<Rule*, vector<Rule*>*>(nonTerm, expectedFirst));
                    productionFirst->insert(
                        pair<Rule*, vector<vector<Rule*>*>*>(nonTerm, expectedProd));
                }
                //printTheMap2(productionFirst);
            }
        }
    }
    printTheMap2(first);

}
bool exist(vector<Rule*>* expectedFollow, Rule *r)
{
    for (unsigned int i = 0; i < expectedFollow->size(); ++i)
    {

        if (expectedFollow->at(i)->name == r->name
                || expectedFollow->at(i)->type == r->type && r->type == 2)
            return true;
    }
    return false;

}

void getFollow()
{
    bool recur = true ;
    vector <bool> recurOrNot (nonTerminals->size(), false);
//    while (recur)
//    {
        recur = false;
        for (unsigned int i = 0; i < nonTerminals->size(); ++i)
        {
        if(recurOrNot.at(i) == false)
            {
                recurOrNot.at(i) = true;
                Rule* nonTerm = nonTerminals->at(i);
                vector<Rule*>* expectedFollow = new vector<Rule*>;
                Rule * r = new Rule("$", 3);
                if (i == 0)
                    expectedFollow->push_back(r);
                //iterate over the whole map
                for (map<Rule*, vector<vector<Rule*>*>*>::iterator it =
                            grammar->begin(); it != grammar->end(); ++it)
                {
                    vector<vector<Rule*>*>* nonTermProd;
                    nonTermProd = it->second;
                    for (unsigned int j = 0; j < nonTermProd->size(); ++j)
                    {
                        vector<Rule*>* nonTermSingleProd = nonTermProd->at(j);
                        bool eps = false;
                        for (unsigned int k = 0; k < nonTermSingleProd->size(); ++k)
                        {
                            Rule* rule = nonTermSingleProd->at(k);
                            if (rule->name == nonTerm->name || eps == true)
                            {
                                eps = false;
                                //check for the next rule whether it's a terminal
                                if (k + 1 < nonTermSingleProd->size())
                                {
                                    Rule * next = nonTermSingleProd->at(k + 1);
                                    if (next->type == 1)
                                    {
                                        if (!exist(expectedFollow, next))
                                            expectedFollow->push_back(next);
                                        break;
                                    }
                                    else if (next->type == 0)
                                    {
                                        vector<Rule*>* firstTerm;
                                        firstTerm = getTheValue(first, next);

                                        for (unsigned int v = 0; v < firstTerm->size();
                                                ++v)
                                        {
                                            Rule * firstTermRule = firstTerm->at(v);
                                            if (firstTermRule->type != 2)
                                            {
                                                if (!exist(expectedFollow,
                                                           firstTermRule))
                                                    expectedFollow->push_back(
                                                        firstTermRule);
                                            }
                                            else
                                            {
                                                eps = true;
                                            }
                                        }
                                        if (!eps)
                                            break;

                                    }
                                    //now we add the follow of the main non terminal which is
                                }
                                else if (k + 1 >= nonTermSingleProd->size())
                                {
                                    Rule * nonTerminal = it->first;
                                    if (nonTerminal->name != rule->name)
                                    {
                                        vector<Rule*>* folowTerm;
                                        folowTerm = getTheValue(follow, nonTerminal);
                                        if (folowTerm != NULL)
                                        {
//                                            recur = true;
//                                            recurOrNot.at(i) = false;
//                                            cout << " ***********************size " << endl;
//                                        }
//                                        else
//                                        {
                                            for (unsigned int h = 0; h < folowTerm->size();
                                                    h++)
                                            {
                                                Rule * followTermRule = folowTerm->at(h);
                                                if (!exist(expectedFollow, followTermRule))
                                                    expectedFollow->push_back(
                                                        followTermRule);
                                            }

                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                if (recurOrNot.at(i) == true)
                {
                    follow->insert(pair<Rule*, vector<Rule*>*>(nonTerm, expectedFollow));
                }
//        cout<<"******************row "<<i<<endl;
            }
        }
  //  }
    printTheMap2(follow);
}
int getIndex(vector<Rule*>* nonFirst, Rule * r)
{
    for (int i = 0; i < nonFirst->size(); ++i)
    {
        Rule * term = nonFirst->at(i);
        if (r->name == term->name || (r->type == 2 && term->type == 2))
            return i;
    }
    return -1;
}

int getIndex2(vector<ParserTableRow*>* paringTable, Rule * r)
{
    for (unsigned int i = 0; i < paringTable->size(); ++i)
    {
        ParserTableRow * row = paringTable->at(i);
        if (r->name == row->name)
            return i;
    }
    return -1;
}

void printParsingTable()
{
    TablePrinter tp(&table);
    tp.AddColumn("Non Terminals", 18);
    for (int i = 0; i < terminals->size(); i++)
    {
        tp.AddColumn(terminals->at(i)->name, 20);
    }
    tp.AddColumn("$", 20);
    tp.PrintHeader();
    for (int i = 0; i < nonTerminals->size(); i++)
    {
        tp << nonTerminals->at(i)->name;
        int index = getIndex2(parsingTable, nonTerminals->at(i));
        if (index != -1)
        {
            map<Rule*, vector<Rule*>*>* tableProd = new map<Rule*,
            vector<Rule*>*>;
            ParserTableRow * row = parsingTable->at(index);
            tableProd = row->tableProduction;
            for (int j = 0; j < terminals->size(); j++)
            {
                vector<Rule*>* inputProd = getTheValue(tableProd,
                                                       terminals->at(j));
                if (found)
                {
                    found = false;

                    string ss = "";
                    for (int k = 0; k < inputProd->size(); k++)
                    {
                        ss = ss + inputProd->at(k)->name + " ";
                    }
                    if (ss.length() > 20)
                    {
                        ss = ss.substr(0, 17);
                        ss = ss + "..";
                    }
                    tp << ss;
                }
                else
                    tp << "  ";
            }
            Rule * r = new Rule("$",3);
            vector<Rule*>* inputProd = getTheValue(tableProd,r);
                if (found)
                {
                    found = false;

                    string ss = "";
                    for (int k = 0; k < inputProd->size(); k++)
                    {
                        ss = ss + inputProd->at(k)->name + " ";
                    }
                    if (ss.length() > 20)
                    {
                        ss = ss.substr(0, 17);
                        ss = ss + "..";
                    }
                    tp << ss;
                }
                else
                    tp << "  ";

        }

    }
    tp.PrintFooter();
}

void generateParsingTable()
{

    for (unsigned int i = 0; i < nonTerminals->size(); ++i)
    {
        Rule * non = nonTerminals->at(i);
        vector<Rule*>* nonFirst = first->at(non);
        vector<vector<Rule*>*>* nonProd = new vector<vector<Rule*>*>;
        nonProd = productionFirst->at(non);
        ParserTableRow * row = new ParserTableRow(non->name);
        vector<Rule*>* nonFollow = follow->at(non);
        for (unsigned int j = 0; j < terminals->size(); ++j)
        {
            Rule * term = terminals->at(j);
            if (term->type != 2)
            {
                int index = getIndex(nonFirst, term);
                if (index != -1)
                {
                    vector<Rule*>* prod = nonProd->at(index);
                    if (row->tableProduction->find(term)
                            != row->tableProduction->end())
                    {
                        cout << "Ambigous Grammar NOT LL(1) Grammar" << endl;
                    }
                    else
                    {
                        row->tableProduction->insert(
                            pair<Rule*, vector<Rule*>*>(term, prod));
                    }
                }
            }
        }
        Rule * eps = new Rule("/L", 2);
        int epsIndex = getIndex(nonFirst, eps);
        //there is epsilon in the first
        if (epsIndex != -1)
        {

            for (unsigned int j = 0; j < nonFollow->size(); ++j)
            {
                Rule * term = nonFollow->at(j);
                if (row->tableProduction->find(term)
                        != row->tableProduction->end())
                {
                    cout << "Ambigous Grammar NOT LL(1) Grammar" << endl;

                }
                else
                {
                    vector<Rule*>* prod = new vector<Rule*>;
                    prod->push_back(eps);
                    row->tableProduction->insert(
                        pair<Rule*, vector<Rule*>*>(term, prod));
                }
            }
            //there is no epsilon in the first
        }
        else
        {
            Rule * eps = new Rule("synch", 4);
            for (unsigned int j = 0; j < nonFollow->size(); ++j)
            {
                Rule * term = nonFollow->at(j);
                getTheValue(row->tableProduction, term);
                if (found)
                {
                    found = false;
                }
                else
                {
                    vector<Rule*>* prod = new vector<Rule*>;
                    prod->push_back(eps);
                    row->tableProduction->insert(
                        pair<Rule*, vector<Rule*>*>(term, prod));
                }

            }
        }
        parsingTable->push_back(row);
    }
    printParsingTable();
}

int getIndexOfString(string s)
{
    for (int i = 0; i < s.length(); i++)
        if (s.at(i) == ' ')
            return i;
}
void match()
{
    stack<Rule*> * s = new stack<Rule*>;
    string in = "";
    Token t = getNextToken();
    if (t.identifier == "punctuations" || t.identifier == "reserved"
            || t.identifier == "assign")
    {
        in = t.lexeme;
    }
    else
    {
        in = t.identifier;
    }
    if (t.lexeme.length() == 0)
        in = "$";
    Rule * input = new Rule(in, 1);
    Rule * dollarSign = new Rule("$", 3);
    s->push(dollarSign);
    string drivRight = "";
    string drivLeft = "";
    vector<string>* drivation = new vector<string>;
    Rule * rule = nonTerminals->at(0);
    string ch = rule->name;
    drivRight = ch + " ";
    s->push(rule);
    string inputPointer = input->name;
    drivation->push_back(drivLeft + drivRight);
    while (!s->empty())
    {
        if (rule->type == 0)
        {
            int index = getIndex2(parsingTable, rule);
            if (index != -1)
            {
                map<Rule*, vector<Rule*>*>* tableProd = new map<Rule*,
                vector<Rule*>*>;
                ParserTableRow * row = parsingTable->at(index);
                tableProd = row->tableProduction;
                vector<Rule*>* inputProd = getTheValue(tableProd, input);
                if (found)
                {
                    found = false;
                    Rule * temp = s->top();
                    s->pop();
                    int ind = getIndexOfString(drivRight);
                    if (ind + 1 < drivRight.length())
                        drivRight = drivRight.substr(ind + 1);
                    else
                        drivRight = "";
                    string tem = "";
                    for (int i = inputProd->size() - 1; i >= 0; i--)
                    {
                        tem = inputProd->at(i)->name + " " + tem;
                        s->push(inputProd->at(i));
                    }
                    drivRight = tem + drivRight;
                    rule = s->top();
                    drivation->push_back(drivLeft + drivRight);

                }
                else
                {
                    cout << "Error illegal input discard " << input->name
                         << endl;
                    drivation->push_back(
                        "Error illegal input discard " + input->name);

                    t = getNextToken();
                    if (t.identifier == "punctuations"
                            || t.identifier == "reserved"
                            || t.identifier == "assign")
                    {
                        in = t.lexeme;
                    }
                    else
                    {
                        in = t.identifier;
                    }
                    if (t.lexeme.length() == 0)
                        in = "$";
                    input = new Rule(in, 1);
                    inputPointer = inputPointer + input->name;

                }

            }
            else
            {
                drivation->push_back("Error undefined symbol ");
                cout << "Error undefined symbol  " << endl;
            }

        }
        else if (rule->type == 1)
        {
            if (rule->name == input->name)
            {
                t = getNextToken();
                if (t.identifier == "punctuations" || t.identifier == "reserved"
                        || t.identifier == "assign")
                {
                    in = t.lexeme;
                }
                else
                {
                    in = t.identifier;
                }
                if (t.lexeme.length() == 0)
                    in = "$";
                input = new Rule(in, 1);
                drivLeft = drivLeft + rule->name + " ";
                inputPointer = inputPointer + input->name;
            }
            else
            {
                //error
                cout << "Error missing " << rule->name
                     << " discard the production" << endl;
                drivation->push_back(
                    "Error missing " + rule->name
                    + " discard the production");

            }

            s->pop();
            rule = s->top();
            int ind = getIndexOfString(drivRight);
            if (ind + 1 < drivRight.length())
                drivRight = drivRight.substr(ind + 1);
            else
                drivRight = "";

            drivation->push_back(drivLeft + drivRight);

        }
        else if (rule->type == 2)
        {

            s->pop();
            rule = s->top();
            int ind = getIndexOfString(drivRight);
            if (ind + 1 < drivRight.length())
                drivRight = drivRight.substr(ind + 1);
            else
                drivRight = "";
            drivation->push_back(drivLeft + drivRight);

        }
        else if (rule->type == 3)
        {
            if (rule->name == input->name)
            {
                //finished correct input
                cout << " correct input " << endl;
                drivation->push_back("correct input ");

            }
            else
            {
                //error
                cout << " wrong input " << endl;
                drivation->push_back("wrong input");

            }
            s->pop();
        }
        else if (rule->type == 4)
        {

            s->pop();
            rule = s->top();
            int ind = getIndexOfString(drivRight);
            if (ind + 1 < drivRight.length())
                drivRight = drivRight.substr(ind + 1);
            else
                drivRight = "";
            drivation->push_back("Error discard the production ");
            cout << "Error discard the production " << endl;
        }
    }

    for (int i = 0; i < drivation->size(); i++)
    {
        leftDrivation << drivation->at(i) << endl;
    }
}

//ParsingTableGeneratorAndMatching::ParsingTableGeneratorAndMatching() {
//	// TODO Auto-generated constructor stub
//
//}
//
//ParsingTableGeneratorAndMatching::~ParsingTableGeneratorAndMatching() {
//	// TODO Auto-generated destructor stub
//}

