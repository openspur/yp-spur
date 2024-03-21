// Copyright (c) 2024 The YP-Spur Authors, except where otherwise indicated.
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

#include <gtest/gtest.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ypspur-md.h>

#include <stdexcept>

char* test_dir;

class E2E : public ::testing::Test
{
protected:
  void initCoordinator(const std::string& param_name)
  {
    coordinator_pid_ = fork();
    ASSERT_GE(coordinator_pid_, 0);
    if (coordinator_pid_ == 0)
    {
      const std::string param_file = std::string(test_dir) + "/" + param_name;
      execl("./ypspur-coordinator", "ypspur-coordinator", "--without-device", "--param", param_file.c_str(), nullptr);
    }
    sleep(1);
    ASSERT_GE(YPSpur_md_init(&cli_), 0) << "Failed to connect to ypspur-coordinator";
  }
  void TearDown()
  {
    kill(coordinator_pid_, 2);
    wait(nullptr);
  }

  YPSpur cli_;
  pid_t coordinator_pid_;
};

TEST_F(E2E, Vel)
{
  ASSERT_NO_FATAL_FAILURE(initCoordinator("vehicle.param"));

  ASSERT_GE(YPSpur_md_set_vel(&cli_, 2), 0);
  ASSERT_GE(YPSpur_md_set_accel(&cli_, 0.5), 0);
  ASSERT_GE(YPSpur_md_set_angvel(&cli_, 2), 0);
  ASSERT_GE(YPSpur_md_set_angaccel(&cli_, 0.5), 0);
  ASSERT_GE(YPSpur_md_vel(&cli_, 0.7, 0.9), 0);

  double v, w;
  sleep(1);
  ASSERT_GE(YPSpur_md_get_vel(&cli_, &v, &w), 0);
  ASSERT_NEAR(v, 0.5, 0.1);
  ASSERT_NEAR(w, 0.5, 0.1);

  sleep(1);
  ASSERT_GE(YPSpur_md_get_vel(&cli_, &v, &w), 0);
  ASSERT_NEAR(v, 0.7, 0.1);
  ASSERT_NEAR(w, 0.9, 0.1);
}

TEST_F(E2E, LineSpin)
{
  ASSERT_NO_FATAL_FAILURE(initCoordinator("vehicle.param"));

  ASSERT_GE(YPSpur_md_set_vel(&cli_, 4), 0);
  ASSERT_GE(YPSpur_md_set_accel(&cli_, 8), 0);
  ASSERT_GE(YPSpur_md_set_angvel(&cli_, 4), 0);
  ASSERT_GE(YPSpur_md_set_angaccel(&cli_, 8), 0);

  ASSERT_GE(YPSpur_md_set_pos(&cli_, CS_GL, 0, 0, 0), 0);

  ASSERT_GE(YPSpur_md_stop_line(&cli_, CS_GL, 0.5, 0, 0), 0);
  sleep(1);

  double x, y, th;
  ASSERT_GE(YPSpur_md_get_pos(&cli_, CS_GL, &x, &y, &th), 0);
  ASSERT_NEAR(x, 0.5, 0.05);
  ASSERT_NEAR(y, 0.0, 0.05);
  ASSERT_NEAR(th, 0.0, 0.05);

  ASSERT_GE(YPSpur_md_spin(&cli_, CS_GL, 1.5708), 0);
  sleep(2);

  ASSERT_GE(YPSpur_md_get_pos(&cli_, CS_GL, &x, &y, &th), 0);
  ASSERT_NEAR(x, 0.5, 0.05);
  ASSERT_NEAR(y, 0.0, 0.05);
  ASSERT_NEAR(th, 1.5708, 0.05);

  ASSERT_GE(YPSpur_md_stop_line(&cli_, CS_GL, 0.5, -0.5, 1.5708), 0);
  sleep(1);

  ASSERT_GE(YPSpur_md_get_pos(&cli_, CS_GL, &x, &y, &th), 0);
  ASSERT_NEAR(x, 0.5, 0.05);
  ASSERT_NEAR(y, -0.5, 0.05);
  ASSERT_NEAR(th, 1.5708, 0.05);
}

int main(int argc, char** argv)
{
  if (argc < 2)
  {
    std::cerr << "test dir path should be provided as a first argument" << std::endl;
    return 1;
  }
  test_dir = argv[1];

  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
