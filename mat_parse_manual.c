#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "vary_array.h"
#include "mat_parse.h"

lex_context_t g_lexctx;

extern int yylex(void);
extern FILE *yyin;

void mat_lex_init(lex_context_t *ctx) {
	ctx->line = 1;
	ctx->col = 1;
	ctx->blankline = 1;
	ctx->state = lex_init;
	ctx->err = 0;
	ctx->val = 0;
}


int gsl_matrix_load(FILE *file, gsl_matrix **matrix) {
	int id = lex_id_butt;
	int dim=0, rno=0; 
	gsl_matrix *mat = NULL;

	mat_lex_init(&g_lexctx);

	vary_array_t *ary = vary_array_malloc(128);

	yyin = file;
	while(id != lex_id_eof) {
		id = yylex();

		switch(id) {
			case lex_id_num:
				if (g_lexctx.state == lex_init) {
					g_lexctx.state = lex_firt_line;
				} else if (g_lexctx.state == lex_blank_line) {
					g_lexctx.state = lex_nblank_line;
				}

				vary_array_push(ary, g_lexctx.val);
			break;

			case lex_id_nl: case lex_id_eof:
				if (g_lexctx.state == lex_firt_line) {
					dim = ary->cnt;
					fprintf(stdout, "derivate dim: %d\n", dim);
					g_lexctx.state = lex_blank_line;

					if (id==lex_id_eof && dim>1) {
						rno = 1;
						break;
					}

					fprintf(stdout, "allocing matrix with dimension %dx%d...", dim, dim);
					mat = gsl_matrix_alloc(dim, dim);
					fprintf(stdout, "done\n");
					memcpy(gsl_matrix_ptr(mat, rno++, 0), ary->data, dim*sizeof(double));
				} else if (g_lexctx.state == lex_nblank_line) {
					// fprintf(stdout, "rno: %d\n", rno);
					if (rno >= dim) {
						fprintf(stderr, "[error] illegal line %d than since matrix dim is %d\n", g_lexctx.line-1, dim);
						abort();
					}

					if (ary->cnt != dim) {
						fprintf(stderr, "[error] line %d numbers count %d != dim %d (first none blank line numbers count)\n", g_lexctx.line-1, ary->cnt, dim);
						abort();
					}

					memcpy(gsl_matrix_ptr(mat, rno++, 0), ary->data, dim*sizeof(double));
					g_lexctx.state = lex_blank_line;
				}

				vary_array_reset(ary);
			break;

			default:
				fprintf(stderr, "[error] illegal state line %d column %d\n", g_lexctx.line, g_lexctx.col);
				abort();
			break;
		}
	}

	if (rno != dim) {
		fprintf(stderr, "[error] inverse operation require square matrix row %d col %d\n", rno, dim);
		assert(0);
		abort();
	}

	*matrix = mat;
	vary_array_free(ary);

	return dim;
}