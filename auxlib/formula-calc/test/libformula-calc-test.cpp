// Simple mathematical formula processing library
//
// Copyright 2017 Atsushi Watanabe, All rights reserved.
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
#include <math.h>

#include <gtest/gtest.h>

#include <formula-calc.h>

TEST(FormulaCalcTest, testFormulaCalculation)
{
  int i;

  for (i = 0; i < 5; i++)
  {
    struct rpf_t *rpf;
    struct rpf_t *rpf2;
    double test = 0;
    struct variables_t variable[2] =
        {
          { "TEST", &test },
          { NULL, NULL }
        };
    float ret;

    ASSERT_EQ(formula("1+1", &rpf, variable), 1);
    ret = formula_eval(rpf);
    EXPECT_EQ(ret, 2.0);
    rpf2 = formula_optimize(rpf);
    ret = formula_eval(rpf2);
    EXPECT_EQ(ret, 2.0);
    formula_free(rpf);
    formula_free(rpf2);

    ASSERT_EQ(formula("abc+1", &rpf, variable), 0);

    ASSERT_EQ(formula("1+2+3+4/8+2*3", &rpf, variable), 1);
    ret = formula_eval(rpf);
    EXPECT_EQ(ret, 12.5);
    rpf2 = formula_optimize(rpf);
    ret = formula_eval(rpf2);
    EXPECT_EQ(ret, 12.5);
    formula_free(rpf);
    formula_free(rpf2);

    ASSERT_EQ(formula("cos(3.1416)+1", &rpf, variable), 1);
    ret = formula_eval(rpf);
    EXPECT_LT(fabs(ret), 1e-3);
    rpf2 = formula_optimize(rpf);
    ret = formula_eval(rpf2);
    EXPECT_LT(fabs(ret), 1e-3);
    formula_free(rpf);
    formula_free(rpf2);

    ASSERT_EQ(formula("(TEST+1)*2-1", &rpf, variable), 1);

    for (test = 0; test < 4; test++)
    {
      ret = formula_eval(rpf);
      EXPECT_EQ(ret, (test + 1) * 2.0 - 1.0);
    }

    rpf2 = formula_optimize(rpf);

    for (test = 0; test < 4; test++)
    {
      ret = formula_eval(rpf2);
      EXPECT_EQ(ret, (test + 1) * 2.0 - 1.0);
    }
    formula_free(rpf);
    formula_free(rpf2);

    ASSERT_EQ(formula("(TEST=5)*0.1+10.0", &rpf, variable), 1);
    ret = formula_eval(rpf);
    EXPECT_NEAR(ret, 10.5, 1e-6);
    EXPECT_NEAR(test, 5.0, 1e-6);
    formula_free(rpf);

    ASSERT_EQ(formula("TEST=(10.0+5*0.1)", &rpf, variable), 1);
    ret = formula_eval(rpf);
    EXPECT_NEAR(ret, 10.5, 1e-6);
    EXPECT_NEAR(test, 10.5, 1e-6);
    formula_free(rpf);
  }
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
