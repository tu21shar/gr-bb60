# GNU Radio - BB60C

###### A GNU Radio module for the Signal Hound BB60C spectrum analyzer

_Currently in development._


## Preinstallation

1. sudo apt-get install libusb-1.0-0
2. cd GNURadio-BB60C/sh_lib
3. sudo cp bb60.rules /etc/udev/rules.d/ 
4. cd lib
5. ldconfig -v -n 
6. ln -sf libbb_api.so.3.0.16 libbb_api.so
7. sudo cp libbb_api.* /usr/local/lib
8. ldconfig -v -n
9. sudo cp libftd2xx.* /usr/local/lib


## Installation

1. cd GNURadio-BB60C
2. mkdir build
3. cd build
4. cmake ..
5. make
6. sudo make install
7. sudo ldconfig
