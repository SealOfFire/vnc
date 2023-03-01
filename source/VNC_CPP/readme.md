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

https://blog.csdn.net/qq_36163982/article/details/122438593  

linux 安装boost  
sudo apt-get install libboost-all-dev  

linux 安装xlib  
sudo apt-get install libx11-dev  
Ubuntu 在登录界面，点击设置按钮Ubuntu on Xorg上运行才能使用截图  


x11/extensions  
sudo apt-get install libxtst-dev  
sudo apt-get install libxext-dev  
