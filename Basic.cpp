/*
 * File: Basic.cpp
 * ---------------
 * Name: [TODO: enter name here]
 * Section: [TODO: enter section leader here]
 * This file is the starter project for the BASIC interpreter from
 * Assignment #6.
 * [TODO: extend and correct the documentation]
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "statement.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"
using namespace std;
bool flag = 1;

/* Function prototypes */

void processLine(string line, Program & program, EvalState & state);
void Run(Program & program, EvalState & state);

/* Main program */

int main() {
   EvalState state;
   Program program;
   while (flag) {
      try {
         processLine(getLine(), program, state);
      } catch (ErrorException & ex) {
         cerr << "Error: " << ex.getMessage() << endl;
      }
   }
   return 0;
}

/*
  Function: processLine
  Usage: processLine(line, program, state);
  -----------------------------------------
  Processes a single line entered by the user.  In this version,
  the implementation does exactly what the interpreter program
  does in Chapter 19: read a line, parse it as an expression,
  and then print the result.  In your implementation, you will
  need to replace this method with one that can respond correctly
  when the user enters a program line (which begins with a number)
  or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program & program, EvalState & state)
{
   StatementType opt = get_type(line);
   if (opt == DIRECTLY_EXECUTED)
   {
       auto obj = DirectlyExecutedStatement(line);
       obj.execute(state);
	   return;
       //delete ptr;
   }
   if (opt == COMMAND)
   {
       if (line.find("QUIT") != line.npos) flag = 0;
       if (line.find("HELP") != line.npos) cout << "NO HELP" << endl;
       if (line.find("CLEAR") != line.npos) program.clear(), state.clear();
       if (line.find("RUN") != line.npos)  Run(program, state);
       if (line.find("LIST") != line.npos) program.list();
	   return;
   }
   int x = 0;
   size_t len = line.length();
   bool jud = 0;
   for (size_t i = 0; i < len  ; i++)
   {
       if (line[i]>'9' || line[i]<'0')
       {
           jud = 1;
           break;
       }
       x = x * 10 + line[i] - '0';
   }
   if (!jud) program.removeSourceLine(x);
   else program.addSourceLine(x, line);
}

void Run(Program & program, EvalState & state)
{
    int t = program.getFirstLineNumber();
    while (t != -1)
    {
        string source = program.getSourceLine(t);
		if (source == "") error("LINE NUMBER ERROR");
        StatementType opt = get_type(source);
        if (opt == SEQUENTIAL)
        {
            auto obj = SequentialStatement(source);
            obj.execute(state);
			if (obj.quit) return;
            t = program.getNextLineNumber(t);
        }
        else
        {
            auto obj = ControlStatement(source);
            int tmp = obj.execute(state);
            t = tmp == -1 ? program.getNextLineNumber(t) : tmp;
        }
    }
}
