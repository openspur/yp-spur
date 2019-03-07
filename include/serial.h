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

#ifndef SERIAL_H
#define SERIAL_H

#define ENABLE 1
#define DISABLE 0

#define DEFAULT_BAUDRATE 38400

/* for measurement time estimation */
extern double SER_BAUDRATE;  /// (Byte/sec) シリアルの通信速度
#define SER_INTERVAL 0.0050
// #define SER_BYTES 13.0
// #define SER_TIME_BYTE (11.0/SER_BAUDRATE)

int serial_tryconnect(char *device_name);
int serial_connect(char *device_name);
int serial_change_baudrate(int baud);

/*----------------PBS_close------------------*/
int serial_close(void);
int serial_write(char *buf, int len);
int serial_recieve(int (*serial_event)(char *, int, double, void *), void *data);
void serial_flush_in(void);
void serial_flush_out(void);
int encode_write(char *data, int len);

#endif  // SERIAL_H
