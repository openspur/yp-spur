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

#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif  // HAVE_CONFIG_H

#include <ipcommunication.h>
#include <utility.h>

#if defined(__MINGW32__)
#include <windows.h>
#include <tchar.h>

HANDLE g_shm = NULL;
void *g_shm_data;
HANDLE g_mutex = NULL;

int msgget(key_t key, int msgflg)
{
  TCHAR name[512];

  _stprintf(name, "MessageQueueShm%d", (int)key);
  g_shm = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 8192, name);

  g_shm_data = (void *)MapViewOfFile(g_shm, FILE_MAP_ALL_ACCESS, 0, 0, 8192);

  _stprintf(name, "MessageQueueMutex%d", (int)key);
  if (msgflg & IPC_CREAT)
  {
    g_mutex = CreateMutex(NULL, FALSE, name);
    *((int32_t *)g_shm_data) = 0;
  }
  else
  {
    g_mutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, name);
  }
  if (g_mutex == NULL)
  {
    return -1;
  }

  return 1;
}

int msgsnd(int msqid, const void *msgp, size_t msgsz, int msgflg)
{
  char *pos;

  if (WaitForSingleObject(g_mutex, INFINITE) == WAIT_FAILED)
  {
    CloseHandle(g_mutex);
    CloseHandle(g_shm);
    return -1;
  }

  msgsz += sizeof(long);  // add size of msg_type

  pos = (char *)g_shm_data;
  while (1)
  {
    int32_t size;

    size = *(int32_t *)pos;
    if (size == 0)
      break;

    pos += sizeof(int32_t) + size;
  }

  if ((char *)pos + msgsz + sizeof(int32_t) - (char *)g_shm_data > 8192)
    return 0;

  *((int32_t *)pos) = (int32_t)msgsz;
  pos += sizeof(int32_t);
  memcpy(pos, msgp, msgsz);
  pos += msgsz;
  *((int32_t *)pos) = 0;

  ReleaseMutex(g_mutex);

  return 1;
}

ssize_t msgrcv(int msqid, void *msgp, size_t msgsz, long msgtyp, int msgflg)
{
  char *pos;
  char *pos_before;
  char *pos_target;
  int32_t readsize;

  msgsz += sizeof(long);  // add size of msg_type

  readsize = 0;
  while (1)
  {
    if (WaitForSingleObject(g_mutex, INFINITE) == WAIT_FAILED)
    {
      CloseHandle(g_mutex);
      CloseHandle(g_shm);
      return -1;
    }

    pos = (char *)g_shm_data;
    pos_target = NULL;
    while (1)
    {
      int32_t size;
      int32_t *ptype;

      size = *(int32_t *)pos;
      if (size == 0)
        break;

      pos_before = pos;
      ptype = (int32_t *)(pos_before + sizeof(int32_t));
      if (((*ptype == msgtyp && msgtyp > 0) || (*ptype <= -msgtyp && msgtyp < 0)) && !pos_target)
      {
        pos_target = pos_before;
      }
      pos += sizeof(int32_t) + size;
    }
    if (pos_target)
    {
      char *pos_next;
      int32_t sizeleft;

      if (*((int32_t *)pos_target) < msgsz)
      {
        readsize = *((int32_t *)pos_target);
      }
      else
      {
        readsize = msgsz;
      }
      pos_next = pos_target + sizeof(int32_t) + *((int32_t *)pos_target);
      sizeleft = pos - pos_next + 1;
      memcpy(msgp, pos_target + sizeof(int32_t), readsize);
      memcpy(pos_target, pos_next, sizeleft);
    }

    ReleaseMutex(g_mutex);

    if (!(msgflg & IPC_NOWAIT) && !pos_target)
    {
      yp_usleep(5000);
      continue;
    }
    break;
  }
  if (!pos_target)
    return -1;

  return readsize;
}

int msgctl(int msqid, int cmd, struct msqid_ds *buf)
{
  switch (cmd)
  {
    case IPC_RMID:
      if (g_shm)
        CloseHandle(g_shm);
      if (g_mutex)
        CloseHandle(g_mutex);
      return 1;
      break;
    case IPC_SET:
    case IPC_STAT:
    default:
      break;
  }
  return -1;
}

#endif  // defined(__MINGW32__)
