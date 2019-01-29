// Simple mathematical formula processing library
//  Written by Atsushi Watanabe
//  Intelligent Robot Laboratory, University of Tsukuba
//
// Copyright 2011 Atsushi Watanabe, All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// * Redistributions of source code must retain the above copyright notice, this
//   list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
// EVENT SHALL THE FREEBSD PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <stdio.h>
#include <stdlib.h>

#include <formula-calc.h>

int main(int argc, char *argv[])
{
  struct rpf_t *rpf;
  struct rpf_t *rpf2;
  double test = 0;
  struct variables_t variable[8] =
      {
        { "TEST", &test },
        { NULL, NULL }
      };

  if (argc < 2)
  {
    printf("Usage: %s <formula>\n", argv[0]);
    return 0;
  }

  if (!formula(argv[1], &rpf, variable))
  {
    printf("Invalid formula\n");
  }
  else
  {
    float d;

    printf("Given formula: %s\n", argv[1]);

    printf("Reverse polish: ");
    formula_print(stdout, rpf);
    printf("\n");

    rpf2 = formula_optimize(rpf);
    printf("Optimized reverse polish: ");
    formula_print(stdout, rpf2);
    printf("\n");

    d = formula_eval(rpf2);
    printf("Result: %f %f\n", d, test);

    formula_free(rpf);
    formula_free(rpf2);
  }

  return 1;
}
