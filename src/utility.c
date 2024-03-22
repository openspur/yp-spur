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
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
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

#include <utility.h>

#ifdef __WIN32
#include <windows.h>
#endif  // __WIN32

/* get time stamp */
double get_time(void)
{
  struct timeval current;

  gettimeofday(&current, NULL);

  return current.tv_sec + current.tv_usec / 1000000.0;
}

void yp_usleep(int usec)
{
#if defined(HAVE_NANOSLEEP)
  // nanosleepが利用可能
  struct timespec request;
  request.tv_sec = usec / 1000000;
  request.tv_nsec = (usec - request.tv_sec * 1000000) * 1000;

  nanosleep(&request, NULL);
#elif defined(__MINGW32__)
  // MinGWのusleepには1ms以下切り捨ての問題があるためWindows環境ではWinAPIのSleepを使う
  // 1ms以下は切り上げ
  Sleep((usec + 999) / 1000);
#else
  // 古いシステム用
  usleep(usec);
#endif  // defined(HAVE_NANOSLEEP)
}

void hook_pre_global()
{
  // Windows環境で標準出力がバッファリングされないように設定
  setvbuf(stdout, 0, _IONBF, 0);
  setvbuf(stderr, 0, _IONBF, 0);
}

#if !defined(HAVE_STRTOK_R)
#ifndef strtok_r

/*
 * public domain strtok_r() by Charlie Gordon
 *   from comp.lang.c  9/14/2007
 *      http://groups.google.com/group/comp.lang.c/msg/2ab1ecbb86646684
 *     (Declaration that it's public domain):
 *      http://groups.google.com/group/comp.lang.c/msg/7c7b39328fefab9c
 */

char* strtok_r(char* str, const char* delim, char** nextp)
{
  char* ret;

  if (str == NULL)
    str = *nextp;

  str += strspn(str, delim);

  if (*str == '\0')
    return NULL;

  ret = str;
  str += strcspn(str, delim);

  if (*str)
    *str++ = '\0';

  *nextp = str;

  return ret;
}

#endif  // strtok_r
#endif  // !defined(HAVE_STRTOK_R)
