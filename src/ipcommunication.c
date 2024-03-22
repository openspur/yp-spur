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

#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <unistd.h>

#if defined(__MINGW32__)
#include <winsock2.h>
#include <windows.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif  // defined(__MINGW32__)

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif  // HAVE_CONFIG_H

#include <ypparam.h>
#include <msq.win32.h>
#include <ipcommunication.h>
#include <utility.h>
#include <yprintf.h>

#if defined(__MINGW32__)
#define SOCK_SHUTDOWN_OPTION SD_BOTH
#define SOCK_DATATYPE char*
#define SIZE_TYPE int
#else
#define SOCK_SHUTDOWN_OPTION SHUT_RDWR
#define SOCK_DATATYPE void*
#define SIZE_TYPE unsigned int
#endif  // defined(__MINGW32__)

int ipcmd_open_msq(struct ipcmd_t* ipcmd, int key, int creat)
{
  if (creat)
    creat = IPC_CREAT;
  /* メッセージ・キューのオープン */
  ipcmd->socket = msgget(key, 0666 | creat);

  ipcmd->send = ipcmd_send;
  ipcmd->recv = ipcmd_recv;
  ipcmd->flush = ipcmd_flush;
  if (ipcmd->socket == -1)
  {
    ipcmd->connection_error = 1;
    return -1;
  }

  /* 内部データの初期化 */
  ipcmd->pid = 0x07fff & getpid();
  if (creat)
    ipcmd->pid = YPSPUR_MSG_CMD;
  ipcmd->connection_error = 0;
  ipcmd->send = ipcmd_send_msq;
  ipcmd->recv = ipcmd_recv_msq;
  ipcmd->flush = ipcmd_flush_msq;

  ipcmd->type = IPCMD_MSQ;

  return 1;
}

int ipcmd_send_msq(struct ipcmd_t* ipcmd, YPSpur_msg* data)
{
  size_t len = YPSPUR_MSG_SIZE;

  if (ipcmd == NULL || ipcmd->connection_error)
    return -1;

  if (msgsnd(ipcmd->socket, data, len, 0) < 0)
  {
    ipcmd->connection_error = 1;
    return -1;
  }
  return len;
}

int ipcmd_recv_msq(struct ipcmd_t* ipcmd, YPSpur_msg* data)
{
  int received;
  size_t len = YPSPUR_MSG_SIZE;

  if (ipcmd == NULL || ipcmd->connection_error)
    return -1;

  received = msgrcv(ipcmd->socket, data, len, ipcmd->pid, 0);
  if (received < 0)
  {
    ipcmd->connection_error = 1;
    return -1;
  }
  return received;
}

void ipcmd_flush_msq(struct ipcmd_t* ipcmd)
{
  char dummy[128];

  if (ipcmd == NULL || ipcmd->connection_error)
    return;

  while (1)
  {
    if (msgrcv(ipcmd->socket, &dummy, 128, YPSPUR_MSG_CMD, IPC_NOWAIT) == -1)
    {
      break;
    }
  }
}

int ipcmd_open_tcp(struct ipcmd_t* ipcmd, char* host, int port)
{
  struct sockaddr_in addr;
  int i;

#if HAVE_LIBWS2_32
  WSADATA wsadata;

  if (WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
  {
    return -1;
  }
#endif  // HAVE_LIBWS2_32

  ipcmd->send = ipcmd_send;
  ipcmd->recv = ipcmd_recv;
  ipcmd->flush = ipcmd_flush;
  memset(&addr, 0, sizeof(addr));
  addr.sin_port = htons(port);
  addr.sin_family = AF_INET;

  ipcmd->socket = socket(AF_INET, SOCK_STREAM, 0);
  for (i = 0; i < YPSPUR_MAX_SOCKET; i++)
  {
    ipcmd->clients[i] = -1;
  }

  if (!host)
  {
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind(ipcmd->socket, (struct sockaddr*)&addr, sizeof(addr));

    listen(ipcmd->socket, YPSPUR_MAX_SOCKET);
    ipcmd->tcp_type = IPCMD_TCP_SERVER;
  }
  else
  {
    addr.sin_addr.s_addr = inet_addr(host);

    if (connect(ipcmd->socket, (struct sockaddr*)&addr, sizeof(addr)) == -1)
    {
      return -1;
    }
    ipcmd->tcp_type = IPCMD_TCP_CLIENT;
  }

  ipcmd->type = IPCMD_TCP;
  ipcmd->send = ipcmd_send_tcp;
  ipcmd->recv = ipcmd_recv_tcp;
  ipcmd->flush = ipcmd_flush_tcp;

  ipcmd->connection_error = 0;

  return 1;
}

int ipcmd_send_tcp(struct ipcmd_t* ipcmd, YPSpur_msg* data)
{
  int sock;
  size_t len = sizeof(YPSpur_msg);

  if (ipcmd == NULL || ipcmd->connection_error)
    return -1;

  if (ipcmd->tcp_type == IPCMD_TCP_CLIENT)
  {
    sock = ipcmd->socket;
  }
  else
  {
    sock = ipcmd->clients[(int)data->msg_type];
  }
  if (send(sock, (SOCK_DATATYPE)data, len, 0) < 0)
  {
    if (ipcmd->tcp_type == IPCMD_TCP_CLIENT)
    {
      shutdown(ipcmd->socket, SOCK_SHUTDOWN_OPTION);
      ipcmd->connection_error = 1;
    }
    return -1;
  }

  return len;
}

int ipcmd_recv_tcp(struct ipcmd_t* ipcmd, YPSpur_msg* data)
{
  fd_set fds;
  struct sockaddr_in client;
  int sock;
  int i;
  size_t len = sizeof(YPSpur_msg);

  if (ipcmd == NULL || ipcmd->connection_error)
    return -1;

  do
  {
    int recved;
    while (1)
    {
      SIZE_TYPE addr_size;
      int nfds = 0;

      FD_ZERO(&fds);
      FD_SET(ipcmd->socket, &fds);
      if (nfds < ipcmd->socket)
        nfds = ipcmd->socket;
      for (i = 0; i < YPSPUR_MAX_SOCKET; i++)
      {
        if (ipcmd->clients[i] != -1)
        {
          FD_SET(ipcmd->clients[i], &fds);
          if (nfds < ipcmd->clients[i])
            nfds = ipcmd->clients[i];
        }
      }

      nfds++;
      if (!select(nfds, &fds, NULL, NULL, NULL))
      {
        yprintf(OUTPUT_LV_ERROR, "Couldn't select the socket.\n");
        return -1;
      }
      if (!FD_ISSET(ipcmd->socket, &fds))
      {
        break;
      }
      if (ipcmd->tcp_type == IPCMD_TCP_CLIENT)
      {
        break;
      }

      addr_size = sizeof(client);
      sock = accept(ipcmd->socket, (struct sockaddr*)&client, &addr_size);
      if (sock <= 0)
      {
        yprintf(OUTPUT_LV_ERROR, "Invalid socket.\n");
        return -1;
      }
      for (i = 0; i < YPSPUR_MAX_SOCKET; i++)
      {
        if (ipcmd->clients[i] == -1)
        {
          break;
        }
      }
      if (i == YPSPUR_MAX_SOCKET)
      {
        yprintf(OUTPUT_LV_ERROR, "Too many connection requests.\n");
        return -1;
      }
      ipcmd->clients[i] = sock;
      yprintf(OUTPUT_LV_INFO, "Connection %d accepted from %s.\n", i, inet_ntoa(client.sin_addr));
    }

    recved = -1;
    if (ipcmd->tcp_type == IPCMD_TCP_CLIENT)
    {
      recved = recv(ipcmd->socket, (SOCK_DATATYPE)data, len, 0);
      data->pid = 0;
    }
    else
    {
      for (i = 0; i < YPSPUR_MAX_SOCKET; i++)
      {
        if (FD_ISSET(ipcmd->clients[i], &fds))
        {
          recved = recv(ipcmd->clients[i], (SOCK_DATATYPE)data, len, 0);
          data->pid = i;
          break;
        }
      }
    }
    if (recved <= 0)
    {
      if (ipcmd->tcp_type == IPCMD_TCP_CLIENT)
      {
        yprintf(OUTPUT_LV_INFO, "Connection closed.\n");
        ipcmd->connection_error = 1;
        shutdown(ipcmd->socket, SOCK_SHUTDOWN_OPTION);
        return -1;
      }
      yprintf(OUTPUT_LV_INFO, "Connection %d closed.\n", i);
      ipcmd->clients[i] = -1;
      continue;
    }
  }
  while (0);
  return len;
}

void ipcmd_flush_tcp(struct ipcmd_t* ipcmd)
{
}

void ipcmd_close(struct ipcmd_t* ipcmd)
{
  if (ipcmd == NULL)
    return;
  switch (ipcmd->type)
  {
    case IPCMD_MSQ:
      msgctl(ipcmd->socket, IPC_RMID, NULL);
      break;
    case IPCMD_TCP:
      shutdown(ipcmd->socket, SOCK_SHUTDOWN_OPTION);
#if HAVE_LIBWS2_32
      WSACleanup();
#endif  // HAVE_LIBWS2_32
      break;
  }
  ipcmd->send = ipcmd_send;
  ipcmd->recv = ipcmd_recv;
  ipcmd->flush = ipcmd_flush;
}

int ipcmd_send(struct ipcmd_t* ipcmd, YPSpur_msg* data)
{
  ipcmd->connection_error = 1;
  return -1;
}

int ipcmd_recv(struct ipcmd_t* ipcmd, YPSpur_msg* data)
{
  ipcmd->connection_error = 1;
  return -1;
}

void ipcmd_flush(struct ipcmd_t* ipcmd)
{
}
