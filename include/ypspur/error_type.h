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

#ifndef YPSPUR_ERROR_TYPE_H
#define YPSPUR_ERROR_TYPE_H

#ifdef __cplusplus
extern "C"
{
#endif  // __cplusplus

typedef enum
{
  ERROR_NONE = 0,
  ERROR_LOW_VOLTAGE = 0x0001,
  ERROR_HALL_SEQ = 0x0002,
  ERROR_HALL_ENC = 0x0004,
  ERROR_WATCHDOG = 0x0008,
  ERROR_EEPROM = 0x0010,
  ERROR_INTERNAL = 0x0020,
} YPSpur_shvel_error_state;

#ifdef __cplusplus
}
#endif  // __cplusplus

#endif  // YPSPUR_ERROR_TYPE_H

