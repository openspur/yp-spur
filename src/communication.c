// Copyright (c) 2010-2016 The YP-Spur Authors, except where otherwise indicated.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <math.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif  // HAVE_CONFIG_H

/* yp-spur用 */
#include <communication.h>

/**
 * @brief エンコード
 */
int encode(const unsigned char *src, int len, unsigned char *dst, int buf_max)
{
  int pos, s_pos, w_pos;
  unsigned short b;
  pos = 0;    // read position
  w_pos = 0;  // write_position
  s_pos = 0;
  b = 0;

  while (pos < len || s_pos >= 6)
  {
    if (s_pos >= 6)
    {
      dst[w_pos] = ((b >> 10) & 0x3f) + 0x40;
      w_pos++;
      if (w_pos >= buf_max)
        return (-1);
      b = b << 6;
      s_pos -= 6;
    }
    else
    {
      b |= src[pos] << (8 - s_pos);
      s_pos += 8;
      pos++;
      if (pos >= len)
        s_pos += 4;  // 最後
    }
  }

  if (w_pos >= buf_max)
    return (-1);

  return w_pos;
}

/**
 * @brief デコード
 * @param src[in] デコードする文字列
 * @param len[in] デコードする文字列の長さ
 * @param dst[out] デコード後のデータ
 * @param buf_max[in] デコード後のデータバッファのサイズ
 * @return デコード後のバイト数
 */
int decode(const unsigned char *src, int len, unsigned char *dst, int buf_max)
{
  unsigned short dat, b;
  int pos, s_pos, w_pos;
  int rerr;
  pos = 0;    // read position
  w_pos = 0;  // write_position
  s_pos = 0;  // shift position
  rerr = 0;
  dat = 0;
  b = 0;
  while (pos < len)
  {
    if (src[pos] >= 0x40)
      b = src[pos] - 0x40;
    else
      rerr++;

    dat |= (b << (10 - s_pos));
    s_pos += 6;
    if (s_pos >= 8)
    {
      dst[w_pos] = (dat >> 8);
      w_pos++;
      if (w_pos >= buf_max)
        return 0;
      s_pos -= 8;
      dat = dat << 8;
    }
    pos++;
  }

  if (rerr)
    return -rerr;
  return w_pos;
}
