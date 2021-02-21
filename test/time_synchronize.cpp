// Copyright (c) 2021 The YP-Spur Authors, except where otherwise indicated.
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

#include <odometry.h>
#include <param.h>
#include <serial.h>

extern double g_interval;
extern double g_offset;
extern Odometry g_odometry;
extern Parameters g_param;

double time_synchronize(double receive_time, int readnum, int wp);

#ifdef __cplusplus
}
#endif  // __cplusplus

TEST(TimeSynchronize, PacketLost)
{
  g_param.output_lv = OUTPUT_LV_DEBUG;
  g_param.option = OPTION_SHOW_TIMESTAMP;
  g_interval = SER_INTERVAL;
  g_offset = 0;
  g_odometry.packet_lost = 0;

  time_synchronize(10.005, 1, 0);
  EXPECT_EQ(g_odometry.packet_lost, 0);
  EXPECT_NEAR(time_estimate(0), 10.005, 0.001);

  time_synchronize(10.010, 1, 0);
  EXPECT_EQ(g_odometry.packet_lost, 0);
  EXPECT_NEAR(time_estimate(0), 10.010, 0.001);

  time_synchronize(10.025, 3, 0);
  EXPECT_EQ(g_odometry.packet_lost, 0);
  EXPECT_NEAR(time_estimate(0), 10.015, 0.001);
  EXPECT_NEAR(time_estimate(1), 10.020, 0.001);
  EXPECT_NEAR(time_estimate(2), 10.025, 0.001);

  time_synchronize(10.030, 1, 0);
  EXPECT_EQ(g_odometry.packet_lost, 0);
  EXPECT_NEAR(time_estimate(0), 10.030, 0.001);

  time_synchronize(10.050, 1, 0);  // lost 3
  EXPECT_EQ(g_odometry.packet_lost, 3);
  EXPECT_NEAR(time_estimate(0), 10.050, 0.001);

  time_synchronize(10.055, 2, 0);  // recover 1, lost 2
  EXPECT_EQ(g_odometry.packet_lost, 2);
  EXPECT_NEAR(time_estimate(0), 10.050, 0.001);
  EXPECT_NEAR(time_estimate(1), 10.055, 0.001);
}

TEST(TimeSynchronize, Reset)
{
  g_param.output_lv = OUTPUT_LV_DEBUG;
  g_param.option = OPTION_SHOW_TIMESTAMP;
  g_interval = SER_INTERVAL;
  g_offset = 0;
  g_odometry.packet_lost = 0;

  time_synchronize(10.005, 1, 0);
  EXPECT_EQ(g_odometry.packet_lost, 0);
  EXPECT_NEAR(time_estimate(0), 10.005, 0.001);

  time_synchronize(10.010, 1, 0);
  EXPECT_EQ(g_odometry.packet_lost, 0);
  EXPECT_NEAR(time_estimate(0), 10.010, 0.001);

  time_synchronize(10.600, 1, 0);
  EXPECT_EQ(g_odometry.packet_lost, 0);
  EXPECT_NEAR(time_estimate(0), 10.600, 0.001);

  time_synchronize(10.605, 1, 0);
  EXPECT_EQ(g_odometry.packet_lost, 0);
  EXPECT_NEAR(time_estimate(0), 10.605, 0.001);
}

TEST(TimeSynchronize, ClockGain)
{
  g_param.output_lv = OUTPUT_LV_DEBUG;
  g_param.option = OPTION_SHOW_TIMESTAMP;
  g_interval = SER_INTERVAL;
  g_offset = 0;
  g_odometry.packet_lost = 0;

  const double actual_intervals[] = {
    0.00495,
    0.00498,
    0.00502,
    0.00505,
  };

  for (const double actual_interval : actual_intervals)
  {
    int cnt = 0;
    for (double t = 100; t < 160; t += actual_interval)
    {
      int readnum = 1;
      if (++cnt % 10 == 0)
      {
        t += actual_interval;
        readnum++;
      }

      time_synchronize(t, readnum, 0);
      ASSERT_EQ(g_odometry.packet_lost, 0);
      for (int i = 0; i < readnum; i++)
      {
        ASSERT_NEAR(time_estimate(i), t - actual_interval * (readnum - 1 - i), 0.001)
            << "interval: " << actual_interval << ", t: " << t << ", cnt: " << cnt << ", i: " << i;
      }
    }

    ASSERT_NEAR(actual_interval, g_interval, 0.00001)
        << "interval: " << actual_interval;
  }
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
