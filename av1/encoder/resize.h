/*
 * Copyright (c) 2016, Alliance for Open Media. All rights reserved
 *
 * This source code is subject to the terms of the BSD 2 Clause License and
 * the Alliance for Open Media Patent License 1.0. If the BSD 2 Clause License
 * was not distributed with this source code in the LICENSE file, you can
 * obtain it at www.aomedia.org/license/software. If the Alliance for Open
 * Media Patent License 1.0 was not distributed with this source code in the
 * PATENTS file, you can obtain it at www.aomedia.org/license/patent.
 */

#ifndef VP10_ENCODER_RESIZE_H_
#define VP10_ENCODER_RESIZE_H_

#include <stdio.h>
#include "aom/aom_integer.h"

#ifdef __cplusplus
extern "C" {
#endif

void vp10_resize_plane(const uint8_t *const input, int height, int width,
                       int in_stride, uint8_t *output, int height2, int width2,
                       int out_stride);
void vp10_resize_frame420(const uint8_t *const y, int y_stride,
                          const uint8_t *const u, const uint8_t *const v,
                          int uv_stride, int height, int width, uint8_t *oy,
                          int oy_stride, uint8_t *ou, uint8_t *ov,
                          int ouv_stride, int oheight, int owidth);
void vp10_resize_frame422(const uint8_t *const y, int y_stride,
                          const uint8_t *const u, const uint8_t *const v,
                          int uv_stride, int height, int width, uint8_t *oy,
                          int oy_stride, uint8_t *ou, uint8_t *ov,
                          int ouv_stride, int oheight, int owidth);
void vp10_resize_frame444(const uint8_t *const y, int y_stride,
                          const uint8_t *const u, const uint8_t *const v,
                          int uv_stride, int height, int width, uint8_t *oy,
                          int oy_stride, uint8_t *ou, uint8_t *ov,
                          int ouv_stride, int oheight, int owidth);

#if CONFIG_VPX_HIGHBITDEPTH
void vp10_highbd_resize_plane(const uint8_t *const input, int height, int width,
                              int in_stride, uint8_t *output, int height2,
                              int width2, int out_stride, int bd);
void vp10_highbd_resize_frame420(const uint8_t *const y, int y_stride,
                                 const uint8_t *const u, const uint8_t *const v,
                                 int uv_stride, int height, int width,
                                 uint8_t *oy, int oy_stride, uint8_t *ou,
                                 uint8_t *ov, int ouv_stride, int oheight,
                                 int owidth, int bd);
void vp10_highbd_resize_frame422(const uint8_t *const y, int y_stride,
                                 const uint8_t *const u, const uint8_t *const v,
                                 int uv_stride, int height, int width,
                                 uint8_t *oy, int oy_stride, uint8_t *ou,
                                 uint8_t *ov, int ouv_stride, int oheight,
                                 int owidth, int bd);
void vp10_highbd_resize_frame444(const uint8_t *const y, int y_stride,
                                 const uint8_t *const u, const uint8_t *const v,
                                 int uv_stride, int height, int width,
                                 uint8_t *oy, int oy_stride, uint8_t *ou,
                                 uint8_t *ov, int ouv_stride, int oheight,
                                 int owidth, int bd);
#endif  // CONFIG_VPX_HIGHBITDEPTH

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // VP10_ENCODER_RESIZE_H_
