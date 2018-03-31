#include <stdio.h>
#include <assert.h>
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_errno.h>
#include "mat_parse.h"

int main(int argc, char const *argv[])
{
  int err=0, sign=0;
  FILE *file=NULL, *outfile=NULL;
  file = fopen("mat.dat", "r");
  if (!file)
  {
     fprintf(stderr, "%s\n", "err: can't open input file mat.dat");
     return 1;
  }

  outfile = fopen("out.dat", "w");
  if (!outfile)
  {
     fprintf(stderr, "%s\n", "err: can't create output file out.dat");
     return 1;
  }

  gsl_matrix *mat = NULL;
  int dim = gsl_matrix_load(file, &mat);
  if (dim<=0 || mat==NULL) {
    return 1;
  }

  fprintf(stdout, "allocing inverse matrix with dimension...");
  gsl_matrix *mat_inv  
    = gsl_matrix_alloc(dim, dim);
  fprintf(stdout, "%s\n", "done");

  gsl_permutation *p 
    = gsl_permutation_alloc(dim);

  fprintf(stdout, "LU decomp...");
  err = gsl_linalg_LU_decomp(mat, p, &sign);
  assert(err == GSL_SUCCESS);
  fprintf(stdout, "%s\n", "done");

  fprintf(stdout, "calculate inverse...");
  gsl_linalg_LU_invert(mat, p, mat_inv);
  fprintf(stdout, "%s\n", "done");

  gsl_permutation_free(p);
  gsl_matrix_free(mat);

  fprintf(stdout, "output inverse matrix to file out.dat ...");
  for (int i = 0; i < dim; ++i)
  {
    for (int j = 0; j < dim; ++j)
    {
        fprintf(outfile, "%g ", gsl_matrix_get(mat_inv, i, j));
    }
    fprintf(outfile, "\n");
  }
  fprintf(stdout, "%s\n", "done");

  gsl_matrix_free(mat_inv);

  fclose(file);
  fclose(outfile);

  return 0;
}
