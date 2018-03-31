#include <stdio.h>
#include <stdlib.h>
#include <assert.h>


int main(int argc, char const *argv[])
{
	FILE *out = NULL;
	int dim = 0;
	if (argc < 2)
	{
		fprintf(stdout, "dimension: \n");
		fscanf(stdin, "%d", &dim);
	} else {
		dim = atoi(argv[1]);
	}

	if (dim <= 0)
	{
		fprintf(stderr, "illegal dimension number: %d\n", dim);
		return 1;
	}

	out = fopen("mat.dat", "w");
	for (int i = 0; i < dim; ++i)
	{
		for (int j = 0; j < dim; ++j)
		{
			fprintf(out, "%d ", i==j ? 1 : 0);
		}

		fprintf(out, "\n");
	}

	fclose(out);

	return 0;
}