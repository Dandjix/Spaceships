## linux : required dependencies :

sudo apt-get install build-essential git make \
pkg-config cmake ninja-build gnome-desktop-testing libasound2-dev libpulse-dev \
libaudio-dev libfribidi-dev libjack-dev libsndio-dev libx11-dev libxext-dev \
libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev \
libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev \
libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev libthai-dev libusb-1.0-0-dev

## to build project (from Spaceships root directory, the one containing the .gitignore and this README.md file) :

cmake -S . -B build
cmake --build build

## Run the project :
./build/Spaceships

## Run the unit tests :
./build/tests/SpaceshipTests