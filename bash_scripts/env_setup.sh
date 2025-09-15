#!/bin/bash

# Check ROS installation
source /opt/ros/humble/setup.bash

# Set up ROS apt repositories 
sudo apt install software-properties-common
sudo add-apt-repository universe
sudo apt update && sudo apt install curl -y
export ROS_APT_SOURCE_VERSION=$(curl -s https://api.github.com/repos/ros-infrastructure/ros-apt-source/releases/latest | grep -F "tag_name" | awk -F\" '{print $4}')
sudo curl -sSL https://raw.githubusercontent.com/ros/rosdistro/master/ros.key -o /usr/share/keyrings/ros-archive-keyring.gpg
echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/ros-archive-keyring.gpg] http://packages.ros.org/ros2/ubuntu $(. /etc/os-release && echo $UBUNTU_CODENAME) main" | sudo tee /etc/apt/sources.list.d/ros2.list > /dev/null

# Update source repositories and upgrade dependencies
sudo apt update
sudo apt upgrade

# Install ROS dependencies
sudo apt install python3-colcon-common-extensions
sudo apt install ros-humble-desktop
sudo apt install ros-humble-ros-base
sudo apt install ros-dev-tools
sudo apt install ros-humble-diagnostic-updater*
sudo apt install ros-humble-control-msgs*
sudo apt install ros-humble-librealsense2*
sudo apt install ros-humble-hardware-interface*
sudo apt install ros-humble-controller-manager*
sudo apt-get install ros-humble-gazebo-*
sudo apt install ros-humble-moveit && sudo apt install ros-humble-moveit-*

# setup .bashrc
echo "source /opt/ros/humble/setup.bash" >> $HOME/.bashrc
echo 'alias scrackle="source ${HOME}/crackle_ws/install/setup.bash"'>> ~/.bashrc
echo "source ${HOME}/crackle_ws/src/crackle/bash_tools" >> ~/.bashrc

# Install Starship
curl -sS https://starship.rs/install.sh | sh
echo 'eval "$(starship init bash)"' >> $HOME/.bashrc

# Configure starship
mkdir -p $HOME/.config && touch $HOME/.config/starship.toml

cp $HOME/crackle_ws/src/crackle/bash_scripts/starship.toml $HOME/.config/
