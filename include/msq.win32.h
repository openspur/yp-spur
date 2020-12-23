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

#ifndef MSQ_WIN32_H
#define MSQ_WIN32_H

#include <sys/types.h>

#ifdef __MINGW32__
// Windows用

#include <windows.h>

#include <unistd.h>

#define IPC_CREAT 01000
#define IPC_EXCL 02000
#define IPC_NOWAIT 04000
#define IPC_RMID 0
#define IPC_SET 1
#define IPC_STAT 2

#ifndef __key_t_defined
typedef int key_t;
#define __key_t_defined
#endif  // ! __key_t_defined

#ifndef __uid_t_defined
typedef int uid_t;
#define __uid_t_defined
#endif  // ! __uid_t_defined

#ifndef __gid_t_defined
typedef int gid_t;
#define __gid_t_defined
#endif  // ! __gid_t_defined

#ifndef __pid_t_defined
#ifndef _PID_T_
typedef int pid_t;
#define __pid_t_defined
#define _PID_T_
#endif  // ! _PID_T_
#endif  // ! __pid_t_defined

typedef unsigned long int msgqnum_t;
typedef unsigned long int msglen_t;

struct ipc_perm
{
  key_t __key;             /* Key.  */
  uid_t uid;               /* Owner's user ID.  */
  gid_t gid;               /* Owner's group ID.  */
  uid_t cuid;              /* Creator's user ID.  */
  gid_t cgid;              /* Creator's group ID.  */
  unsigned short int mode; /* Read/write permission.  */
  unsigned short int __pad1;
  unsigned short int __seq; /* Sequence number.  */
  unsigned short int __pad2;
  unsigned long int __unused1;
  unsigned long int __unused2;
};
struct msqid_ds
{
  struct ipc_perm msg_perm; /* structure describing operation permission */
  time_t msg_stime;         /* time of last msgsnd command */
  unsigned long int __unused1;
  time_t msg_rtime; /* time of last msgrcv command */
  unsigned long int __unused2;
  time_t msg_ctime; /* time of last change */
  unsigned long int __unused3;
  unsigned long int __msg_cbytes; /* current number of bytes on queue */
  msgqnum_t msg_qnum;             /* number of messages currently on queue */
  msglen_t msg_qbytes;            /* max number of bytes allowed on queue */
  pid_t msg_lspid;                /* pid of last msgsnd() */
  pid_t msg_lrpid;                /* pid of last msgrcv() */
  unsigned long int __unused4;
  unsigned long int __unused5;
};

int msgget(key_t key, int msgflg);
int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg);
ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg);
int msgctl(int msqid, int cmd, struct msqid_ds *buf);

#else
// Linux用

#include <sys/ipc.h>
#include <sys/msg.h>

#endif  // __MINGW32__

#endif  // MSQ_WIN32_H
