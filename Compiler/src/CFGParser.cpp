#include "CFGParser.h"

map<Rule*, vector<vector<Rule*> *>*>* grammar_T=new map<Rule*, vector<vector<Rule*> *>*>;
vector<Rule*>* terminals_T=new vector<Rule*>;
vector<Rule*>* nonTerminals_T=new vector<Rule*>;
vector<Rule*>* terminals_Distinct=new vector<Rule*>;
vector<string> input_File;



map<Rule*, vector<vector<Rule*> *>*>*  getGrammarMap()
{
    return grammar_T;
}


vector<Rule*>*  getNonTerminals()
{
    return nonTerminals_T;
}


vector<Rule*>*  getTerminals()
{
    return terminals_Distinct;
}



void parse()
{
    readFromFile2("CFG.txt");
    getGrammar();
    removeDuplicatesOfTerminals();
}


void printTheMap()
{
    cout<<"=========================MAP========================="<<endl;
    cout<<"Size of map = "<<grammar_T->size()<<endl;
    for (std::map<Rule*, vector<vector<Rule*>* >* >::iterator it=grammar_T->begin(); it!=grammar_T->end(); ++it)
    {
        Rule *key = it->first;
        int lhs_type = key->type;
        string str_key = key->name;
        cout << "Key: " << str_key << "," << lhs_type <<endl;
        for (unsigned i=0; i< it->second->size(); i++)
        {
            for (unsigned j=0; j< it->second->at(i)->size(); j++)
            {
                cout << "Value: " << it->second->at(i)->at(j)->name << "," << it->second->at(i)->at(j)->type <<"    ";
            }
            cout <<endl;
        }
        cout <<"-----------------------------"<<endl;
    }
    cout<<"=================================================="<<endl;
}




void repairMapTypes()
{
    for (std::map<Rule*, vector<vector<Rule*>* >* >::iterator it=grammar_T->begin(); it!=grammar_T->end(); ++it)
    {
        Rule *key = it->first;
        int lhs_type = key->type;
        string str_key = key->name;
        for (unsigned i=0; i< it->second->size(); i++)
        {
            for (unsigned j=0; j< it->second->at(i)->size(); j++)
            {
                string valueName=it->second->at(i)->at(j)->name;
                if(valueName.length()>=4&&valueName[0]=='\''&&valueName[1]=='\\'&&valueName[2]=='L'&&valueName[3]=='\''){
                    it->second->at(i)->at(j)->type=2;
                    valueName=valueName.substr(1,valueName.length()-2);
                    it->second->at(i)->at(j)->name=valueName;
                    continue;
                }
                if(valueName[0]=='\'')
                {
                    it->second->at(i)->at(j)->type=1;
                    valueName=valueName.substr(1,valueName.length()-2);
                    it->second->at(i)->at(j)->name=valueName;
                    Rule* r=new Rule(valueName,1);
                    terminals_T->push_back(r);
                }
                else if(valueName.length()>1&&valueName[0]=='\\' &&valueName[1]=='L')
                {
                    it->second->at(i)->at(j)->type=2;
                }
                else
                {
                    it->second->at(i)->at(j)->type=0;
                }
            }
        }
    }
}


void getGrammar()
{
    for (int i = 0; i < input_File.size(); i++)
    {
        string g = input_File[i];
        int j = 0;
        while (j < g.length() && (g[j] == '#' || g[j] == ' ' || g[j] == '\t'))
        {
            j++;
            continue;
        }
        stringstream ss;
        string temp;
        while (j < g.length() && g[j] != '=')
        {
            ss << g[j++];
        }
        ss >> temp;
        Rule* r1= new Rule(temp, 0);
        ss.clear();
        while (j < g.length()
                && (g[j] == '=' || g[j] == ':' || g[j] == ' ' || g[j] == '\t'))
        {
            j++;
            continue;
        }
        vector<string> splitOR = split(g.substr(j, g.length() - 1), '|');
        vector<vector<Rule*>* >* tempV=new vector<vector<Rule*>* >;
        for (int k = 0; k < splitOR.size(); ++k)
        {
            vector<Rule*>* rulus_vector=new vector<Rule*>;
            string str = splitOR[k];
            vector<string> split_Space = split(str, ' ');
            for (int d = 0; d < split_Space.size(); d++)
            {
                string tempStr = split_Space.at(d);
                if (tempStr.length() == 0 || tempStr == "")
                    continue;

                Rule* r=new Rule(tempStr, 0);
                rulus_vector->push_back(r);
            }
            tempV->push_back(rulus_vector);
        }
        r1->type=0;
        nonTerminals_T->push_back(r1);
        grammar_T->insert(make_pair<Rule*, vector< vector<Rule *>* > *>(r1, tempV));
        cout<<"Saved"<<endl;
    }
    repairMapTypes();
}

void readFromFile2(char*pathOfFile)
{
    ifstream in_stream;
    string line;
    in_stream.open(pathOfFile);
    while (std::getline(in_stream, line))
    {
        string tempLine="";
        for(int i=0;i<line.length();i++){
            tempLine+=line[i];
            if(i+1<line.length() && line[i]=='\'' && line[i+1]=='\''){
                tempLine+=" ";
            }
        }
        cout << "line=" << tempLine << endl;
        if (line.size() == 0)
            continue;
        parseLine2(tempLine);
    }
    in_stream.close();
}

void parseLine2(string line)
{
    if (line[0] == '#')
    {
        input_File.push_back(line);
    }
    else if (line[0] == '|')
    {
        string temp = input_File.at(input_File.size() - 1) + line;
        input_File.pop_back();
        input_File.push_back(temp);
    }
}

void print_vector_22d(vector<vector<string> > *v2d)
{
    cout << "___________________________________" << endl;
    for (int i = 0; i < v2d->size(); i++)
    {
        for (int j = 0; j < v2d->at(i).size(); j++)
        {
            cout << v2d->at(i).at(j) << " ";
        }
        cout << endl;
    }
}

void print_vector_22d_Rule(vector<vector<Rule> > *v2d)
{
    cout << "___________________________________" << endl;
    for (int i = 0; i < v2d->size(); i++)
    {
        for (int j = 0; j < v2d->at(i).size(); j++)
        {
            Rule r = v2d->at(i).at(j);
            cout << "|name = |" << r.name << "| ";
            cout << "type = |" << r.type << "| " << endl;
        }
        cout << endl;
    }
}


bool isPushed(string name)
{
    for(int i=0;i<terminals_Distinct->size();i++)
    {
        if(terminals_Distinct->at(i)->name == name)
        {
            return true;
        }
    }
    return false;
}

void removeDuplicatesOfTerminals()
{

    for(int i=0;i<terminals_T->size();i++)
    {
        if(!isPushed(terminals_T->at(i)->name))
        {
            terminals_Distinct->push_back(terminals_T->at(i));
        }
    }
}


void print_vector_11d_Rule(vector<Rule*> *v1d)
{
    cout << "___________________________________" << endl;
    for (int i = 0; i < v1d->size(); i++)
    {
            Rule* r = v1d->at(i);
            cout << "|name = |" << r->name << "| ";
            cout << "type = |" << r->type << "| " << endl;
    }
}





void print_vector_11d(vector<string> *v1d)
{
    cout << "___________________________________" << endl;
    for (int j = 0; j < v1d->size(); j++)
    {
        cout << "|" << v1d->at(j) << "| " << endl;
    }
    cout << endl;
}

vector<string>& split(const string &s, char delim, vector<string> &elems)
{
    stringstream ss(s);
    string item;
    while (std::getline(ss, item, delim))
    {
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim)
{
    vector<string> elems;
    split(s, delim, elems);
    return elems;
}
