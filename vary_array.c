#include <stdio.h>
#include <stdlib.h>
#include "vary_array.h"

vary_array_t*
vary_array_malloc(int cap) {
	vary_array_t *ary = malloc(sizeof(vary_array_t));
    ary->cnt = 0;
    ary->cap = cap;
    ary->data = malloc(cap * sizeof(double));
    return ary;
}

void 
vary_array_free(vary_array_t *ary) {
	free(ary->data);
	free(ary);
}

void 
vary_array_push(vary_array_t *ary, double val) {
	if (ary->cnt >= ary->cap) {
		ary->cap *= 2;
		ary->data = realloc(ary->data, ary->cap*sizeof(double));
	}

	ary->data[ary->cnt++] = val;
}

void
vary_array_reset(vary_array_t *ary) {
	ary->cnt = 0;
}