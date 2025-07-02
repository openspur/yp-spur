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

#include <setjmp.h>
#include <signal.h>
#include <stdio.h>

#ifdef __MINGW32__
#include <windows.h>
#endif  // __MINGW32__

#include <ypspur/param.h>
#include <ypspur/serial.h>
#include <ypspur/ssm_spur_handler.h>
#include <ypspur/yprintf.h>
#include <ypspur/ypspur-coordinator.h>

#if HAVE_SIGLONGJMP
sigjmp_buf ctrlc_capture;
#elif HAVE_LONGJMP
jmp_buf ctrlc_capture;
#endif  // HAVE_SIGLONGJMP

#if defined(__MINGW32__)
BOOL WINAPI win32_ctrlc_handler(DWORD type)
{
  fprintf(stderr, "\n");
#ifdef HAVE_SSM
  // SSM終了処理
  if (!option(OPTION_WITHOUT_SSM))
  {
    end_ypspurSSM();
  }
#endif  // HAVE_SSM
  ypsc_kill();

  return TRUE;
}
#else
void emergency(int sig)
{
  fprintf(stderr, "\n");
#if HAVE_SIGLONGJMP
  siglongjmp(ctrlc_capture, 1);
#elif HAVE_LONGJMP
  longjmp(ctrlc_capture, 1);
#else
#ifdef HAVE_SSM
  // SSM終了処理
  if (!option(OPTION_WITHOUT_SSM))
  {
    end_ypspurSSM();
  }
#endif  // HAVE_SSM
  ypsc_kill();

  exit(0);
#endif  // HAVE_SIGLONGJMP
}
#endif  // defined(__MINGW32__)

void enable_ctrlc_handling(const int enable)
{
#if defined(__MINGW32__)
  if (enable)
  {
    if (!SetConsoleCtrlHandler(win32_ctrlc_handler, TRUE))
    {
      yprintf(OUTPUT_LV_ERROR, "Error: Win32 Ctrl+C handler registration failed.\n");
    }
  }
  else
  {
    if (!SetConsoleCtrlHandler(NULL, FALSE))
    {
      yprintf(OUTPUT_LV_ERROR, "Error: Win32 Ctrl+C handler restoration failed.\n");
    }
  }
#else
  if (enable)
  {
    signal(SIGINT, emergency);
  }
  else
  {
    signal(SIGINT, SIG_DFL);
  }
#endif  // defined(__MINGW32__)
}

int ctrlc_setjmp()
{
#if HAVE_SIGLONGJMP
  return sigsetjmp(ctrlc_capture, 1);
#elif HAVE_LONGJMP
  return setjmp(ctrlc_capture);
#else
  return 0;
#endif  // HAVE_SIGLONGJMP
}

#if HAVE_SIGLONGJMP
sigjmp_buf* get_ctrlc_jmp_buf_ptr()
{
  return &ctrlc_capture;
}
#elif HAVE_LONGJMP
jmp_buf* get_ctrlc_jmp_buf_ptr()
{
  return &ctrlc_capture;
}
#endif  // HAVE_SIGLONGJMP
