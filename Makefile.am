AM_YFLAGS = -d
bin_PROGRAMS =  matgen matadd matinverse
matinverse_SOURCES = mat_parse.y mat_lex.l main.c vary_array.h vary_array.c
matinverse_LDADD = -lgsl -lgslcblas $(MATINV_LIBS)

matadd_SOURCES = idxmat_parse.y idxmat_lex.l matadd.c vary_array.h vary_array.c
matadd_LDADD = -lgsl -lgslcblas $(MATINV_LIBS)

EXTRA_DIST = mat_parse_manual.h mat_parse_manual.c

matgen_SOURCES = mat_gen.c