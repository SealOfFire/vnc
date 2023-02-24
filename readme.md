####VNC协议的服务器端实现  
协议格式参考 https://github.com/rfbproto/rfbproto/blob/master/rfbproto.rst  
C# 部分代码参考了 https://github.com/T1T4N/NVNC
目前只处理了  

|消息类型|名称|
|:----|:----|
|0|SetPixelFormat|
|2|SetEncodings|
|3|FramebufferUpdateRequest|
|4|KeyEvent|
|5|PointerEvent|
|6|ClientCutText|
这几个必须实现的协议  
图像使用Raw Encoding编码  

####项目路径文件说明
source  
&ensp;&ensp;├ VNC C#代码的服务器端 只能用在windows系统上  
&ensp;&ensp;└ VNC_CPP C++版的服务端，可以在windows和ubuntu上测试过可用  

#####C#版代码
日志使用的nlog。  
的图像处理使用的gdi+，鼠标键盘操作引用的user32.dll

#####C++版的代码
项目设置是cmake  
socket通信使用的boost/asio  
日志使用的boost/log  
windows上图像处理使用的GDI，键盘鼠标操作使用的winpai的WinUser.h中方法，和C#版实际上使用的是相同的方式。  
ubuntu版使用的图像处理使用的xlib（需要ubuntu启动时选择xorg）。键盘鼠标操作也是xlib。

支持UltraVNC view和UltraVNC Repeater

