#ifndef STORAGE_H
#define STORAGE_H

#include <fstream>
#include <sstream>
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
#include "DFA.h"

using namespace std;

void storeGraph ();

vector<DFA*>* getStorageGraph ();

#endif // STORAGE_H
