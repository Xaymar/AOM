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

#ifndef VP10_COMMON_SEG_COMMON_H_
#define VP10_COMMON_SEG_COMMON_H_

#include "aom_dsp/prob.h"

#ifdef __cplusplus
extern "C" {
#endif

#define SEGMENT_DELTADATA 0
#define SEGMENT_ABSDATA 1

#define MAX_SEGMENTS 8
#define SEG_TREE_PROBS (MAX_SEGMENTS - 1)

#define PREDICTION_PROBS 3

// Segment level features.
typedef enum {
  SEG_LVL_ALT_Q = 0,      // Use alternate Quantizer ....
  SEG_LVL_ALT_LF = 1,     // Use alternate loop filter value...
  SEG_LVL_REF_FRAME = 2,  // Optional Segment reference frame
  SEG_LVL_SKIP = 3,       // Optional Segment (0,0) + skip mode
  SEG_LVL_MAX = 4         // Number of features supported
} SEG_LVL_FEATURES;

struct segmentation {
  uint8_t enabled;
  uint8_t update_map;
  uint8_t update_data;
  uint8_t abs_delta;
  uint8_t temporal_update;

  int16_t feature_data[MAX_SEGMENTS][SEG_LVL_MAX];
  unsigned int feature_mask[MAX_SEGMENTS];
};

struct segmentation_probs {
  aom_prob tree_probs[SEG_TREE_PROBS];
  aom_prob pred_probs[PREDICTION_PROBS];
};

static INLINE int segfeature_active(const struct segmentation *seg,
                                    int segment_id,
                                    SEG_LVL_FEATURES feature_id) {
  return seg->enabled && (seg->feature_mask[segment_id] & (1 << feature_id));
}

void vp10_clearall_segfeatures(struct segmentation *seg);

void vp10_enable_segfeature(struct segmentation *seg, int segment_id,
                            SEG_LVL_FEATURES feature_id);

int vp10_seg_feature_data_max(SEG_LVL_FEATURES feature_id);

int vp10_is_segfeature_signed(SEG_LVL_FEATURES feature_id);

void vp10_set_segdata(struct segmentation *seg, int segment_id,
                      SEG_LVL_FEATURES feature_id, int seg_data);

static INLINE int get_segdata(const struct segmentation *seg, int segment_id,
                              SEG_LVL_FEATURES feature_id) {
  return seg->feature_data[segment_id][feature_id];
}

extern const aom_tree_index vp10_segment_tree[TREE_SIZE(MAX_SEGMENTS)];

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // VP10_COMMON_SEG_COMMON_H_
