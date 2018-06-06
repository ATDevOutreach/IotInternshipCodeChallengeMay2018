#!/usr/bin/bash

cd /opt/
sudo mkdir paho_cpp
cd paho_cpp
sudo git clone https://github.com/eclipse/paho.mqtt.cpp.git
pushd paho.mqtt.cpp
sudo git checkout develop
sudo mkdir build && cd build
sudo cmake  -DPAHO_MQTT_C_PATH=/opt/paho_c/paho.mqtt.c ..
sudo make
sudo make install
sudo ldconfig   
popd

exit 0