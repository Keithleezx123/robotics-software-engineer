#!/bin/bash

set -e

echo "Fixing ROS apt source..."
sudo sh -c 'echo "deb http://packages.ros.org/ros/ubuntu focal main" > /etc/apt/sources.list.d/ros-latest.list'

echo "Refreshing ROS key..."
sudo apt-key adv --keyserver 'hkp://keyserver.ubuntu.com:80' --recv-key F42ED6FBAB17C654

echo "Updating apt..."
sudo apt update

echo "Installing ROS control packages..."
sudo apt install -y ros-noetic-effort-controllers ros-noetic-ros-controllers ros-noetic-ros-control

echo "Sourcing ROS..."
source /opt/ros/noetic/setup.bash

echo "Building catkin workspace..."
cd ~/robotics-software-engineer/catkin_ws
catkin_make

echo "Sourcing workspace..."
source devel/setup.bash

echo "Setting Gazebo plugin path..."
export GAZEBO_PLUGIN_PATH=$GAZEBO_PLUGIN_PATH:~/robotics-software-engineer/catkin_ws/build

echo "Done."

