%{
  #include <stdlib.h>
  #include "parser.h"
  #define YYERROR_VERBOSE
%}


%code requires{
  #include "driver.hpp"
  #include "ast.hpp"
}
%code provides{
  int yyparse (Parser* parser);
  int yyerror(Parser* p,const char* s);
  int yylex(YYSTYPE*);
}

%defines
%pure-parser
%parse-param{Parser* parser}
%expect 0 //No conflicts here

%token <smb> RPTR LPTR INCCELL DECCELL PRINTBY EATBY LSQBR RSQBR BAD_TOKEN

%union{
  int val;
  OpNode* op;
  OpList* ops;
}

%type <op> Op
%type <ops> OpPair File

%%
  //Rule(s) section

  Top:
    File {parser->setAST($1);
          parser->accept();}
  ;

  File:
    OpPair  {$$ = $1;}
  ;

  OpPair:
    Op OpPair {if($1) $2->getOps()->push_front($1); $$ = $2;}
    |%empty {$$ = new OpList();}
  ;

  Op:
    RPTR  {$$ = new IncPtrOp();}
    |LPTR {$$ = new DecPtrOp();}
    |INCCELL {$$ = new IncCellOp();}
    |DECCELL {$$ = new DecCellOp();}
    |PRINTBY {$$ = new PrintCellOp();}
    |EATBY {$$ = new EatByteOp();}
    |LSQBR OpPair RSQBR {$$ = new LoopOp($2);}
  ;
%%
