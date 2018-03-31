AM_YFLAGS = -d
bin_PROGRAMS = matinverse matgen
matinverse_SOURCES = mat_parse.y mat_lex.l main.c vary_array.h vary_array.c
matinverse_LDFLAGS = -lgsl -lgslcblas $(MATINV_LIBS)

EXTRA_DIST = mat_parse_manual.h mat_parse_manual.c

matgen_SOURCES = mat_gen.c