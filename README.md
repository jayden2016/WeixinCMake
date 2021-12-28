# WeixinCMake
CMake to compile Weixin SDK

Linux环境：
cd build
sudo cmake -DCMAKE_HOST_SYSTEM_NAME=Linux ..
sudo make
sudo make install

Windows环境：
cd build
cmake -DCMAKE_HOST_SYSTEM_NAME=Windows ..
make
make install

Mac环境：
cd build
cmake -DCMAKE_HOST_SYSTEM_NAME=Darwin ..
make
make install

库的位置：
./build/lib