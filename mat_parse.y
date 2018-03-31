%code requires {
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <gsl/gsl_matrix.h>

#include "vary_array.h"

typedef struct mat_parser {
	int rno;
	int dim;
	int blanknum;

	vary_array_t *ary;

	gsl_matrix *mat;
	// lex related
	int lineno;
	int colno;
	double val;
}mat_parser_t;

extern mat_parser_t g_matparser;

mat_parser_t *mat_parser_alloc();
void mat_parser_init(mat_parser_t *);
void mat_parser_free(mat_parser_t *);
int gsl_matrix_load(FILE *file, gsl_matrix **tmat);

#define parser (&g_matparser)
}

%code {
mat_parser_t g_matparser;
extern FILE *yyin;

extern int yylex();

void yyerror(const char *msg) {
    fprintf(stderr, "%s: line %d col %d\n", msg, parser->lineno, parser->colno);
    abort();
}

}

%token NL
%token <value> NUM
%type  <placeholder> matrix blank_or_rows blank row 

%union {
	int 	placeholder;
	double 	value;
};
%%
matrix : blank_or_rows {
	if (parser->rno != parser->dim) {
		fprintf(stderr, "line %d error: row count %d small than column's\n", parser->lineno, parser->rno);
		abort();
	}

	fprintf(stdout, "[info] parse total %d lines, %d blank lines, dimension %d\n", parser->lineno, parser->blanknum, parser->dim);
}

blank_or_rows : blank
			  | blank_or_rows NL blank
     		  | row {
                int cnt = parser->ary->cnt;
                assert(cnt > 0);

                parser->dim = cnt;
                parser->mat = gsl_matrix_alloc(parser->dim, parser->dim);

                fprintf(stdout, "[info] derivate dim: %d\n", parser->dim);

                memcpy(gsl_matrix_ptr(parser->mat, parser->rno++, 0), parser->ary->data, parser->dim*sizeof(double));
                parser->ary->cnt = 0;
              }
              | blank_or_rows NL row {
     		  	int cnt = parser->ary->cnt;

     		  	assert(cnt > 0);

     		  	if(cnt != parser->dim) {
     		  		fprintf(stderr, "line %d error: different dimension %d\n", parser->lineno, cnt);
     		  		abort();
     		  	}

     		  	if(parser->rno >= parser->dim) {
     		  		fprintf(stderr, "line %d error: row count %d bigger than column's\n", parser->lineno, cnt);
     		  		abort();
     		  	}

     		  	memcpy(gsl_matrix_ptr(parser->mat, parser->rno++, 0), parser->ary->data, parser->dim*sizeof(double));
     		  	parser->ary->cnt = 0;
     		  }
     		  

blank :  {parser->blanknum++; fprintf(stdout, "[info] find blank line at line %d\n", parser->lineno);}

row : NUM {vary_array_push(parser->ary, $1);}
    | row NUM {vary_array_push(parser->ary, $2);}

%%
#undef parser

mat_parser_t *mat_parser_alloc() {
	mat_parser_t *parser = malloc(sizeof(mat_parser_t));
	mat_parser_init(parser);
    return parser;
}

void mat_parser_init(mat_parser_t *parser) {
    parser->rno = 0;
    parser->dim = 0;
    parser->blanknum = 0;

    parser->ary = vary_array_malloc(128);
    parser->mat = NULL;

    parser->lineno = 1;
    parser->colno = 1;
    parser->val = 0;
}

void mat_parser_free(mat_parser_t *parser) {
	if(parser->ary) {
		vary_array_free(parser->ary);
	}
	free(parser);
}


int gsl_matrix_load(FILE *file, gsl_matrix **tmat) {
    yyin = file;

    mat_parser_init(&g_matparser);
    yyparse();
    *tmat = g_matparser.mat;

    return g_matparser.dim;
}




