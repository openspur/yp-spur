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
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <fcntl.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <time.h>

// yp-spur用
#include <ypspur/odometry.h>
#include <ypspur/serial.h>
#include <ypspur/utility.h>

// ライブラリ用
#include <ypspur.h>

void process_addata(int* ad, const unsigned char* buf, const int len)
{
  for (int j = 0; j < len; j += 2)
  {
    Short_2Char data;
    data.byte[1] = buf[j];
    data.byte[0] = buf[j + 1];
    ad[((unsigned short)data.integer) >> 12] = (unsigned short)data.integer & 0x0FFF;
    // printf("%d=%d\n",((unsigned short)data.integer) >> 12,(unsigned short)data.integer & 0x0FFF);
  }
}

int admask_receive(char* buf, int len, double receive_time, void* data)
{
  buf[len] = 0;
  strcat((char*)data, buf);
  if (strstr((char*)data, "00P\n\n"))
    return -1;
  if (strstr((char*)data, "\n\n"))
    return -2;
  return 0;
}

int set_admask(unsigned char mask)
{
  // Send & Recive Buffer
  char buf[2048];
  int i;

  strcpy(buf, "\n\n\n\n");
  serial_write(buf, strlen(buf));
  yp_usleep(50000);
  serial_flush_in();
  yp_usleep(50000);
  strcpy(buf, "ADMASK");
  for (i = 0; i < 8; i++)
  {
    if (mask & 0x80)
    {
      strcat(buf, "1");
    }
    else
    {
      strcat(buf, "0");
    }
    mask = mask << 1;
  }
  strcat(buf, "\n");
  serial_write(buf, strlen(buf));

  buf[0] = 0;
  if (serial_recieve(admask_receive, buf) == -1)
    return 1;

  return 0;
}

int set_diomask(unsigned char enable)
{
  // Send & Recive Buffer
  char buf[2048];

  strcpy(buf, "\n\n\n\n");
  serial_write(buf, strlen(buf));
  yp_usleep(50000);
  serial_flush_in();
  yp_usleep(50000);
  strcpy(buf, "GETIO");
  if (enable)
  {
    strcat(buf, "1");
  }
  else
  {
    strcat(buf, "0");
  }
  strcat(buf, "\n");
  serial_write(buf, strlen(buf));

  buf[0] = 0;
  if (serial_recieve(admask_receive, buf) == -1)
    return 1;

  return 0;
}
