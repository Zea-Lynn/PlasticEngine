PLASTIC ENGINE
==============

Build commands:

git clone git@github.com:zeajenson/PlasticEngine.git
cd PlasticEngine

git submodule update --init --recursive
mkdir build; cd build
cmake ../ -G Ninja
ninja
./plastic

