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

#ifndef YPSPUR_IPCOMMUNICATION_H
#define YPSPUR_IPCOMMUNICATION_H

#include <ypspur/msq.win32.h>
#include <ypspur/ypparam.h>

int ipcmd_send_msq(struct ipcmd_t* ipcmd, YPSpur_msg* data);
int ipcmd_recv_msq(struct ipcmd_t* ipcmd, YPSpur_msg* data);
void ipcmd_flush_msq(struct ipcmd_t* ipcmd);
int ipcmd_send_tcp(struct ipcmd_t* ipcmd, YPSpur_msg* data);
int ipcmd_recv_tcp(struct ipcmd_t* ipcmd, YPSpur_msg* data);
void ipcmd_flush_tcp(struct ipcmd_t* ipcmd);
int ipcmd_send(struct ipcmd_t* ipcmd, YPSpur_msg* data);
int ipcmd_recv(struct ipcmd_t* ipcmd, YPSpur_msg* data);
void ipcmd_flush(struct ipcmd_t* ipcmd);

int ipcmd_open_msq(struct ipcmd_t* ipcmd, int key, int creat);
int ipcmd_open_tcp(struct ipcmd_t* ipcmd, char* host, int port);
void ipcmd_close(struct ipcmd_t* ipcmd);

#endif  // YPSPUR_IPCOMMUNICATION_H
