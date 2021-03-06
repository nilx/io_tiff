# Copyright 2010 Nicolas Limare <nicolas.limare@cmla.ens-cachan.fr>
#
# Copying and distribution of this file, with or without
# modification, are permitted in any medium without royalty provided
# the copyright notice and this notice are preserved.  This file is
# offered as-is, without any warranty.

# source code, C language
CSRC	= io_tiff.c example.c

# source code, all languages (only C here)
SRC	= $(CSRC)
# object files (partial compilation)
OBJ	= $(CSRC:.c=.o)
# binary executable program
BIN	= example

# default target: the binary executable program
default: $(BIN)

# standard C compiler optimization options
COPT	= -O2 -funroll-loops -fomit-frame-pointer
# complete C compiler options
CFLAGS	= -ansi -pedantic -Wall -Wextra -Werror -pipe $(COPT)
# linker options
LDFLAGS	= -ltiff -lm
# library build dependencies (none)
LIBDEPS =

# use local embedded libraries
ifdef LOCAL_LIBS
# library location
LIBDIR = ./libs/build/lib
INCDIR = ./libs/build/include
# libtiff is required
LIBDEPS += libtiff
# compile options to use the local libtiff header
CFLAGS 	+= -I$(INCDIR) -DIO_TIFF_LOCAL_LIBTIFF
# link options to use the local libraries
LDFLAGS = $(LIBDIR)/libtiff.a $(LIBDIR)/libz.a $(LIBDIR)/libjpeg.a -lm
endif

# build the tiff library
.PHONY	: libtiff
libtiff	:
	$(MAKE) -C libs libtiff

# partial C compilation xxx.c -> xxx.o
%.o	: %.c $(LIBDEPS)
	$(CC) $< -c $(CFLAGS) -o $@

# final link of the partially compiled files
$(BIN)	: $(OBJ) $(LIBDEPS)
	$(CC) $(OBJ) $(LDFLAGS) -o $@

# cleanup
.PHONY	: clean distclean scrub
clean	:
	$(RM) $(OBJ)
	$(RM) *.flag
	$(MAKE) -C libs $@
distclean	: clean
	$(RM) $(BIN)
	$(MAKE) -C libs $@
scrub	: distclean
	$(MAKE) -C libs $@
