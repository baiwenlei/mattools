#ifndef __VARY_ARRAY_H
#define __VARY_ARRAY_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct vary_array {
	int cnt;
	int cap;
	double *data;
}vary_array_t;

vary_array_t*
vary_array_malloc(int cap);

void 
vary_array_free(vary_array_t *ary);

void 
vary_array_push(vary_array_t *ary, double val);

void
vary_array_reset(vary_array_t *ary);

#ifdef __cplusplus
}
#endif

#endif