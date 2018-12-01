/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"
using namespace std;
typedef map<int, string>::iterator IT;

Program::Program()
{
    NumToLine.clear();

}

Program::~Program()
{
    NumToLine.clear();
}

void Program::clear()
{
    NumToLine.clear();
}

void Program::addSourceLine(int lineNumber, string line)
{
    NumToLine[lineNumber] = line;
}

void Program::removeSourceLine(int lineNumber)
{
    IT it = NumToLine.find(lineNumber);
    if (it == NumToLine.end()) return;
    NumToLine.erase(it);
}

string Program::getSourceLine(int lineNumber)
{
    IT it = NumToLine.find(lineNumber);
    if (it == NumToLine.end()) return "";
    return (it->second);
}

int Program::getFirstLineNumber()
{
    if (NumToLine.begin() == NumToLine.end()) return -1;
    return NumToLine.begin() -> first;
}

int Program::getNextLineNumber(int lineNumber)
{
    IT it = NumToLine.upper_bound(lineNumber);
    if (it == NumToLine.end()) return -1;
    return it -> first;
}

void Program::list()
{
    int t = Program::getFirstLineNumber();
    while (t != -1)
    {
        cout << Program::getSourceLine(t) << endl;
        t = Program::getNextLineNumber(t);
    }
}
