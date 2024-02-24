# This is an auto generated Dockerfile for ros:ros-core
# generated from docker_images_ros2/create_ros_core_image.Dockerfile.em
FROM ros:humble-ros-core-jammy

# install packages
RUN apt-get update && apt-get install -q -y --no-install-recommends \
    build-essential \
    git \
    python3-colcon-common-extensions \
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
    && rm -rf /var/lib/apt/lists/*


# Set the working directory inside the container
WORKDIR /root

# Copy the entire project into the container
COPY . /root

# setup entrypoint
COPY ./ros_entrypoint.sh /

ENTRYPOINT ["/ros_entrypoint.sh"]

CMD ["bash"]