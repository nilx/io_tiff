#ifndef IOTIFF_H
#define IOITFF_H

#ifdef __cplusplus
extern "C" {
#endif

#define IO_TIFF_VERSION "0.20100817"

#include <stddef.h>

float* read_tiff_f32_gray(const char *fname, size_t *nx, size_t *ny);
int write_tiff_f32(const char *fname, const float *data, size_t nx, size_t ny, size_t nc);

#ifdef __cplusplus
}
#endif

#endif
