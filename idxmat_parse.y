%code requires {
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <gsl/gsl_matrix.h>
#include "vary_array.h"

typedef struct idxmat_parser {
    int rno;
    int dim;
    int blanknum;
    int cur_idx1, last_idx1;

    vary_array_t *ary;

    gsl_matrix *mat;
    // lex related
    int lineno;
    int colno;
} idxmat_parser_t;

extern idxmat_parser_t g_idxmat_parser;

idxmat_parser_t *idxmat_parser_alloc();
void idxmat_parser_init(idxmat_parser_t *);
void idxmat_parser_free(idxmat_parser_t *);
int gsl_idxmatrix_load(FILE *file, gsl_matrix **tmat);

#define parser (&g_idxmat_parser)
}

%code {
idxmat_parser_t g_idxmat_parser;
extern FILE *yyin;

extern int yylex();

void yyerror(const char *msg) {
    fprintf(stderr, "%s: line %d col %d\n", msg, parser->lineno, parser->colno);
    abort();
}

static void add_one_value(double val) {
    vary_array_push(parser->ary, val);

    if(parser->last_idx1 == -1) {
        parser->last_idx1 = parser->cur_idx1;
        return;
    }

    if(parser->cur_idx1 == parser->last_idx1) {
        return;
    }

    parser->last_idx1 = parser->cur_idx1;

    if(!parser->mat) {
        parser->dim = parser->ary->cnt;
        parser->mat = gsl_matrix_calloc(parser->dim, parser->dim);
        fprintf(stdout, "[info] derivate dim: %d\n", parser->dim);
    }

    memcpy(gsl_matrix_ptr(parser->mat, parser->rno++, 0), parser->ary->data, parser->ary->cnt * sizeof(double));
    vary_array_reset(parser->ary);
}
}

%token<ival> INT 
%token<dval> FLOAT
%token NL
%type<ival> matrix lines line blank 

%union {
    int ival;
    double dval;
}

%%

matrix: lines {
    fprintf(stdout, "[info] parse total %d lines, dimension %d, blank line %d\n", parser->lineno, parser->dim, parser->blanknum);
}

lines : line
      | lines NL line

line : blank {parser->blanknum++;}
     | INT INT INT {
        parser->cur_idx1 = $1;
        add_one_value($3);
     }
     | INT INT FLOAT {
        parser->cur_idx1 = $1;
        add_one_value($3);
     }

blank: {}

%% 

idxmat_parser_t *idxmat_parser_alloc() {
    idxmat_parser_t *p = malloc(sizeof(idxmat_parser_t));
    idxmat_parser_init(p);
    return p;
}

void idxmat_parser_init(idxmat_parser_t *p) {
    p->rno = 0;
    p->dim = 0;
    p->blanknum = 0;
    p->cur_idx1 = -1;
    p->last_idx1 = -1;

    p->ary = vary_array_malloc(128);
    p->mat = NULL;

    p->lineno = 1;
    p->colno = 1;
}

void idxmat_parser_free(idxmat_parser_t *p) {
    if(p->ary) {
        vary_array_free(p->ary);
    }
    free(p);
}


int gsl_idxmatrix_load(FILE *file, gsl_matrix **tmat) {
    yyin = file;

    idxmat_parser_init(&g_idxmat_parser);
    yyparse();
    *tmat = g_idxmat_parser.mat;

    return g_idxmat_parser.dim;
}


