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

#include <stdarg.h>
#include <stdio.h>
#include <strings.h>
#include <unistd.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif  // HAVE_CONFIG_H

#include <param.h>

void yprintf(ParamOutputLv level, const char* format, ...)
{
  va_list ap;

  if (output_lv() < level)
    return;

#if !defined(__MINGW32__)
  if (isatty(2))
  {
    switch (level)
    {
      case OUTPUT_LV_ERROR:
        fputs("\x1B[0;31m", stderr);
        break;
      case OUTPUT_LV_WARNING:
        fputs("\x1B[0;33m", stderr);
        break;
      default:
        break;
    }
  }
#endif  // !defined(__MINGW32__)

  va_start(ap, format);
  vfprintf(stderr, format, ap);
  va_end(ap);

#if !defined(__MINGW32__)
  if (isatty(2))
    fputs("\x1B[0m", stderr);
#endif  // !defined(__MINGW32__)

  fflush(stderr);
}

void ansi_clear_line(ParamOutputLv level)
{
  if (output_lv() < level)
    return;

#if !defined(__MINGW32__)
  if (isatty(2))
    fprintf(stderr, "\x1B[2K\r");
  else
    fputs("\n", stderr);
#else
  fputs("\n", stderr);
#endif  // !defined(__MINGW32__)
}
