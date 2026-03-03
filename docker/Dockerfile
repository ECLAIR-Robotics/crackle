# Ubuntu 22.04
FROM ros:humble-ros-base-jammy

# Setup timezone
ENV TZ=Etc/UTC
RUN echo $TZ > /etc/timezone && \
  ln -fs /usr/share/zoneinfo/$TZ /etc/localtime

# Tools necessary and useful during development
RUN apt-get update && \
  DEBIAN_FRONTEND=noninteractive \
  apt install --no-install-recommends -y \
  build-essential \
  atop \
  ca-certificates \
  cmake \
  cppcheck \
  curl \
  expect \
  gdb \
  git \
  gnupg2 \
  gnutls-bin \
  iputils-ping \
  libbluetooth-dev \
  libccd-dev \
  libcwiid-dev \
  libeigen3-dev \
  libfcl-dev \
  libgflags-dev \
  libgles2-mesa-dev \
  libgoogle-glog-dev \
  libspnav-dev \
  libusb-dev \

  # CGAL
  libcgal-dev \
  libgmp-dev\
  libmpfr-dev\
  libboost-all-dev\

  # FFTW3
  libfftw3-dev \
  pkg-config \
  lsb-release \
  net-tools \
  pkg-config \
  protobuf-compiler \
  python3-dbg \
  python3-empy \
  python3-numpy \
  python3-setuptools \
  python3-pip \
  python3-venv \
  ruby \
  software-properties-common \
  sudo \
  vim \
  wget \
  xvfb \

  # Audio support (for PyAudio / openwakeword)
  portaudio19-dev \
  libasound2-dev \
  libsndfile1-dev \

  # Python development headers (required by CMake find_package Python3 Development)
  python3-dev \

  && apt clean -qq

# Setup locale
RUN sudo apt update && sudo apt install locales \
  && sudo locale-gen en_US en_US.UTF-8 \
  && sudo update-locale LC_ALL=en_US.UTF-8 LANG=en_US.UTF-8

ARG ROSDIST=humble
ARG GZDIST=garden
ENV GZ_VERSION=garden

# Set up repo to install Ignition/Gazebo
RUN /bin/sh -c 'wget https://packages.osrfoundation.org/gazebo.gpg -O /usr/share/keyrings/pkgs-osrf-archive-keyring.gpg' \
  && /bin/sh -c 'echo "deb [arch=$(dpkg --print-architecture) signed-by=/usr/share/keyrings/pkgs-osrf-archive-keyring.gpg] http://packages.osrfoundation.org/gazebo/ubuntu-stable $(lsb_release -cs) main" | tee /etc/apt/sources.list.d/gazebo-stable.list > /dev/null'

# Set up Intel RealSense repository
RUN mkdir -p /etc/apt/keyrings \
  && curl -sSf https://librealsense.intel.com/Debian/librealsense.pgp \
  | tee /etc/apt/keyrings/librealsense.pgp > /dev/null \
  && echo "deb [signed-by=/etc/apt/keyrings/librealsense.pgp] https://librealsense.intel.com/Debian/apt-repo $(lsb_release -cs) main" \
  | tee /etc/apt/sources.list.d/librealsense.list > /dev/null

# Install Gazebo and ROS2 Desktop
RUN apt update \
  && apt install -y --no-install-recommends \
  gz-${GZDIST} \
  ros-${ROSDIST}-desktop \
  && rm -rf /var/lib/apt/lists/* \
  && apt clean -qq

# Install Intel RealSense SDK
RUN apt update \
  && apt install -y --no-install-recommends \
  librealsense2-dev \
  librealsense2-udev-rules \
  && rm -rf /var/lib/apt/lists/* \
  && apt clean -qq

# Install some 'standard' ROS packages and utilities.
RUN apt update \
  && apt install -y --no-install-recommends \
  python3-colcon-common-extensions \
  python3-vcstool \
  python3-sdformat13 \
  ros-${ROSDIST}-actuator-msgs \
  ros-${ROSDIST}-ament-cmake-pycodestyle \
  ros-${ROSDIST}-image-transport \
  ros-${ROSDIST}-image-transport-plugins \
  ros-${ROSDIST}-joy-teleop \
  ros-${ROSDIST}-joy-linux \
  ros-${ROSDIST}-mavros-msgs \
  ros-${ROSDIST}-radar-msgs \
  ros-${ROSDIST}-ros-gzgarden \
  ros-${ROSDIST}-rqt-graph \
  ros-${ROSDIST}-rqt-image-view \
  ros-${ROSDIST}-rqt-plot \
  ros-${ROSDIST}-rqt-topic \
  ros-${ROSDIST}-rviz2 \
  ros-${ROSDIST}-xacro \
  ros-${ROSDIST}-plotjuggler-ros \
  && sudo rm -rf /var/lib/apt/lists/* \
  && sudo apt clean -qq

# Install MoveIt2 and motion planning
RUN apt update \
  && apt install -y --no-install-recommends \
  ros-${ROSDIST}-moveit \
  ros-${ROSDIST}-moveit-servo \
  ros-${ROSDIST}-moveit-ros-planning-interface \
  && rm -rf /var/lib/apt/lists/* \
  && apt clean -qq

# Install perception, control, and hardware packages
RUN apt update \
  && apt install -y --no-install-recommends \
  ros-${ROSDIST}-cv-bridge \
  ros-${ROSDIST}-pcl-conversions \
  ros-${ROSDIST}-pcl-ros \
  ros-${ROSDIST}-octomap-ros \
  ros-${ROSDIST}-octomap-msgs \
  ros-${ROSDIST}-tf2 \
  ros-${ROSDIST}-tf2-ros \
  ros-${ROSDIST}-tf2-geometry-msgs \
  ros-${ROSDIST}-tf2-tools \
  ros-${ROSDIST}-robot-state-publisher \
  ros-${ROSDIST}-joint-state-publisher \
  ros-${ROSDIST}-joint-state-publisher-gui \
  ros-${ROSDIST}-joint-trajectory-controller \
  ros-${ROSDIST}-joint-state-broadcaster \
  ros-${ROSDIST}-controller-manager \
  ros-${ROSDIST}-hardware-interface \
  ros-${ROSDIST}-control-toolbox \
  ros-${ROSDIST}-ros2-control \
  ros-${ROSDIST}-ros2-controllers \
  ros-${ROSDIST}-diagnostic-updater \
  ros-${ROSDIST}-backward-ros \
  ros-${ROSDIST}-realsense2-description \
  ros-${ROSDIST}-camera-info-manager \
  && rm -rf /var/lib/apt/lists/* \
  && apt clean -qq

# Install PyTorch Nightly for 5090 (sm_120) but force NumPy < 2.0 for ROS 2 Humble
# Install PyTorch and other Python dependencies for MPPI controllers
RUN echo "numpy<2.0" > /tmp/constraints.txt \
  && pip3 install --no-cache-dir --upgrade pip setuptools wheel \
  # Manually force-upgrade distutils-installed packages to bypass the
  # "distutils installed project" error. This overwrites the apt versions.
  && pip3 install --no-cache-dir --constraint /tmp/constraints.txt \
  --ignore-installed "sympy>=1.13.3" blinker \
  # 0. Install numpy explicitly first so C headers are in a known pip-managed
  #    location before any other package installs it as a transitive dependency.
  #    This ensures rosidl_generator_py can find numpy/ndarrayobject.h via
  #    numpy.get_include() during colcon builds.
  && pip3 install --no-cache-dir "numpy==1.26.3" \
  # 1. Install PyTorch Nightly (CUDA 12.8)
  && pip3 install --no-cache-dir \
  --constraint /tmp/constraints.txt \
  --pre \
  --index-url https://download.pytorch.org/whl/nightly/cu128 \
  torch torchvision torchaudio \
  # 2. Install scientific stack
  && pip3 install --no-cache-dir \
  --constraint /tmp/constraints.txt \
  scipy \
  matplotlib \
  pyyaml \
  numba \
  pillow \
  opencv-python-headless \
  "transformers[vision]" \
  # 3. Install project-specific Python dependencies
  && pip3 install --no-cache-dir \
  --constraint /tmp/constraints.txt \
  mediapipe \
  pyaudio \
  openai \
  elevenlabs \
  ultralytics \
  open3d \
  # tf_transformations \
  scikit-learn \
  onnxruntime \
  openwakeword \
  pyntcore \
  robotpy-apriltag \
  robotpy-wpimath \
  robotpy-wpinet \
  robotpy-wpiutil \
  && rm /tmp/constraints.txt \
  && rm -rf /root/.cache/pip

ENTRYPOINT ["/ros_entrypoint.sh"]
CMD ["bash"]
