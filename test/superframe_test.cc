/*
 *  Copyright (c) 2012 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */
#include <climits>
#include "third_party/googletest/src/include/gtest/gtest.h"
#include "test/codec_factory.h"
#include "test/encode_test_driver.h"
#include "test/i420_video_source.h"
#include "test/util.h"

namespace {

const int kTestMode = 0;
const int kSuperframeSyntax = 1;

typedef std::tr1::tuple<libaom_test::TestMode, int> SuperframeTestParam;

class SuperframeTest
    : public ::libaom_test::EncoderTest,
      public ::libaom_test::CodecTestWithParam<SuperframeTestParam> {
 protected:
  SuperframeTest()
      : EncoderTest(GET_PARAM(0)), modified_buf_(NULL), last_sf_pts_(0) {}
  virtual ~SuperframeTest() {}

  virtual void SetUp() {
    InitializeConfig();
    const SuperframeTestParam input = GET_PARAM(1);
    const libaom_test::TestMode mode = std::tr1::get<kTestMode>(input);
    const int syntax = std::tr1::get<kSuperframeSyntax>(input);
    SetMode(mode);
    sf_count_ = 0;
    sf_count_max_ = INT_MAX;
    is_vp10_style_superframe_ = syntax;
  }

  virtual void TearDown() { delete[] modified_buf_; }

  virtual void PreEncodeFrameHook(libaom_test::VideoSource *video,
                                  libaom_test::Encoder *encoder) {
    if (video->frame() == 1) {
      encoder->Control(VP8E_SET_ENABLEAUTOALTREF, 1);
    }
  }

  virtual const aom_codec_cx_pkt_t *MutateEncoderOutputHook(
      const aom_codec_cx_pkt_t *pkt) {
    if (pkt->kind != VPX_CODEC_CX_FRAME_PKT) return pkt;

    const uint8_t *buffer = reinterpret_cast<uint8_t *>(pkt->data.frame.buf);
    const uint8_t marker = buffer[pkt->data.frame.sz - 1];
    const int frames = (marker & 0x7) + 1;
    const int mag = ((marker >> 3) & 3) + 1;
    const unsigned int index_sz =
        2 + mag * (frames - is_vp10_style_superframe_);
    if ((marker & 0xe0) == 0xc0 && pkt->data.frame.sz >= index_sz &&
        buffer[pkt->data.frame.sz - index_sz] == marker) {
      // frame is a superframe. strip off the index.
      if (modified_buf_) delete[] modified_buf_;
      modified_buf_ = new uint8_t[pkt->data.frame.sz - index_sz];
      memcpy(modified_buf_, pkt->data.frame.buf, pkt->data.frame.sz - index_sz);
      modified_pkt_ = *pkt;
      modified_pkt_.data.frame.buf = modified_buf_;
      modified_pkt_.data.frame.sz -= index_sz;

      sf_count_++;
      last_sf_pts_ = pkt->data.frame.pts;
      return &modified_pkt_;
    }

    // Make sure we do a few frames after the last SF
    abort_ |=
        sf_count_ > sf_count_max_ && pkt->data.frame.pts - last_sf_pts_ >= 5;
    return pkt;
  }

  int is_vp10_style_superframe_;
  int sf_count_;
  int sf_count_max_;
  aom_codec_cx_pkt_t modified_pkt_;
  uint8_t *modified_buf_;
  aom_codec_pts_t last_sf_pts_;
};

TEST_P(SuperframeTest, TestSuperframeIndexIsOptional) {
  sf_count_max_ = 0;  // early exit on successful test.
  cfg_.g_lag_in_frames = 25;

  ::libaom_test::I420VideoSource video("hantro_collage_w352h288.yuv", 352, 288,
                                       30, 1, 0, 40);
  ASSERT_NO_FATAL_FAILURE(RunLoop(&video));
  EXPECT_EQ(sf_count_, 1);
}

VP10_INSTANTIATE_TEST_CASE(
    SuperframeTest,
    ::testing::Combine(::testing::Values(::libaom_test::kTwoPassGood),
                       ::testing::Values(CONFIG_MISC_FIXES)));
}  // namespace
