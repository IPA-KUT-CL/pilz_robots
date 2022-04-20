# PILZ robot manipulator module PRBT 6 in ROS for ROS Endurance Test

## Introduction
This repo is a modified version of [pilz_robots](https://github.com/PilzDE/pilz_robots) specialized for RET application, the original readme file see [PILZ_README](https://github.com/IPA-KUT-CL/pilz_robots/blob/melodic-devel/README.md)

### Modification
- Unable the safety interface by change default value of `iso10218_support` to `false`, see [robot.launch](https://github.com/IPA-KUT-CL/pilz_robots/blob/master/prbt_support/launch/robot.launch#L8)
- Add `ready` pose as a group pose, see [prbt.srdf.xacro](https://github.com/IPA-KUT-CL/pilz_robots/blob/master/prbt_moveit_config/config/prbt.srdf.xacro#L23)

## Prerequisite
- [pilz_common](https://github.com/PilzDE/pilz_common) in the same workspace

## Usage
Use the package `prbt_moveit_config` to run moveit to manipulate the robot both in simulation and on a real robot, the core launch file is `moveit_planning_execution.launch` with following attributes:
- `sim` (default: True) <br>
    true: Use fake execution and display emulated robot position in RViz<br>
    false: connect to real robot using `ros_canopen`
- `pipeline` (default: ompl) <br>
    Planning pipeline to use with moveit
- `load_robot_description` (default: True)<br>
    Load robot description to parameter server. Can be set to false to let someone else load the model
- `rviz_config` (default: prbt_moveit_config/launch/moveit.rviz)<br>
    Start RViz with default configuration settings. Once you have changed the configuration and have saved
       it inside your package folder, set the path and file name here.
- `gripper` (default: None) <br>
    See [Running the prbt with a gripper](https://github.com/IPA-KUT-CL/pilz_robots/blob/melodic-devel/README.md#running-the-prbt-with-a-gripper)
- `safety_hw` (default: pss4000)<br>
    Connect to the safety controller that handles the safe-torque-off signal.
        Only relevant for `sim:=False` to issue a Safe stop 1.
        See [prbt_hardware_support package](prbt_hardware_support/README.md).

### Simulation
1. Run `roslaunch prbt_moveit_config moveit_planning_execution.launch sim:=true`
2. Use the moveit Motion Planning rviz plugin to plan and execute

### On robot
1. Set up the connection to robot, set new wired setting with `IP: 169.254.60.100 Netmask: 255.255.255.0`
2. Set up CAN interface
    ```
    # first bring the can down
    sudo ip link set can0 down
    # then bring it up
    sudo ip link set can0 up type can bitrate 1000000
    ```
3. Run `roslaunch prbt_moveit_config moveit_planning_execution.launch sim:=false`
4. Use the moveit Motion Planning rviz plugin to plan and execute

## Troubleshooting
1. Sometimes following errors appears when launch bring up the robot, try power off the robot for a minute and boot it again
   ```
   [ERROR] [1649844939.511514720]: Action client not connected: prbt/manipulator_joint_trajectory_controller/follow_joint_trajectory
   [ERROR] [1649844933.488457319]: not operational
   [ERROR] [1649844933.489202470]: Initializing failed: Transition timeout; Could not enable motor; Transition timeout
   ```
2. The holding mode of `manipulator_joint_trajectory_controller` is set for some trajectory exceeding limits and may not be deactivated by itself, call unhold service in another terminal
    ```
    rosservice call /prbt/manipulator_joint_trajectory_controller/unhold
    ```
3. While shutting down moveit application, error `Controller Spawner error while taking down controllers` could appear, *solution to be found*