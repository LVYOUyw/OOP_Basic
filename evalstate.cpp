/*
 * File: evalstate.cpp
 * -------------------
 * This file implements the EvalState class, which defines a symbol
 * table for keeping track of the value of identifiers.  The public
 * methods are simple enough that they need no individual documentation.
 */

#include <string>
#include "evalstate.h"

#include "../StanfordCPPLib/map.h"
using namespace std;
const string ban[12] =  {"REM", "LET", "PRINT", "END", "GOTO",
						 "IF", "THEN", "RUN", "LIST", "CLEAR",
						 "QUIT", "HELP" };

/* Implementation of the EvalState class */

EvalState::EvalState() {
   /* Empty */
}

EvalState::~EvalState() {
   /* Empty */
}

bool check(string &var) {
    for (int i = 0; i < 12; i++)
        if (var == ban[i]) return 0;
    return (var[0] < '0' || var[0] > '9' );
}

void EvalState::setValue(string var, int value) {
   if (check(var)) symbolTable.put(var, value);
   else error("SYNTAX ERROR");
}

int EvalState::getValue(string var) {
   return symbolTable.get(var);
}

bool EvalState::isDefined(string var) {
   return symbolTable.containsKey(var);
}

void EvalState::clear()
{
	symbolTable.clear();
}
