# This is an auto generated Dockerfile for ros:ros-core
# generated from docker_images_ros2/create_ros_core_image.Dockerfile.em
FROM ros:humble-ros-core-jammy

# install packages
RUN apt-get update && apt-get install -q -y --no-install-recommends \
    wget\
    ffmpeg \
    libsm6 \
    libxext6 \
    build-essential \
    git \
    python3-pip\
    python3-colcon-common-extensions  \
    python3-colcon-mixin \
    python3-rosdep \
    python3-vcstool \
    && rm -rf /var/lib/apt/lists/*\
    dirmngr \
    gnupg2 \
    && rm -rf /var/lib/apt/lists/*

# setup environment
ENV LANG C.UTF-8
ENV LC_ALL C.UTF-8

ENV ROS_DISTRO humble

ENV VNC_RESOLUTION=1024x640 \
    TERM=xterm \
    HOME=/home/user \
    PATH=/opt/TurboVNC/bin:$PATH \
    SSH_PORT=22


# bootstrap rosdep
RUN rosdep init && \
  rosdep update --rosdistro $ROS_DISTRO

# setup colcon mixin and metadata
RUN colcon mixin add default \
      https://raw.githubusercontent.com/colcon/colcon-mixin-repository/master/index.yaml && \
    colcon mixin update && \
    colcon metadata add default \
      https://raw.githubusercontent.com/colcon/colcon-metadata-repository/master/index.yaml && \
    colcon metadata update

# install ros2 packages
RUN apt-get update && apt-get install -y --no-install-recommends \
    ros-humble-ros-core=0.10.0-1* \
    x11vnc \
    ros-humble-turtlesim \
    && rm -rf /var/lib/apt/lists/*

RUN apt-get update && apt-get install -y \
      x11-apps \
      && rm -rf /usr/share/doc/* && \
      rm -rf /usr/share/info/* && \
      rm -rf /tmp/* && \
      rm -rf /var/tmp/*

# Set the working directory inside the container
WORKDIR /app

# Copy the entire project into the container
COPY . /app

ENTRYPOINT ["/ros_entrypoint.sh"]

CMD ["bash"]