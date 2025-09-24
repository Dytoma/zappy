#!/bin/bash

# Install required packages (Debian/Ubuntu)
sudo apt update
sudo apt install -y git build-essential cmake libasound2-dev libpulse-dev \
  libx11-dev libxcursor-dev libxrandr-dev libxi-dev libxinerama-dev \
  libgl1-mesa-dev libegl1-mesa-dev

# Clone raylib
git clone https://github.com/raysan5/raylib.git
cd raylib || exit 1
mkdir build && cd build || exit 1
cmake ..
sudo make install
