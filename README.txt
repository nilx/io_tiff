% io_tiff: simplified front-end to libtiff

* overview
* license
* requirements
* usage
 * read
 * write
 * example
* compilation
 * local libraries
* todo
* copyright

# OVERVIEW

io_tiff.c contains high-level routines to read and write TIFF images
using libtiff. It focuses on floating-point images, only handles
common use cases, and provides a simplified interface.

# LICENSE

io_tiff.c is distributed under a GPL3+ or BSD licence, at your
option. See the included copyright notice, conditions and disclaimer
for details.

# REQUIREMENTS

libtiff is required, version >= 3.9. The source code and binaries
can be found at http://www.remotesensing.org/libtiff/.

Note that libtiff uses zlib and libjpeg for compression. The
source code and binaries can be found at http://www.zlib.net/ and
http://www.ijg.org/.

io_tiff.c is ANSI C, and should compile on any system with any ANSI C
compiler.

# USAGE

Compile io_tiff.c with your program, and include io_tiff.h to get the
function declarations. You can use io_tiff.c with C or C++ code.

## READ

A TIFF image is read into a single array. For multiple channel images,
the output array is deinterlaced and successively contains each
channel. For example, a color image with 30 rows and 40 columns is
read into a single array of 3600 cells, with:

* the first 1200 cells (30 x 40) containing the red channel
* the next 1200 cells containing the green channel
* the last 1200 cells containing the blue channel

In each channel, the image is stored row after row.

No image structure is needed, and the image size information is
collected via pointer parameters.

The main front-end function is:

* read_tiff_f32():
  read a TIFF image as a float array
  - integer images are converted to float

The function syntax is:

    read_tiff_f32(fname, &nx, &ny, &nc)
    - fname: file name; the standard input stream is used if fname is "-"
    - nx, ny, nc: variables to fill with the image size

> NOT IMPLEMENTED YET
> Two secondary read functions can be used to force a color model:
> 
> * read_tiff_f32_rgb():
>   convert gray images to RGB and strip the alpha channel 
> * read_tiff_f32_gray():
>   convert RGB images to gray and strip the alpha channel
> 
> These functions have the same syntax as the previous ones, except that
> they don't need the &nc parameter.

## WRITE

A TIFF image is written from a single array, with the same layout as
the one received from the read functions.

The front-end function is:

* write_tiff_f32():
  write a TIFF image from a float array
  - the float values are stored in a floating-point TIFF image without
    conversion

The function syntax is:

    write_tiff_f32(fname, data, nx, ny, nc)
    - fname: file name, the standard output stream is used if fname is "-"
    - data: image array
    - nx, ny, nc: image size


## EXAMPLE

see example.c

# COMPILATION

You can compile the example code example.c using the provided
makefile, with the `make` command.

## LOCAL LIBRARIES

If libtiff is not installed on your system, of if you prefer a local
static build, a mechanism is available to automatically download,
build and include libtiff in your program:

1. run `make libtiff`;
   this uses the makefiles from the `libs` folder to download and
   compile libtiff, zlib and libjpeg, and builds the libraries into
   `libs/build`;
2. use the "-DIO_TIFF_LOCAL_LIBTIFF -I./libs/build/include" options to compile
   io_tiff.c;
3. add ./libs/build/lib/libtiff.a ./libs/build/lib/libz.a
   ./libs/build/lib/libjpeg.a to the list of files being linked into
   your program

This is automatically handled in the provided makefile for the example
code example.c; simply use the `make LOCAL_LIBS=1` command
instead of `make`.

# TODO

* handle strip/tile images, any channels
* implement all the missing parts
* cmake support
* C++ wrappers (vector output, merged functions)

# COPYRIGHT

Copyright 2010-2011 Nicolas Limare <nicolas.limare@cmla.ens-cachan.fr>

Copying and distribution of this README file, with or without
modification, are permitted in any medium without royalty provided
the copyright notice and this notice are preserved.  This file is
offered as-is, without any warranty.
