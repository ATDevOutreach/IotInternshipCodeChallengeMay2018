#!/usr/bin/bash

cd /opt/
sudo mkdir paho_c
cd paho_c
sudo git clone git clone https://github.com/eclipse/paho.mqtt.c.git
pushd paho.mqtt.c
sudo git checkout develop
sudo mkdir cmake_build && cd cmake_build
sudo cmake ..
sudo make
sudo make install
sudo ldconfig
popd

exit 0