# Getting Started (Setup)

<aside>
💡 Notion Tip: When creating a page, it's important to give it a clear title and provide some content. This could include verifying the information, summarizing the topic, or sharing your thoughts and opinions on something that matters to you.

</aside>

# The Basics

# Setup

**You should only run ROS commands in some form on Linux (preferably Ubuntu 22.04.X) installation. If you don’t have a Linux boot running on your machine, then there are a few options:**

- **WSL / [Oracle Virtual Box](https://download.virtualbox.org/virtualbox/7.1.4/VirtualBox-7.1.4-165100-Win.exe)**
    - 
    
    [Setting up Oracle Virtual Box](https://www.notion.so/Setting-up-Oracle-Virtual-Box-147d0549950a80f9b9e2c240506b72eb?pvs=21)
    
- Docker Container (only recommended for MacOS)

- If you have a dual boot Ubuntu 22.04 or WSL: Install ROS2 humble from [ROS2 Humble Tutorials](https://docs.ros.org/en/humble/Tutorials.html)

### Install ROS2 Humble

Install ROS2 humble from https://docs.ros.org/en/humble/Installation/Ubuntu-Install-Debs.html

### You need to make a workspace for your project. We will call this folder "crackle_ws" and we will CD into this folder.

`mkdir crackle_ws`

`cd crackle_ws`

### Make an "src" folder to put your code in, then clone the repo inside the folder.

`mkdir src`

`cd src`

`git clone git@github.com:ECLAIR-Robotics/crackle.git`

### Install Dependencies

- Colcon: `sudo apt install python3-colcon-common-extensions`
- Diagnostic Updater: `sudo apt install ros-humble-diagnostic-updater*`
- Control Messages: `sudo apt install ros-humble-control-msgs*`
- Realsense SDK: `sudo apt install ros-humble-librealsense2*`
- Hardware Interface: `sudo apt install ros-humble-hardware-interface*`
- Controller Manager: `sudo apt install ros-humble-controller-manager*`
- Gazebo: `sudo apt install ros-humble-gazebo-*`
- Moveit: `sudo apt install ros-humble-moveit && suudo apt install ros-humble-moveit-*`

### Quality of Life for Lazy Coders

`echo "source /opt/ros/humble/setup.bash" >> ~/.bashrc`

`echo “alias scrackle=’source ${HOME}/crackle_ws/install/`

`setup.bash'">> ~/.bashrc` 

`echo "source ${HOME}/crackle_ws/src/crackle/bash_tools" >>` 

`~/.bashrc`

---

### Start the docker container (Skip if you are using virtualbox, WSL, or boot)

- copy `Dockerfile` and `Makefile` such that they are in the same level as the `crackle_ws` folder. Something like this image
    
    https://github.com/user-attachments/assets/ab88cc6b-3745-422d-b492-4f8011f24be8
    
- Open terminal such that you are now in the same level as the `Dockerfile` and the `Makefile` and run `docker build -t crackle:humble .` *This will take some time, grab a coffee :coffee: or look at something in distance for the time being*
- Run `make docker` to enter the terminal inside your docker container

# Setting up X forwarding

[How to set up working X11 forwarding on WSL2](https://stackoverflow.com/questions/61110603/how-to-set-up-working-x11-forwarding-on-wsl2)

# FAQs

1. How do I know if I have to use a Docker container?
- You will need it if you don't have ros2 humble installed and are using Windows or MacOS.
