// 日志
// #include "logger.h"
#include <boost/asio.hpp>

#include "Common.h"

// 图像处理和键鼠处理
#ifdef WIN

#include "screen_win.h"

#endif
#ifdef LINUX
#include "screen_linux.h"
#endif

// 客户端消息
#define MessageType_SetPixelFormat (U8)0 // 设置显示的像素格式
#define MessageType_SetEncodings (U8)2 // 客户端支持的帧格式
#define MessageType_FramebufferUpdateRequest (U8)3 // 请求更新帧
#define MessageType_KeyEvent (U8)4  // 键盘事件
#define MessageType_PointerEvent (U8)5 // 鼠标事件
#define MessageType_ClientCutText (U8)6 // 剪切板数据
// 服务器消息
#define MessageType_FramebufferUpdate (U8)0 // 服务器发送帧数据

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
    void startRepeater(const char* id, const char* host, int port, const char* password);

    /*
     * 启动服务
     * port 接听的端口
     * password 连接服务使用的密码
     */
    void start(int port, const char* password);

    static std::string compress2(const std::string& data);

    static std::string decompress2(const std::string& cipher_text);

private:

    /* 客户端支持的编码格式列表 */
    S32* encodings;

    /**
     * socket 初始化设置
     * isRepeater 是否连接到中继
     * io_context
     * endpoint
     */
    void initSocket(boost::asio::io_context& io_context, boost::asio::ip::tcp::endpoint& endpoint, const char* password,
        const char* id);

    /*
     * 找到域名对应的地址
     */
    bool findEndpoint(boost::asio::io_context& io_context, const char* host, int port,
        boost::asio::ip::tcp::endpoint& endpoint);

    /*
     * id注册到中继器
     */
    void writeRepeaterID(boost::asio::ip::tcp::socket& socket, const char* id);

    /**/
    void protocolVersion(boost::asio::ip::tcp::socket& socket);

    /**/
    void security(boost::asio::ip::tcp::socket& socket, const char* password);

    /**/
    void initialisationMessages(boost::asio::ip::tcp::socket& socket);

    /**/
    void serviceMessageLoop(boost::asio::ip::tcp::socket& socket, int timeout);

    /**/
    void setPixelFormat(boost::asio::ip::tcp::socket& socket);

    /**/
    void setEncodings(boost::asio::ip::tcp::socket& socket);

    /**/
    void framebufferUpdateRequest(boost::asio::ip::tcp::socket& socket);

    /**/
    void framebufferUpdate(boost::asio::ip::tcp::socket& socket, U16 xPosition, U16 yPosition, U16 width, U16 height);

    /**/
    void keyEvent(boost::asio::ip::tcp::socket& socket);

    /**/
    void pointerEvent(boost::asio::ip::tcp::socket& socket);

    /**/
    void clientCutText(boost::asio::ip::tcp::socket& socket);

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
    int zlibEncoding(unsigned char* frameData, int size, char*& output);

    /**
     * 压缩帧数据
     * source 元数据
     * sourceLength 元数据的长度
     * compressData 压缩后数据
     * compressLength 压缩后数据长度
     */
    static void compressFrameData(BYTE* source, U32 sourceLength, BYTE*& compressData, U32* compressLength);
};