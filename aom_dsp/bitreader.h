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

#ifndef VPX_DSP_BITREADER_H_
#define VPX_DSP_BITREADER_H_

#include <stddef.h>
#include <limits.h>

#include "./aom_config.h"
#include "aom_ports/mem.h"
#include "aom/vp8dx.h"
#include "aom/aom_integer.h"
#include "aom_dsp/prob.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef size_t BD_VALUE;

#define BD_VALUE_SIZE ((int)sizeof(BD_VALUE) * CHAR_BIT)

// This is meant to be a large, positive constant that can still be efficiently
// loaded as an immediate (on platforms like ARM, for example).
// Even relatively modest values like 100 would work fine.
#define LOTS_OF_BITS 0x40000000

typedef struct {
  // Be careful when reordering this struct, it may impact the cache negatively.
  BD_VALUE value;
  unsigned int range;
  int count;
  const uint8_t *buffer_end;
  const uint8_t *buffer;
  aom_decrypt_cb decrypt_cb;
  void *decrypt_state;
  uint8_t clear_buffer[sizeof(BD_VALUE) + 1];
} aom_reader;

int aom_reader_init(aom_reader *r, const uint8_t *buffer, size_t size,
                    aom_decrypt_cb decrypt_cb, void *decrypt_state);

void aom_reader_fill(aom_reader *r);

const uint8_t *aom_reader_find_end(aom_reader *r);

static INLINE int aom_reader_has_error(aom_reader *r) {
  // Check if we have reached the end of the buffer.
  //
  // Variable 'count' stores the number of bits in the 'value' buffer, minus
  // 8. The top byte is part of the algorithm, and the remainder is buffered
  // to be shifted into it. So if count == 8, the top 16 bits of 'value' are
  // occupied, 8 for the algorithm and 8 in the buffer.
  //
  // When reading a byte from the user's buffer, count is filled with 8 and
  // one byte is filled into the value buffer. When we reach the end of the
  // data, count is additionally filled with LOTS_OF_BITS. So when
  // count == LOTS_OF_BITS - 1, the user's data has been exhausted.
  //
  // 1 if we have tried to decode bits after the end of stream was encountered.
  // 0 No error.
  return r->count > BD_VALUE_SIZE && r->count < LOTS_OF_BITS;
}

static INLINE int aom_read(aom_reader *r, int prob) {
  unsigned int bit = 0;
  BD_VALUE value;
  BD_VALUE bigsplit;
  int count;
  unsigned int range;
  unsigned int split = (r->range * prob + (256 - prob)) >> CHAR_BIT;

  if (r->count < 0) aom_reader_fill(r);

  value = r->value;
  count = r->count;

  bigsplit = (BD_VALUE)split << (BD_VALUE_SIZE - CHAR_BIT);

  range = split;

  if (value >= bigsplit) {
    range = r->range - split;
    value = value - bigsplit;
    bit = 1;
  }

  {
    register int shift = aom_norm[range];
    range <<= shift;
    value <<= shift;
    count -= shift;
  }
  r->value = value;
  r->count = count;
  r->range = range;

  return bit;
}

static INLINE int aom_read_bit(aom_reader *r) {
  return aom_read(r, 128);  // aom_prob_half
}

static INLINE int aom_read_literal(aom_reader *r, int bits) {
  int literal = 0, bit;

  for (bit = bits - 1; bit >= 0; bit--) literal |= aom_read_bit(r) << bit;

  return literal;
}

static INLINE int aom_read_tree(aom_reader *r, const aom_tree_index *tree,
                                const aom_prob *probs) {
  aom_tree_index i = 0;

  while ((i = tree[i + aom_read(r, probs[i >> 1])]) > 0) continue;

  return -i;
}

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // VPX_DSP_BITREADER_H_
