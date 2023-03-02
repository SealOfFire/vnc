#include <iostream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/zlib.hpp>
//zlib压缩
//#include "zlib/include/zlib.h"

// des加密
#include <des.h>
#include <cryptlib.h>
#include <modes.h>
#include <filters.h>
// 日志
#include "logger.h"

#include "Common.h"
#include "PixelFormat.h"

// 图像处理和键鼠处理
#ifdef WIN

#include "screen_win.h"

#endif
#ifdef LINUX
#include "screen_linux.h"
#endif


/**
 * vnc服务处理
 */
class VNCService {
public:

    /**
     * 图像的编码格式
     */
    enum Encodings : S32 {
        Raw = 0,
        CopyRect = 1,
        REF = 2,
        CoREF = 4,
        Hextile = 5,
        zlib = 6,
        Tight = 7,
        zlibhex = 8,
        ZRLE = 16,
        JPEG = 21,
        OpenH264 = 50,
        TightPNG = -260
    };

    /**
     * 构造函数
     */
    VNCService();

    /**
     * 析构函数
     * 释放截图处理和键鼠操作使用的资源
     */
    ~VNCService();

    /*
     * 启动服务连接到中继
     * id 中继器显示的id
     * host 中继器的地址
     * port 接听的端口
     * password 连接服务使用的密码
     */
    void startRepeater(const char *id, const char *host, int port, const char *password);

    /*
     * 启动服务
     * port 接听的端口
     * password 连接服务使用的密码
     */
    void start(int port, const char *password);

    static std::string compress(const std::string &data);

    static std::string decompress(const std::string &cipher_text);

private:
    /**
     * socket 初始化设置
     * isRepeater 是否连接到中继
     * io_context
     * endpoint
     */
    void initSocket(boost::asio::io_context &io_context, boost::asio::ip::tcp::endpoint &endpoint, const char *password,
                    const char *id);

    /*
     * 找到域名对应的地址
     */
    bool findEndpoint(boost::asio::io_context &io_context, const char *host, int port,
                      boost::asio::ip::tcp::endpoint &endpoint);

    /*
     * id注册到中继器
     */
    void writeRepeaterID(boost::asio::ip::tcp::socket &socket, const char *id);

    /**/
    void protocolVersion(boost::asio::ip::tcp::socket &socket);

    /**/
    void security(boost::asio::ip::tcp::socket &socket, const char *password);

    /**/
    void initialisationMessages(boost::asio::ip::tcp::socket &socket);

    /**/
    void serviceMessageLoop(boost::asio::ip::tcp::socket &socket, int timeout);

    /**/
    void setPixelFormat(boost::asio::ip::tcp::socket &socket);

    /**/
    void setEncodings(boost::asio::ip::tcp::socket &socket);

    /**/
    void framebufferUpdateRequest(boost::asio::ip::tcp::socket &socket);

    /**/
    void framebufferUpdate(boost::asio::ip::tcp::socket &socket, U16 xPosition, U16 yPosition, U16 width, U16 height);

    /**/
    void keyEvent(boost::asio::ip::tcp::socket &socket);

    /**/
    void pointerEvent(boost::asio::ip::tcp::socket &socket);

    /**/
    void clientCutText(boost::asio::ip::tcp::socket &socket);

    /**
     * raw格式编码
     */
    void RawEncoding();

    /**
     * zilb格式编码
     * buffer raw格式的原始数据
     * output 压缩后的数据
     * return 压缩后数据的字节数
     */
    int zlibEncoding(unsigned char *frameData, int size, char *&output);
};