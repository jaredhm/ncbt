#include "driver.hpp"
#include <iostream>
#include <iomanip>

bool Parser::run(){
  extern int yydebug;
  extern int yyparse(Parser *);
  extern FILE* yyin;

  //yydebug = 1;
  if(_target != 0)
    yyin = fopen(_target,"r");  //Read from file extension if it exists, else the input lives in stdin
  return yyparse(this) == 0;  //return whether or not parsing occured without synactic errors
}

extern "C" int yywrap()
{
  //TODO: cover chaining of multiple files (or not -actually- because I just remembered we're compiling Brainfuck)
  return 1;
}

int yyerror(Parser* p,const char* s)
{
   extern int yylineno;
   std::cerr << "Error with input near line " <<  yylineno << ":\n" <<  s << std::endl;
   return 0;
}
