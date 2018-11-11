#include <stdio.h>
#include <stdlib.h>

#include <stdexcept>

#include <gtest/gtest.h>

#ifdef __cplusplus
extern "C" {
#endif

#include <param.h>
#include <ypparam.h>

#ifdef __cplusplus
}
#endif

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
      static_cast<YPSpur_param>(key) == YP_PARAM_RADIUS_R ||
      static_cast<YPSpur_param>(key) == YP_PARAM_RADIUS_L)
    return true;
  return false;
}

TEST_F(ParamLoadTest, LeftRightCommon)
{
  char param_names[YP_PARAM_NUM][32] = YP_PARAM_NAME;

  fprintf(fp, "VERSION: %f\n", YP_PARAM_REQUIRED_VERSION);
  for (int i = 0; i < YP_PARAM_NUM; i++)
  {
    if (param_names[i][0] == '_' || skipKeys(i))
      continue;
    fprintf(fp, "%s: %f\n", param_names[i], i * 0.1 + 10.0);
  }
  fflush(fp);
  rewind(fp);
  ASSERT_GT(set_paramptr(fp), 0);

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

  fprintf(fp, "VERSION: %f\n", YP_PARAM_REQUIRED_VERSION);
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

  fprintf(fp, "VERSION: %f", YP_PARAM_REQUIRED_VERSION);
  for (int i = 0; i < YP_PARAM_NUM; i++)
  {
    if (param_names[i][0] == '_' || skipKeys(i))
      continue;
    fprintf(fp, "\n%s: %f", param_names[i], i * 0.1 + 10.0);
  }
  fflush(fp);
  rewind(fp);
  ASSERT_GT(set_paramptr(fp), 0);

  for (int i = 0; i < YP_PARAM_NUM; i++)
  {
    if (param_names[i][0] == '_' || skipKeys(i))
      continue;
    ASSERT_DOUBLE_EQ(p(static_cast<YPSpur_param>(i), MOTOR_RIGHT), i * 0.1 + 10.0);
    ASSERT_DOUBLE_EQ(p(static_cast<YPSpur_param>(i), MOTOR_LEFT), i * 0.1 + 10.0);
  }
}

int main(int argc, char **argv)
{
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
