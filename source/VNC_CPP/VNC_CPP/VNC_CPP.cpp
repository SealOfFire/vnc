// VNC_CPP.cpp: 定义应用程序的入口点。
//

//#include <boost/lambda/lambda.hpp> // 不需要
#include <boost/asio.hpp>
#include <iostream>
//#include <iterator> // 不需要
//#include <algorithm> // 不需要
#include "logger.h"

//#include "Common.cpp"

//#include "VNCService.cpp"
//#include "template.hpp"
#include "VNCService.h"


using namespace std;
//using namespace boost; // 不需要
//using namespace boost::asio; // 不需要
//using namespace boost::log;
//using tcp = boost::asio::ip::tcp;
#define BOOST_LOG_DYN_LINK 1

int main()
{
    initLogger();

#ifdef WIN
    BOOST_LOG_TRIVIAL(debug) << "运行平台:win";
#endif
#ifdef LINUX
    BOOST_LOG_TRIVIAL(debug) << "运行平台:linux";
#endif

    //BOOST_LOG_SEV(lg, trivial::info) << "Simple use of Boost log";
    VNCService service;
    //service.start(5900, "123456");
    service.startRepeater("456", "DESKTOP-WORK", 5500, "123456");

    //VNCService service;
    //service.startRepeater("123", "DESKTOP-WORK", 5500, "123456");
    //service.startRepeater("http://www.baidu.com", "123456");
    //service.start(5900, "123456");

}

//https://blog.csdn.net/weixin_42700740/article/details/124647454
