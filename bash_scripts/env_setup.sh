#!/bin/bash

# Check ROS installation
source /opt/ros/humble/setup.bash

# Install ROS dependencies
sudo apt install python3-colcon-common-extensions
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
