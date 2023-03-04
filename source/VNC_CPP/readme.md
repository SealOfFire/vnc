windows安装boost  
./bootstrap.bat --with-python=D:\ProgramData\Anaconda3  
./b2 install --prefix="D:\Program Files\boost" --with-python  
./b2 --build-dir="D:\Program Files\boost" --build-type=complete msvc stage  

.\b2 link=static runtime-link=shared threading=multi variant=release --build-type=complete toolset=msvc install --prefix="D:\Program Files\boost"

.\b2 --build-type=complete --prefix="D:\Program Files\boost"
.\b2 install --prefix="D:\Program Files\boost" --build-type=complete --toolset=msvc -14.3
variant=release address-model=64 link=static,shared

.\b2 variant=release address-model=64 link=static,shared --prefix="D:\Program Files\boost"

./bootstrap.bat --prefix="D:\Program Files\boost"  

.\b2 link=static runtime-link=static threading=multi variant=debug install --prefix="D:\Program Files\boost"  

.\b2 ariant=release address-model=64 link=static,shared --build-type=complete install --prefix="D:\Program Files\boost"


.\b2 install --prefix="D:\Program Files\boost"

多个版本的python打包
.\bootstrap.bat --with-python=D:/ProgramData/Anaconda3/envs/DeepAIEnviroment32  
.\b2 install --prefix="D:\Program Files\boost" --with-python  

boost 添加zlib
.\b2 install --prefix="D:\Program Files\boost" --with-iostreams -sZLIB_SOURCE="D:\Program Files\zlib-1.2.13"

https://blog.csdn.net/qq_36163982/article/details/122438593  

linux 安装boost  
sudo apt-get install libboost-all-dev  

./bootstrap.sh --help
手动安装
./bootstrap.sh --prefix=/home/xus/boost 
./b2 install --prefix=/home/xus/boost
安装路径设置到环境变量中  

linux 上boost设置python环境  
./bootstrap.sh --with-python=/home/xus/anaconda3/envs/env/bin/python --with-python-version=3.8 --with-python-root=/home/xus/anaconda3/envs/env --prefix=/home/xus/boost  
./bootstrap.sh --with-python-root=/usr/local/python3.8 --prefix=/home/xus/boost  
./bootstrap.sh --with-python=/usr/local/python3.8/python3
./bootstrap.sh --with-python=/usr/local/python3.8/python3 --with-python-version=3.8 --with-python-root=/usr/local/python3.8 --prefix=/home/xus/boost  
./bootstrap.sh --with-python-version=3.8 --prefix=/home/xus/boost  

./bootstrap.sh --with-python=/home/xus/anaconda3/envs/env/bin/python --with-python-version=3.8 --with-python-root=/home/xus/anaconda3/envs/env --prefix=/home/xus/
./bootstrap.sh --with-python=python3 --with-python-root=/usr/local/python3.8/bin/python3.8 --with-python-version=3.8 --prefix=/home/xus/boost  

./b2 install --prefix=/home/xus/boost  --with-python

https://github.com/lcit/ext3DLBP/issues/2

linux 安装xlib  
sudo apt-get install libx11-dev  
Ubuntu 在登录界面，点击设置按钮Ubuntu on Xorg上运行才能使用截图  


x11/extensions  
sudo apt-get install libxtst-dev  
sudo apt-get install libxext-dev  

安装zlib
sudo apt-get install zlib1g-dev
