/*
 * Copyright (c) 2020 Pilz GmbH & Co. KG
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PILZ_TESTUTILS_JOINT_STATE_PUBLISHER_MOCK_H
#define PILZ_TESTUTILS_JOINT_STATE_PUBLISHER_MOCK_H

#include <atomic>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

#include <ros/ros.h>

#include <sensor_msgs/JointState.h>

namespace pilz_testutils
{
/**
 * @brief Mocks the joint_states interface. Can simulate robot movement by changing the position of the first joint in
 * \c controller_joint_names.
 */
class JointStatePublisherMock
{
  typedef sensor_msgs::JointState JointState;
  typedef sensor_msgs::JointStateConstPtr JointStateConstPtr;

public:
  /**
   * @brief Constructor.
   * @param ns Namespace in which the joint states are to be published.
   */
  JointStatePublisherMock();

  /**
   * @brief Start periodic publishing in a separate thread.
   */
  void startAsync();

  /**
   * @brief Set velocity for joint1.
   */
  void setVelocity(const double& joint1_velocity);

  /**
   * @brief Go back to home position (position=velocity=0.0).
   *
   * This is needed in order to ensure a clean tear-down.
   * The current velocity is maintained until the home position is reached.
   */
  void goHome();

  /**
   * @brief Stop publishing joint states.
   */
  void stop();

  /**
   * @brief In degenerate-time-mode each message is published twice.
   */
  void setDegenerateTimeMode();

  /**
   * @brief Return the message which will be published next.
   */
  JointStateConstPtr getNextMessage();

private:
  void run();
  void createMessage(JointState& msg);
  void updateJoint1Position();

private:
  ros::NodeHandle nh_;
  ros::Publisher pub_;
  std::vector<std::string> joint_names_;
  std::atomic_bool stop_flag_;
  std::atomic_bool go_home_flag_;
  std::atomic_bool degenerate_time_flag_{ false };
  std::thread publisher_thread_;
  std::mutex joint1_position_mutex_;
  std::mutex joint1_velocity_mutex_;
  double joint1_position_{ 0.0 };
  double joint1_velocity_{ 0.0 };
};

}  // namespace pilz_testutils

#endif
