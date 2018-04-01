%{
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "idxmat_parse.h"
%}

%option noyywrap

%%

[0-9]+ {parser->colno+=yyleng; yylval.ival=atoi(yytext); return INT;}

[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)? {
    parser->colno+=yyleng; 
    yylval.dval=atof(yytext);
    return FLOAT;
}

[ \t\r,]+ {parser->colno+=yyleng;}

\n {parser->lineno++; return NL;}

<<EOF>> {return 0;}

. {
    fprintf(stderr, "[error] illegal character 0x%x at line %d column %d\n", 
     *yytext, parser->lineno, parser->colno);
    abort();
}



