/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"
using namespace std;

/* Implementation of the Statement class */
StatementType get_type(string &line)
{
    if (line == "RUN" || line == "LIST" || line == "CLEAR" || line == "QUIT" || line == "HELP") return COMMAND;
    if (line[0] >= '0' && line[0] <= '9')
    {
        if (line.find("GO") != line.npos || line.find("IF") != line.npos) return CONTROL;
        else return SEQUENTIAL;
    }
    return DIRECTLY_EXECUTED;
}

Statement::Statement(string &_line) : line(_line), linenumber(0)
{
   /* Empty */
}

Statement::~Statement()
{
   linenumber = 0;
}

SequentialStatement::SequentialStatement(string &_line) : Statement(_line), quit(false), exp(nullptr)
{
    if (line.find("REM") == line.npos && line.find("LET") == line.npos && line.find("PRINT") == line.npos && line.find("INPUT") == line.npos && line.find("END") == line.npos) error("SYNTAX ERROR");
}

SequentialStatement::~SequentialStatement()
{
    if (exp != nullptr) delete exp;
}

int SequentialStatement::execute(EvalState &state)
{
    if (line.find("REM") != line.npos) return 0; 
    else if (line.find("END") != line.npos)
    {
        quit = true;
        return 0;
    }
    else
    {
        size_t len = line.length();
        size_t pos;
        string trans = "";
        bool flag = false;
        if (line.find("INPUT") != line.npos)
        {
            pos = line.find("INPUT");
            string val = "";
            while (true)
            {
            
				bool jud = true;
                cout << " ? " ;
                char ch = getchar();
                while (ch != '\n')
                {
                    val += ch;
					if (ch > '9' || ch < '0') jud = false;
                    ch = getchar();
                }
				if (jud) break;
				val = "";
				cout << "INVALID NUMBER" << endl;
            }
			stringToInteger(val);
            trans = line.substr(pos + 6, len - pos - 6) + "=" + val;
        }
        else if (line.find("PRINT") != line.npos)
        {
            flag = 1;
            pos = line.find("PRINT");
            trans = line.substr(pos + 6, len - pos - 6);
        }
        else if (line.find("LET") != line.npos)
        {
            pos = line.find("LET");
            trans = line.substr(pos + 4, len - pos -4);
        }
        TokenScanner scanner;
        scanner.ignoreWhitespace();
        scanner.setInput(trans);
        exp = parseExp(scanner);
        if (flag) cout << exp -> eval(state) << endl;
        else exp -> eval(state);
    }
    return 0;
}

DirectlyExecutedStatement::DirectlyExecutedStatement(string &_line) : Statement(_line), exp(nullptr)
{
    if (line.find("LET") == line.npos && line.find("INPUT") == line.npos && line.find("PRINT") == line.npos)  error("SYNTAX ERROR");
}

DirectlyExecutedStatement::~DirectlyExecutedStatement()
{
    if (exp != nullptr) delete exp;
}

int DirectlyExecutedStatement::execute(EvalState &state)
{
    bool flag = false;
	size_t pos = 0;
	string trans = "";
	size_t len = line.length();
    if (line.find("INPUT") != line.npos)
    {
        pos = line.find("INPUT");
        string val = "";
		while (true)
		{

			bool jud = true;
			cout << " ? " ;
			char ch = getchar();
			while (ch != '\n')
			{
				val += ch;
				if (ch > '9' || ch < '0') jud = false;
				ch = getchar();
			}
			if (jud) break;
			val = "";
			cout << "INVALID NUMBER" << endl;
		}
		stringToInteger(val);
        trans = line.substr(pos + 6, len - pos - 6) + "=" + val;
    }
    else if (line.find("PRINT") != line.npos)
    {
        flag = 1;
        pos = line.find("PRINT");
        trans = line.substr(pos + 6, len - pos - 6);
    }
    else if (line.find("LET") != line.npos)
    {
        pos = line.find("LET");
        trans = line.substr(pos + 4, len - pos -4);
    }
    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.setInput(trans);
    exp = parseExp(scanner);
    if (flag) cout << exp -> eval(state) << endl;
    else exp -> eval(state);
    return 0;
}

ControlStatement::ControlStatement(string &_line) : Statement(_line), err(0), exp1(nullptr), exp2(nullptr)
{
    if (line.find("GOTO") != line.npos)
    {
        size_t pos = line.find("GOTO") + 5;
        size_t len = line.length();
		for (auto i = pos; i < len; i++)
		{
			if (line[i] > '9' || line[i] < '0') err = 1;
			linenumber = linenumber * 10 + line[i] - '0';
		}
    }
    else
    {
        size_t pos = line.find("THEN") + 5;
        size_t len = line.length();
		for (auto i = pos; i < len; i++)
		{
			if (line[i] > '9' || line[i] < '0') err = 1;
			linenumber = linenumber * 10 + line[i] - '0';
		}
    }
}

ControlStatement::~ControlStatement()
{
    if (exp1 != nullptr) delete exp1;
    if (exp2 != nullptr) delete exp2;
}

int ControlStatement::execute(EvalState &state)
{
    int opt = 0;
    size_t pos = 0;
	if (err) error("SYNTAX ERROR");
    if (line.find("GOTO") != line.npos) return linenumber;
    if (line.find("<") != line.npos) opt = 1, pos = line.find("<");
    else if (line.find("=") != line.npos) opt = 2, pos = line.find("=");
    else if (line.find(">") != line.npos) opt = 3, pos = line.find(">");
    size_t pt = line.find("IF") + 3;
    size_t pr = line.find("THEN") - 1;
    string t1 = line.substr(pt, pos - 2 - pt + 1);
    string t2 = line.substr(pos + 2, pr - pos - 2 + 1);
    TokenScanner scanner1, scanner2;
    scanner1.ignoreWhitespace();
    scanner2.ignoreWhitespace();
    scanner1.setInput(t1);
    scanner2.setInput(t2);
    exp1 = parseExp(scanner1);
    exp2 = parseExp(scanner2);
    int a1 = exp1 -> eval(state);
    int a2 = exp2 -> eval(state);
    if (a1 < a2 && opt == 1) return linenumber;
    else if (a1 == a2 && opt == 2) return linenumber;
    else if (a1 > a2 && opt == 3) return linenumber;
    return -1;
}
