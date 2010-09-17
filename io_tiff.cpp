#include "io_tiff.h"

#include <tiffio.h>

#include <iostream>
#include <cassert>
#include <cstring>
#include <cstdlib>

/// Read a TIFF float image.
static float* readTIFF(TIFF* tif, size_t& nx, size_t& ny)
{
    uint32 w=0, h=0;
    TIFFGetField(tif, TIFFTAG_IMAGEWIDTH, &w);
    TIFFGetField(tif, TIFFTAG_IMAGELENGTH, &h);
    uint16 spp=0, bps=0, fmt=0;
    TIFFGetField(tif, TIFFTAG_SAMPLESPERPIXEL, &spp);
    TIFFGetField(tif, TIFFTAG_BITSPERSAMPLE, &bps);
    TIFFGetField(tif, TIFFTAG_SAMPLEFORMAT, &fmt);
    if(spp != 1 || bps != (uint16)sizeof(float)*8 || fmt != SAMPLEFORMAT_IEEEFP)
        return false;

    assert((size_t)TIFFScanlineSize(tif) == w*sizeof(float));
    float* data = (float*)malloc(w*h*sizeof(float));
    nx = static_cast<size_t>(w);
    ny = static_cast<size_t>(h);
    for(uint32 i=0; i < h; i++) {
        float* line = data + i*w;
        if(TIFFReadScanline(tif, line, i) < 0) {
            std::cerr << "readTIFF: error reading row " << i << std::endl;
            free(data);
            return NULL;
        }
    }

    return data;
}

/// Write a TIFF float image.
static bool writeTIFF(TIFF* tif, const float* data, size_t w,size_t h,size_t c)
{
    TIFFSetField(tif, TIFFTAG_IMAGEWIDTH, (uint32)w);
    TIFFSetField(tif, TIFFTAG_IMAGELENGTH,(uint32)h);
    TIFFSetField(tif, TIFFTAG_PLANARCONFIG, PLANARCONFIG_SEPARATE);
    TIFFSetField(tif, TIFFTAG_SAMPLESPERPIXEL, (uint16)c);
    TIFFSetField(tif, TIFFTAG_BITSPERSAMPLE, (uint16)sizeof(float)*8);
    TIFFSetField(tif, TIFFTAG_SAMPLEFORMAT, SAMPLEFORMAT_IEEEFP);
    uint32 rowsperstrip = TIFFDefaultStripSize(tif, (uint32)h);
    TIFFSetField(tif, TIFFTAG_ROWSPERSTRIP, rowsperstrip);
    TIFFSetField(tif, TIFFTAG_COMPRESSION, COMPRESSION_NONE);
    TIFFSetField(tif, TIFFTAG_ORIENTATION, ORIENTATION_TOPLEFT);

    bool ok=true;
    for(size_t k=0; ok && k < c; k++)
        for(size_t i=0; ok && i < h; i++) {
            float* line = const_cast<float*>(data + (i+k*h)*w);
            if(TIFFWriteScanline(tif, line, i, k) < 0) {
                std::cout << "writeTIFF: error writing row " << i << std::endl;
                ok = false;
            }
        }
    return ok;
}

/// Load TIFF float image.
float* read_tiff_f32_gray(const char *fname, size_t *nx, size_t *ny)
{
    TIFF* tif = TIFFOpen(fname, "r");
    if(! tif) {
        std::cerr << "Unable to read TIFF file " << fname << std::endl;
        return false;
    }
    float* data  = readTIFF(tif, *nx, *ny);
    TIFFClose(tif);
    return data;
}

/// Write float image as TIFF 32 bits per sample.
int write_tiff_f32(const char *fname, const float *data, size_t nx, size_t ny, size_t nc)
{
    TIFF* tif = TIFFOpen(fname, "w");
    if(! tif) {
        std::cerr << "Unable to write TIFF file " << fname << std::endl;
        return false;
    }

    bool ok = writeTIFF(tif, data, nx, ny, nc);
    TIFFClose(tif);
    return (ok? 0: -1);
}
