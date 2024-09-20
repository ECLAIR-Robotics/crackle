# crackle
Contextual Robotics Arm for Knowledge Learning and Exploration

# Setup 

**You should only run ros commands in some form on Linux installation. Either WSL, a dual boot Ubuntu 22.04, or a docker container.** 

- If you have a dual boot Ubuntu 22.04 or WSL: Install ROS2 humble from [ROS2 Humble Tutorials](https://docs.ros.org/en/humble/Tutorials.html)
- if you plan on using a docker container (Check FAQs for more information), first make a folder using ```mkdir eclair``` and cd into the folder using ```cd eclair``` and then run the following commands normally.

### You need to make a workspace for your project. We will call this folder "crackle_ws" and we will CD into this folder.

  ```mkdir crackle_ws```

  ```cd crackle_ws```

### Make an "src" folder to put your code in, then clone the repo inside the folder. 
  ```mkdir src```
  
  ```cd src```
  
  ```git clone --recursive git@github.com:ECLAIR-Robotics/crackle.git```

### Start the docker container

* copy ```Dockerfile``` and ```Makefile``` such that they are in the same level as the ```crackle_ws``` folder. Something like this image ![image](https://github.com/user-attachments/assets/ab88cc6b-3745-422d-b492-4f8011f24be8)
* Open terminal such that you are now in the same level as the ```Dockerfile``` and the ```Makefile``` and run ```docker build -t crackle:humble .``` _This will take some time, grab a coffee :coffee: or look at something in distance for the time being_ 
* Run ```make docker``` to enter the terminal inside your docker container
  
# FAQs

1. How do I know if I have to use a Docker container?
* You will need it if you don't have ros2 humble installed and are using Windows or MacOS.

 
