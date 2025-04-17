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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif  // HAVE_CONFIG_H

#include <ypspur.h>
#include <utility.h>

int main(int argc, char** argv)
{
  hook_pre_global();
  if (argc > 1)
  {
    char ip[64];
    char* p;
    int port;

    strncpy(ip, argv[1], 64);
    p = strchr(ip, ':');
    if (p == NULL)
    {
      fprintf(stderr, "USAGE: %s ip:port\n", argv[0]);
      return -1;
    }
    *p = 0;
    port = atoi(p + 1);
    Spur_init_socket(ip, port);
  }
  else
  {
    if (Spur_init() < 0)
    {
      fprintf(stderr, "ERROR: ypspur-coordinator stopped.\n");
      return -1;
    }
  }

  Spur_free();

  return 0;
}
