%option yylineno
%{
   #include <iostream>
   #include <iomanip>
   #include "parser.h"
   #include "driver.hpp"
%}

WS [ \t\r\n]*

%%

{WS}  {/*Do nothing*/}

">"   {return RPTR;}
"<"   {return LPTR;}
"+"   {return INCCELL;}
"-"   {return DECCELL;}
"."   {return PRINTBY;}
","   {return EATBY;}
"["   {return LSQBR;}
"]"   {return RSQBR;}

.   {printf("Unrecognized token: %s\n",yytext); return BAD_TOKEN;}

%%

/* In truth we could ignore erroneous symbols, rather than throwing an error on them, but there's something satisfying about
 * BF code devoid of all but operation symbols and whitespace (plus, the lack of meaningful characters is the main source of brainfuckery)*/
