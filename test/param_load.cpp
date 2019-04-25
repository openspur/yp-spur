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

#include <stdio.h>
#include <stdlib.h>

#include <stdexcept>

#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus

#include <param.h>
#include <ypparam.h>

#ifdef __cplusplus
}
#endif  // __cplusplus

extern Parameters g_param;

class ParamLoadTest : public ::testing::Test
{
protected:
  FILE *fp;

  virtual void SetUp()
  {
    fp = tmpfile();
    if (fp == NULL)
      throw std::runtime_error("Failed to create tmpfile");
  }
};

bool skipKeys(const int key)
{
  if (static_cast<YPSpur_param>(key) == YP_PARAM_VERSION ||
      static_cast<YPSpur_param>(key) == YP_PARAM_ENCODER_DENOMINATOR ||
      static_cast<YPSpur_param>(key) == YP_PARAM_RADIUS_R ||
      static_cast<YPSpur_param>(key) == YP_PARAM_RADIUS_L)
    return true;
  return false;
}

TEST_F(ParamLoadTest, LeftRightCommon)
{
  char param_names[YP_PARAM_NUM][32] = YP_PARAM_NAME;

  fprintf(fp, "VERSION: %f\n", YP_PARAM_SUPPORTED_VERSION);
  for (int i = 0; i < YP_PARAM_NUM; i++)
  {
    if (param_names[i][0] == '_' || skipKeys(i))
      continue;
    fprintf(fp, "%s: %f\n", param_names[i], i * 0.1 + 10.0);
  }
  fflush(fp);
  rewind(fp);
  ASSERT_GT(set_paramptr(fp), 0);
  ASSERT_EQ(g_param.num_motor_enable, 2);

  for (int i = 0; i < YP_PARAM_NUM; i++)
  {
    if (param_names[i][0] == '_' || skipKeys(i))
      continue;
    ASSERT_DOUBLE_EQ(p(static_cast<YPSpur_param>(i), MOTOR_RIGHT), i * 0.1 + 10.0);
    ASSERT_DOUBLE_EQ(p(static_cast<YPSpur_param>(i), MOTOR_LEFT), i * 0.1 + 10.0);
  }
}

TEST_F(ParamLoadTest, LeftRightDifferent)
{
  char param_names[YP_PARAM_NUM][32] = YP_PARAM_NAME;

  fprintf(fp, "VERSION: %f\n", YP_PARAM_SUPPORTED_VERSION);
  for (int i = 0; i < YP_PARAM_NUM; i++)
  {
    if (param_names[i][0] == '_' || skipKeys(i))
      continue;
    fprintf(fp, "%s[0]: %f\n", param_names[i], i * 0.1 + 10.0);
    fprintf(fp, "%s[1]: %f\n", param_names[i], i * 0.1 + 100.0);
  }
  fflush(fp);
  rewind(fp);
  ASSERT_GT(set_paramptr(fp), 0);
  ASSERT_EQ(g_param.num_motor_enable, 2);

  for (int i = 0; i < YP_PARAM_NUM; i++)
  {
    if (param_names[i][0] == '_' || skipKeys(i))
      continue;
    ASSERT_DOUBLE_EQ(p(static_cast<YPSpur_param>(i), MOTOR_RIGHT), i * 0.1 + 10.0);
    ASSERT_DOUBLE_EQ(p(static_cast<YPSpur_param>(i), MOTOR_LEFT), i * 0.1 + 100.0);
  }
}

TEST_F(ParamLoadTest, NoNewline)
{
  char param_names[YP_PARAM_NUM][32] = YP_PARAM_NAME;

  fprintf(fp, "VERSION: %f", YP_PARAM_SUPPORTED_VERSION);
  for (int i = 0; i < YP_PARAM_NUM; i++)
  {
    if (param_names[i][0] == '_' || skipKeys(i))
      continue;
    fprintf(fp, "\n%s: %f", param_names[i], i * 0.1 + 10.0);
  }
  fflush(fp);
  rewind(fp);
  ASSERT_GT(set_paramptr(fp), 0);
  ASSERT_EQ(g_param.num_motor_enable, 2);

  for (int i = 0; i < YP_PARAM_NUM; i++)
  {
    if (param_names[i][0] == '_' || skipKeys(i))
      continue;
    ASSERT_DOUBLE_EQ(p(static_cast<YPSpur_param>(i), MOTOR_RIGHT), i * 0.1 + 10.0);
    ASSERT_DOUBLE_EQ(p(static_cast<YPSpur_param>(i), MOTOR_LEFT), i * 0.1 + 10.0);
  }
}

TEST_F(ParamLoadTest, EncoderDenominatorUnsupported)
{
  char param_names[YP_PARAM_NUM][32] = YP_PARAM_NAME;

  fprintf(fp, "VERSION: 4.0\n");
  fprintf(fp, "ENCODER_DENOMINATOR: 5.0\n");
  for (int i = 0; i < YP_PARAM_NUM; i++)
  {
    if (param_names[i][0] == '_' || skipKeys(i))
      continue;
    fprintf(fp, "%s: %f\n", param_names[i], i * 0.1 + 10.0);
  }
  fflush(fp);
  rewind(fp);
  ASSERT_EQ(set_paramptr(fp), 0);
}

TEST_F(ParamLoadTest, EncoderDenominatorSupported)
{
  char param_names[YP_PARAM_NUM][32] = YP_PARAM_NAME;

  fprintf(fp, "VERSION: %f\n", YP_PARAM_SUPPORTED_VERSION);
  fprintf(fp, "ENCODER_DENOMINATOR: 5.0\n");
  for (int i = 0; i < YP_PARAM_NUM; i++)
  {
    if (param_names[i][0] == '_' || skipKeys(i))
      continue;
    fprintf(fp, "%s: %f\n", param_names[i], i * 0.1 + 10.0);
  }
  fflush(fp);
  rewind(fp);
  ASSERT_GT(set_paramptr(fp), 0);
  ASSERT_EQ(g_param.num_motor_enable, 2);

  ASSERT_DOUBLE_EQ(p(static_cast<YPSpur_param>(YP_PARAM_ENCODER_DENOMINATOR), MOTOR_RIGHT), 5.0);
  ASSERT_DOUBLE_EQ(p(static_cast<YPSpur_param>(YP_PARAM_ENCODER_DENOMINATOR), MOTOR_LEFT), 5.0);
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
