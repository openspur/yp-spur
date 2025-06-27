// Copyright (c) 2010-2025 The YP-Spur Authors, except where otherwise indicated.
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

#ifndef YPSPUR_YPSPUR_COORDINATOR_H
#define YPSPUR_YPSPUR_COORDINATOR_H

#include <ypspur/ypparam.h>

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

int ypsc_main(int argc, char* argv[]);
int ypsc_command(const YPSpur_msg* msg, YPSpur_msg* res_msg);

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // YPSPUR_YPSPUR_COORDINATOR_H
