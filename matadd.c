#include "idxmat_parse.h"

int main(int argc, char const *argv[])
{
    FILE *gf, *a2f, *af, *hf;

    gf = fopen("gmat.dat", "r");
    if (!gf) {
        fprintf(stderr, "%s\n", "[error] can't open file gmat.dat");
        return 1;
    }

    a2f = fopen("a2mat.dat", "r");
    if (!a2f) {
        fprintf(stderr, "%s\n", "[error] can't open file a2fmat.dat");
        return 1;
    }

    af = fopen("amat.dat", "r");
    if (!af) {
        fprintf(stderr, "%s\n", "[error] can't open file amat.dat");
        return 1;
    }

    hf = fopen("hmat.mat", "w");
    if (!hf) {
        fprintf(stderr, "%s\n", "[error] can't open file hmat.dat");
        return 1;
    }

    gsl_matrix *gmat, *a2mat, *amat;
    int gdim, a2dim, adim;

    fprintf(stdout, "[info] parsing gmat.dat...\n");
    gdim = gsl_idxmatrix_load(gf, &gmat);
    fprintf(stdout, "[info] parse done!\n\n");

    fprintf(stdout, "[info] parsing a2mat.dat...\n");
    a2dim = gsl_idxmatrix_load(a2f, &a2mat);
    fprintf(stdout, "[info] parse done!\n\n");
    if (gdim != a2dim) {
        assert(0);
        return 1;
    }

    fprintf(stdout, "[info] calculating (G-A2)...\n");
    gsl_matrix_sub(gmat, a2mat);
    fprintf(stdout, "[info] calculate done!\n\n");
    gsl_matrix_free(a2mat);

    fprintf(stdout, "[info] parsing amat.dat...\n");
    adim = gsl_idxmatrix_load(af, &amat);
    fprintf(stdout, "[info] parse done!\n\n");
    if (adim < gdim) {
        fprintf(stderr, "[error] amat dimension less than gmat, exit\n");
        return 1;
    }

    fprintf(stdout, "[info] calculating A + (G-A2)...\n");
    gsl_matrix_view submat = gsl_matrix_submatrix(amat, adim-gdim, adim-gdim, gdim, gdim);
    gsl_matrix_add(&submat.matrix, gmat);
    gsl_matrix_free(gmat);
    fprintf(stdout, "[info] calculate done!\n\n");

    fprintf(stdout, "[info] writing result matrix to file hmat.dat...\n");
    for (int i = 0; i < adim; ++i) {
        for (int j = 0; j < adim; ++j) {
            fprintf(hf, "%g\n", gsl_matrix_get(amat, i, j));
        }
    }
    fprintf(stdout, "[info] write done\n");

    gsl_matrix_free(amat);

    return 0;
}