# $Id: Makefile.inc 1762 2007-02-12 03:48:53Z peter $

libyasm_a_SOURCES += modules/objfmts/coff/coff-objfmt.c
libyasm_a_SOURCES += modules/objfmts/coff/coff-objfmt.h
libyasm_a_SOURCES += modules/objfmts/coff/win64-except.c

YASM_MODULES += objfmt_coff

EXTRA_DIST += modules/objfmts/coff/tests/Makefile.inc

include modules/objfmts/coff/tests/Makefile.inc
