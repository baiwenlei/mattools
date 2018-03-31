%{
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "mat_parse.h"
%}

%array
%option noyywrap
%%

[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?	{ 
	yylval.value = atof(yytext);
	parser->colno += yyleng;
	return NUM;
}

[ \t,\r]+	{parser->colno += yyleng;}

\n 			{++parser->lineno; parser->colno=0; return NL;}

<<EOF>> 	{return 0;}

. 			{ 
				fprintf(stderr, "[error] illegal character 0x%x in line %d column %d\n", *yytext, parser->lineno, parser->colno);
				abort();
			}
