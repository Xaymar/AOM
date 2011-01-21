/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */


#include <math.h>
#include "vpx_ports/config.h"
void vp8_short_fdct4x4_c(short *input, short *output, int pitch)
{
    int i;
    int a1, b1, c1, d1;
    short *ip = input;
    short *op = output;

    for (i = 0; i < 4; i++)
    {
#if CONFIG_EXTEND_QRANGE
        a1 = ((ip[0] + ip[3])<<5);
        b1 = ((ip[1] + ip[2])<<5);
        c1 = ((ip[1] - ip[2])<<5);
        d1 = ((ip[0] - ip[3])<<5);
#else
        a1 = ((ip[0] + ip[3])<<3);
        b1 = ((ip[1] + ip[2])<<3);
        c1 = ((ip[1] - ip[2])<<3);
        d1 = ((ip[0] - ip[3])<<3);
#endif
        op[0] = a1 + b1;
        op[2] = a1 - b1;

        op[1] = (c1 * 2217 + d1 * 5352 +  14500)>>12;
        op[3] = (d1 * 2217 - c1 * 5352 +   7500)>>12;

        ip += pitch / 2;
        op += 4;

    }
    ip = output;
    op = output;
    for (i = 0; i < 4; i++)
    {
        a1 = ip[0] + ip[12];
        b1 = ip[4] + ip[8];
        c1 = ip[4] - ip[8];
        d1 = ip[0] - ip[12];

        op[0]  = ( a1 + b1 + 7)>>4;
        op[8]  = ( a1 - b1 + 7)>>4;

        op[4]  =((c1 * 2217 + d1 * 5352 +  12000)>>16) + (d1!=0);
        op[12] = (d1 * 2217 - c1 * 5352 +  51000)>>16;

        ip++;
        op++;
    }
}

void vp8_short_fdct8x4_c(short *input, short *output, int pitch)
{
    vp8_short_fdct4x4_c(input,   output,    pitch);
    vp8_short_fdct4x4_c(input + 4, output + 16, pitch);
}

void vp8_short_walsh4x4_c(short *input, short *output, int pitch)
{
    int i;
    int a1, b1, c1, d1;
    int a2, b2, c2, d2;
    short *ip = input;
    short *op = output;


    for (i = 0; i < 4; i++)
    {
#if !CONFIG_EXTEND_QRANGE
        a1 = ((ip[0] + ip[2])<<2);
        d1 = ((ip[1] + ip[3])<<2);
        c1 = ((ip[1] - ip[3])<<2);
        b1 = ((ip[0] - ip[2])<<2);

        op[0] = a1 + d1 + (a1!=0);
#else
        a1 = ((ip[0] + ip[2]));
        d1 = ((ip[1] + ip[3]));
        c1 = ((ip[1] - ip[3]));
        b1 = ((ip[0] - ip[2]));


        op[0] = a1 + d1;
#endif
        op[1] = b1 + c1;
        op[2] = b1 - c1;
        op[3] = a1 - d1;
        ip += pitch / 2;
        op += 4;
    }

    ip = output;
    op = output;

    for (i = 0; i < 4; i++)
    {
        a1 = ip[0] + ip[8];
        d1 = ip[4] + ip[12];
        c1 = ip[4] - ip[12];
        b1 = ip[0] - ip[8];

        a2 = a1 + d1;
        b2 = b1 + c1;
        c2 = b1 - c1;
        d2 = a1 - d1;

        a2 += a2<0;
        b2 += b2<0;
        c2 += c2<0;
        d2 += d2<0;

#if !CONFIG_EXTEND_QRANGE
        op[0] = (a2+3) >> 3;
        op[4] = (b2+3) >> 3;
        op[8] = (c2+3) >> 3;
        op[12]= (d2+3) >> 3;
#else
        op[0] = (a2+1) >> 2;
        op[4] = (b2+1) >> 2;
        op[8] = (c2+1) >> 2;
        op[12]= (d2+1) >> 2;
#endif
        ip++;
        op++;
    }
}
