#ifndef __MAT_INVERSE_H
#define __MAT_INVERSE_H

#include <stdio.h>
#include <gsl/gsl_matrix.h>

#ifdef __cplusplus
extern "C" {
#endif

extern struct lex_context g_lexctx;

typedef enum lex_id {
	lex_id_num,
	lex_id_nl,
	lex_id_other,
	lex_id_eof,
	lex_id_butt,
}lex_id_e;

typedef enum lex_state {
	lex_init,
	lex_firt_line,
	lex_blank_line,
	lex_nblank_line,
}lex_state_e;

typedef struct lex_context {
	int line;
	int col;

	int blankline:1;
	int state:3;
	int err : 8;

	double val;
}lex_context_t;

int gsl_matrix_load(FILE *file, gsl_matrix **mat);

#ifdef __cplusplus
}
#endif

#endif