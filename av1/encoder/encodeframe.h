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

#ifndef VP10_ENCODER_ENCODEFRAME_H_
#define VP10_ENCODER_ENCODEFRAME_H_

#include "aom/aom_integer.h"

#ifdef __cplusplus
extern "C" {
#endif

struct macroblock;
struct yv12_buffer_config;
struct VP10_COMP;
struct ThreadData;

// Constants used in SOURCE_VAR_BASED_PARTITION
#define VAR_HIST_MAX_BG_VAR 1000
#define VAR_HIST_FACTOR 10
#define VAR_HIST_BINS (VAR_HIST_MAX_BG_VAR / VAR_HIST_FACTOR + 1)
#define VAR_HIST_LARGE_CUT_OFF 75
#define VAR_HIST_SMALL_CUT_OFF 45

void vp10_setup_src_planes(struct macroblock *x,
                           const struct yv12_buffer_config *src, int mi_row,
                           int mi_col);

void vp10_encode_frame(struct VP10_COMP *cpi);

void vp10_init_tile_data(struct VP10_COMP *cpi);
void vp10_encode_tile(struct VP10_COMP *cpi, struct ThreadData *td,
                      int tile_row, int tile_col);

void vp10_set_variance_partition_thresholds(struct VP10_COMP *cpi, int q);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // VP10_ENCODER_ENCODEFRAME_H_
