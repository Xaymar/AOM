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

#ifndef TEST_UTIL_H_
#define TEST_UTIL_H_

#include <stdio.h>
#include <math.h>
#include "third_party/googletest/src/include/gtest/gtest.h"
#include "aom/aom_image.h"

// Macros
#define GET_PARAM(k) std::tr1::get<k>(GetParam())

inline double compute_psnr(const aom_image_t *img1, const aom_image_t *img2) {
  assert((img1->fmt == img2->fmt) && (img1->d_w == img2->d_w) &&
         (img1->d_h == img2->d_h));

  const unsigned int width_y = img1->d_w;
  const unsigned int height_y = img1->d_h;
  unsigned int i, j;

  int64_t sqrerr = 0;
  for (i = 0; i < height_y; ++i)
    for (j = 0; j < width_y; ++j) {
      int64_t d = img1->planes[VPX_PLANE_Y][i * img1->stride[VPX_PLANE_Y] + j] -
                  img2->planes[VPX_PLANE_Y][i * img2->stride[VPX_PLANE_Y] + j];
      sqrerr += d * d;
    }
  double mse = static_cast<double>(sqrerr) / (width_y * height_y);
  double psnr = 100.0;
  if (mse > 0.0) {
    psnr = 10 * log10(255.0 * 255.0 / mse);
  }
  return psnr;
}

#endif  // TEST_UTIL_H_
