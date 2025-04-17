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

#include <math.h>
#include <stdio.h>

#include <cartesian2d.h>

CSptr BS, GL, LC, LC2, LC3, LC4;

int main(int argc, char* argv[])
{
  double x, y, theta;

  BS = CS_add(0, 0, 0, 0);
  GL = CS_add(BS, 1, 1, 0);
  LC = CS_add(GL, 1, 1, 0);
  LC2 = CS_add(GL, 2, 2, 0);
  LC3 = CS_add(BS, -3, -3, 0);
  LC4 = CS_add(LC3, -3, -3, 0);

  x = 2;
  y = 2;
  theta = 2;
  CS_recursive_trans(GL, BS, &x, &y, &theta);
  printf("%f %f %f\n", x, y, theta);

  x = 2;
  y = 2;
  theta = 2;
  CS_recursive_trans(LC, GL, &x, &y, &theta);
  printf("%f %f %f\n", x, y, theta);

  x = 2;
  y = 2;
  theta = 2;
  CS_recursive_trans(LC2, GL, &x, &y, &theta);
  printf("%f %f %f\n", x, y, theta);

  x = 2;
  y = 2;
  theta = 2;
  CS_recursive_trans(LC4, LC, &x, &y, &theta);
  printf("%f %f %f\n", x, y, theta);

  return 0;
}
