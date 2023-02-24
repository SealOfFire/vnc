#include "VNCService.h"
#include <string>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>
//#include <boost/log/common.hpp>
//#include <boost/log/sinks.hpp>
//#include <boost/log/sources/logger.hpp>
//#include <boost/log/sources/severity_logger.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/format.hpp>
//#include <boost/lexical_cast.hpp>

#include <des.h>
#include <cryptlib.h>
#include <modes.h>
#include <filters.h>

#include "logger.h"
#include "PixelFormat.h"
#ifdef WIN
    #include "screen_win.cpp"
#endif
#ifdef LINUX
#include "screen_linux.cpp"
#endif

using namespace CryptoPP;
using namespace std;
//using namespace boost::log;
using tcp = boost::asio::ip::tcp;

class VNCService {

private:
    int port;
    string password;

    // 复制桌面处理
    // DulicationManager duplMgr;

    /*
     * 启动服务
     * port 监听端口号
     * 接入时的密码
     */
public:

    ~VNCService() {
        // 释放屏幕处理资源
        release();
    }

    /*
    * 连接到中继
    */
    void startRepeater(const char* id, const char* host, int port, const char* password) {
        BOOST_LOG_TRIVIAL(info) << "启动vnc服务，连接到中继";

        boost::asio::io_context ioContext;
        tcp::resolver ioResolver(ioContext);
        //tcp::resolver::query ioQuery(boost::asio::ip::tcp::v4(), host, "https");
        tcp::resolver::query ioQuery(host, boost::lexical_cast<string>(port));
        boost::system::error_code ec;
        tcp::resolver::results_type ioResults = ioResolver.resolve(ioQuery, ec);

        if (ec)
        {
            BOOST_LOG_TRIVIAL(error) << ec.message();
        }

        tcp::endpoint endpoint;
        BOOST_LOG_TRIVIAL(debug) << "启动vnc服务==size:[" << ioResults.size() << "]";
        for (tcp::resolver::iterator iter = ioResults.begin(); iter != ioResults.end(); ++iter)
        {
            BOOST_LOG_TRIVIAL(debug) << "启动vnc服务==[" << iter->endpoint().address().to_string() << "]";
            if (iter->endpoint().address().is_v4())
            {
                endpoint = iter->endpoint();
                break;
            }
        }

        if (ioResults.size() == 0)
        {
            BOOST_LOG_TRIVIAL(warning) << "启动vnc服务，连接到中继。没有找到中继器地址";
            return;
        }

        int count = 0; // 重新连接的次数
        while (true) {
            BOOST_LOG_TRIVIAL(info) << "重连次数" << count;
            count++;
            try {
                tcp::socket socket(ioContext);
                BOOST_LOG_TRIVIAL(info) << "启动vnc服务，连接到中继"<< endpoint.address();
                socket.connect(endpoint);
                BOOST_LOG_TRIVIAL(info) << "启动vnc服务，连接到中继。连接到服务器";

                this->writeRepeaterID(socket, id);
                this->protocolVersion(socket);
                this->security(socket, password);
                this->initialisationMessages(socket);
                this->serviceMessageLoop(socket, 10000);
            }
            catch (const std::exception& e)
            {
                BOOST_LOG_TRIVIAL(error) << "Exception:" << e.what();
            }
            catch (const char* err)
            {
                BOOST_LOG_TRIVIAL(error) << "Exception:" << err;
            }
        }

    }

    void start(int port, const char* password) {
        //BOOST_LOG_TRIVIAL(trace) << "启动vnc服务";
        //BOOST_LOG_TRIVIAL(debug) << "启动vnc服务";
        BOOST_LOG_TRIVIAL(info) << "启动vnc服务";
        //BOOST_LOG_TRIVIAL(warning) << "启动vnc服务";
        //BOOST_LOG_TRIVIAL(error) << "启动vnc服务";
        //BOOST_LOG_TRIVIAL(fatal) << "启动vnc服务";

        //this->duplMgr.GetFrame();
        //this->duplMgr.InitDX();
        //this->duplMgr.InitDupl();
        //this->duplMgr.GetFrame();

        this->port = port;
        this->password = password;

        int count = 0; // 重新连接的次数

        // 创建socket
        //io_context io;
        //ip::tcp::socket socket(io);

        //unsigned short port = 5900;
        boost::asio::io_context iocontext;

        while (true)
        {
            BOOST_LOG_TRIVIAL(info) << "重连次数" << count;
            count++;
            try
            {
                // 等待连接接入
                tcp::acceptor acceptor(iocontext, tcp::endpoint(tcp::v4(), port));
                // tcp::socket socket(iocontext);
                tcp::socket socket = acceptor.accept();

                //this->writeRepeaterID(socket, "123");

                this->protocolVersion(socket);
                this->security(socket, password);
                this->initialisationMessages(socket);
                this->serviceMessageLoop(socket, 10000);
            }
            catch (const std::exception& e)
            {
                BOOST_LOG_TRIVIAL(error) << "Exception:" << e.what();
            }
            catch (const char* err)
            {
                BOOST_LOG_TRIVIAL(error) << "Exception:" << err;
            }
        }
    }


private:

    /*
     * id发送到中继器
     */
    void writeRepeaterID(tcp::socket& socket, const char* id) {
        BOOST_LOG_TRIVIAL(info) << "服务器信息:服务器->中继器==id:[" << id << "]";
        std::string s = str(boost::format("ID:%s") % id);
        char data[250] = { '\0' };
        memcpy(data, s.c_str(), sizeof(s.c_str()));
        boost::asio::write(socket, boost::asio::buffer(data, 250));
    }

    /*
     * 发送版本信息
     */
    void protocolVersion(tcp::socket& socket) {
        unsigned char version[] = "RFB 003.008\n";
        BOOST_LOG_TRIVIAL(info) << "版本号:服务器->客户端";
        // 发送版本号
        boost::asio::write(socket, boost::asio::buffer(version, 12));

        // 接受版本号
        char data[12] = { 0 };
        boost::system::error_code ec;
        size_t length = socket.read_some(boost::asio::buffer(data), ec);
        BOOST_LOG_TRIVIAL(info) << "版本号:客户端->服务器==:[" << data << "]";
    }

    /*
     * 安全协议内容
     */
    void security(tcp::socket& socket, const char* password) {
        // 发送安全协议
        BOOST_LOG_TRIVIAL(info) << "安全协议:服务器->客户端==列表";
        // 第一个char是列表数量
        // 从第二个char开始是支持的安全协议种类
        char securityTypes[4];
        securityTypes[0] = 3;
        securityTypes[1] = 0; // Invalid
        securityTypes[2] = 1; // None
        securityTypes[3] = 2; // VNC Authentication
        boost::asio::write(socket, boost::asio::buffer(securityTypes, 12));

        // 读取客户端的选择的协议
        char data[1] = { 0 };
        boost::system::error_code ec;
        size_t length = socket.read_some(boost::asio::buffer(data), ec);
        BOOST_LOG_TRIVIAL(info) << "安全协议:客户端->服务器==种类:[" << (int)data[0] << "]";

        if (data[0] == 2)
        {
            // VNC Authentication
            BOOST_LOG_TRIVIAL(debug) << "安全协议:服务器->客户端==Challenge:[" << "challenge" << "]";
            boost::uuids::random_generator gen;
            boost::uuids::uuid id = gen();
            int i = 0;
            for (char item : id.data)
            {
                BOOST_LOG_TRIVIAL(debug) << "安全协议:服务器->客户端==Challenge " << i << ":[" << (int)item << "]";
                i++;
            }
            boost::asio::write(socket, boost::asio::buffer(id.data, 16));


            // 接收客户端密码，客户端密码用发送的challenge进行des加密
            char data[16] = { 0 };
            size_t length = socket.read_some(boost::asio::buffer(data), ec);
            BOOST_LOG_TRIVIAL(info) << "安全协议:客户端->服务器==密码";
            i = 0;
            for (char item : data)
            {
                BOOST_LOG_TRIVIAL(debug) << "安全协议:客户端->服务器==密码 " << i << ":[" << (int)item << "]";
                i++;
            }

            // 本地密码加密后和客户端发送的密码对比
            unsigned char key[8] = { 0 };
            for (int i = 0; i < 8; i++) {
                //cout << std::strlen(password);
                if (i < std::strlen(password)) {
                    key[i] = password[i];
                }
            }
            // 每个bit反向
            for (int i = 0; i < 8; i++)
            {
                key[i] = ((key[i] & 0xaa) >> 1 | (key[i] & 0x55) << 1);
                key[i] = ((key[i] & 0xcc) >> 2 | (key[i] & 0x33) << 2);
                key[i] = ((key[i] & 0xf0) >> 4 | (key[i] & 0x0f) << 4);
            }

            // DES
            char temp[16];
            for (int i = 0; i < 16; i++)
            {
                temp[i] = id.data[i];
            }
            std::string content = temp;
            std::string cipher;
            try {
                ECB_Mode<DES>::Encryption e;
                e.SetKey(key, DES::DEFAULT_KEYLENGTH);
                StringSource(content, true,
                    new StreamTransformationFilter(e, new StringSink(cipher)));
            }
            catch (const CryptoPP::Exception& e) {
                std::cerr << e.what() << std::endl;
            }

            unsigned int securityResult[1] = { 0 };
            for (int i = 0; i < 16; i++)
            {
                if (data[i] != cipher[i])
                {
                    // 密码错误
                    BOOST_LOG_TRIVIAL(warning) << "安全协议:客户端->服务器==密码错误";
                    securityResult[0] = 1;
                    break;
                }
            }

            // 返回正确信息
            // unsigned int securityResult[1] = { 0 };
            boost::asio::write(socket, boost::asio::buffer(securityResult, 4));
        }
    }

    /*
     * 初始化数据
     */
    void initialisationMessages(tcp::socket& socket) {
        BOOST_LOG_TRIVIAL(info) << "初始化:客户端->服务器";
        // 接收clientInit
        char sharedFlag[1] = { 0 };
        boost::system::error_code ec;
        size_t length = socket.read_some(boost::asio::buffer(sharedFlag), ec);
        BOOST_LOG_TRIVIAL(debug) << "初始化:客户端->服务器==shared flag:[" << (int)sharedFlag[0] << "]";

        // 发送serverInit
        BOOST_LOG_TRIVIAL(info) << "初始化:服务器->客户端";
        unsigned char buffer[24] = {};

        unsigned short framebufferWidth = getWidth();
        unsigned short framebufferHeight = getHeight();
        CopyToArray<unsigned short>(framebufferWidth, buffer, 0);
        CopyToArray<unsigned short>(framebufferHeight, buffer, 2);
        pixelFormat pf;
        unsigned char bufferPF[16] = { 0 };
        pixelFormatToBuffer(pf, bufferPF);
        memcpy(buffer + 4, bufferPF, 16);
        std::string name = "test nvc service";
        unsigned int nameLength = name.length();
        // 名字长度部分
        CopyToArray<unsigned int>(nameLength, buffer, 20);
        //memcpy(buffer + 20, &nameLength, 4);

        boost::asio::write(socket, boost::asio::buffer(buffer, 24));
        boost::asio::write(socket, boost::asio::buffer(name.c_str(), nameLength));
    }

    void serviceMessageLoop(tcp::socket& socket, int timeout) {
        BOOST_LOG_TRIVIAL(info) << "客户端发送的消息循环:客户端->服务器";

        boost::system::error_code ec;
        char type[1] = { -1 };

        boost::posix_time::ptime time_begin, time_end;
        boost::posix_time::millisec_posix_time_system_config::time_duration_type time_elapse;
        while (true)
        {
            size_t length = socket.read_some(boost::asio::buffer(type), ec);
            if (length > 0) {
                time_begin = boost::posix_time::second_clock::universal_time();
                BOOST_LOG_TRIVIAL(debug) << "客户端发送的消息循环:客户端->服务器:[" << (int)type[0] << "],length:[" << length << "]";
                switch (type[0])
                {
                case 0:
                    this->setPixelFormat(socket);
                    break;
                case 2:
                    this->setEncodings(socket);
                    break;
                case 3:
                    this->framebufferUpdateRequest(socket);
                    break;
                case 4:
                    this->keyEvent(socket);
                    break;
                case 5:
                    this->pointerEvent(socket);
                    break;
                case 6:
                    this->clientCutText(socket);
                    break;
                default:
                    BOOST_LOG_TRIVIAL(warning) << "未接入的客户端消息处理:客户端->服务器:[" << (int)type[0] << "]";
                    break;
                }
            }
            else {
                // TODO
                // 没有数据,计算超时
                time_end = boost::posix_time::second_clock::universal_time();
                time_elapse = time_end - time_begin;
                if (time_elapse.total_seconds() > 5) {
                    //timer.stop();
                    // timer.start();
                    BOOST_LOG_TRIVIAL(warning) << "长时间没有输入连接:客户端->服务器:[" << time_elapse.total_seconds() << "]秒";
                    throw ("长时间没有接入信息。重新连接");
                }
            }
        }
    }

    void setPixelFormat(tcp::socket& socket) {
        BOOST_LOG_TRIVIAL(info) << "SetPixelFormat";

        unsigned   char data[19] = { 0 };
        boost::system::error_code ec;
        size_t length = socket.read_some(boost::asio::buffer(data), ec);
        int i = 0;

        unsigned char padding[3] = { 0 };
        for (int i = 0; i < 3; i++) {
            padding[i] = data[i];
            BOOST_LOG_TRIVIAL(debug) << "SetPixelFormat:客户端->服务器==pixel-format->padding " << i << ":[" << (int)data[i] << "]";
        }
        unsigned char bitsPerPixel = data[3];
        unsigned char depth = data[4];
        unsigned char bigEndianFlag = data[5];
        unsigned char trueColourFlag = data[6];
        unsigned short redMax = ArrayToValue<unsigned short>(data, 7);
        unsigned short greenMax = ArrayToValue<unsigned short>(data, 9);
        unsigned short blueMax = ArrayToValue<unsigned short>(data, 11);
        unsigned char redShift = data[13];
        unsigned char greenShift = data[14];
        unsigned char blueShift = data[15];
        BOOST_LOG_TRIVIAL(debug) << "SetPixelFormat:客户端->服务器==pixel-format->bits-per-pixel:[" << (int)bitsPerPixel << "]";
        BOOST_LOG_TRIVIAL(debug) << "SetPixelFormat:客户端->服务器==pixel-format->depth:[" << (int)depth << "]";
        BOOST_LOG_TRIVIAL(debug) << "SetPixelFormat:客户端->服务器==pixel-format->big-endian-flag:[" << (int)bigEndianFlag << "]";
        BOOST_LOG_TRIVIAL(debug) << "SetPixelFormat:客户端->服务器==pixel-format->true-colour-flag:[" << (int)trueColourFlag << "]";
        BOOST_LOG_TRIVIAL(debug) << "SetPixelFormat:客户端->服务器==pixel-format->red-max:[" << redMax << "]";
        BOOST_LOG_TRIVIAL(debug) << "SetPixelFormat:客户端->服务器==pixel-format->green-max:[" << greenMax << "]";
        BOOST_LOG_TRIVIAL(debug) << "SetPixelFormat:客户端->服务器==pixel-format->blue-max:[" << blueMax << "]";
        BOOST_LOG_TRIVIAL(debug) << "SetPixelFormat:客户端->服务器==pixel-format->red-shift:[" << (int)redShift << "]";
        BOOST_LOG_TRIVIAL(debug) << "SetPixelFormat:客户端->服务器==pixel-format->green-shift:[" << (int)greenShift << "]";
        BOOST_LOG_TRIVIAL(debug) << "SetPixelFormat:客户端->服务器==pixel-format->blue-shift:[" << (int)blueShift << "]";
    }

    void setEncodings(tcp::socket& socket) {
        BOOST_LOG_TRIVIAL(info) << "SetEncodings";

        unsigned char data[3] = { 0 };
        boost::system::error_code ec;
        size_t length = socket.read_some(boost::asio::buffer(data), ec);
        BOOST_LOG_TRIVIAL(debug) << "SetEncodings:客户端->服务器==padding:[" << (int)data[0] << "]";

        unsigned short numberOfEncodings = ArrayToValue<unsigned short>(data, 1);
        BOOST_LOG_TRIVIAL(debug) << "SetEncodings:客户端->服务器==number-of-encodings:[" << numberOfEncodings << "]";

        for (int i = 0; i < numberOfEncodings; i++)
        {
            unsigned char buffer[4] = { 0 };
            length = socket.read_some(boost::asio::buffer(buffer), ec);
            int value = ArrayToValue<int>(buffer, 0);
            BOOST_LOG_TRIVIAL(debug) << "SetEncodings:客户端->服务器==encoding-type " << i << ":[" << value << "]";
        }
    }

    /*
     * 请求一帧画面
     */
    void framebufferUpdateRequest(tcp::socket& socket) {
        BOOST_LOG_TRIVIAL(info) << "FramebufferUpdateRequest";

        unsigned char data[9] = { 0 };
        boost::system::error_code ec;
        size_t length = socket.read_some(boost::asio::buffer(data), ec);

        unsigned char incremental = data[0];
        unsigned short xPosition = ArrayToValue<unsigned short>(data, 1);
        unsigned short yPosition = ArrayToValue<unsigned short>(data, 3);
        unsigned short width = ArrayToValue<unsigned short>(data, 5);
        unsigned short height = ArrayToValue<unsigned short>(data, 7);

        BOOST_LOG_TRIVIAL(debug) << "incremental:[" << (int)incremental << "]";
        BOOST_LOG_TRIVIAL(debug) << "x-position:[" << xPosition << "]";
        BOOST_LOG_TRIVIAL(debug) << "x-position:[" << yPosition << "]";
        BOOST_LOG_TRIVIAL(debug) << "width:[" << width << "]";
        BOOST_LOG_TRIVIAL(debug) << "height:[" << height << "]";

        this->framebufferUpdate(socket, xPosition, yPosition, width, height);
    }

    /**
     * 发送一帧画面数据
     */
    void framebufferUpdate(tcp::socket& socket, unsigned short xPosition, unsigned short yPosition,
        unsigned short width, unsigned short height) {
        BOOST_LOG_TRIVIAL(info) << "framebufferUpdate:服务器->客户端";

        // 图片的容量
        int frameSize = width * height * (32 / 8);
        unsigned char* frameData = new  unsigned char[frameSize];
        BOOST_LOG_TRIVIAL(debug) << "framebufferUpdate:服务器->客户端:frameSize[" << frameSize << "]";

        // 屏幕图像处理
        ScreenShort(xPosition, yPosition, width, height, frameData);

        unsigned char buffer[4] = { 0 };
        buffer[0] = 0; // message-type
        buffer[1] = 0; //padding
        unsigned short numberOfRectangles = 1;
        CopyToArray<unsigned short>(numberOfRectangles, buffer, 2);

        //unsigned short xPosition = xPosition;
        //unsigned short yPosition = yPosition;
        //unsigned short width = width;
        //unsigned short height = height;
        int encodingType = 0; // RAW编码
        unsigned char bufferRectangle[12] = { 0 };
        CopyToArray<unsigned short>(xPosition, bufferRectangle, 0);
        CopyToArray<unsigned short>(yPosition, bufferRectangle, 2);
        CopyToArray<unsigned short>(width, bufferRectangle, 4);
        CopyToArray<unsigned short>(height, bufferRectangle, 6);
        CopyToArray<int>(encodingType, bufferRectangle, 8);
        //
        boost::asio::write(socket, boost::asio::buffer(buffer, 4));
        for (int i = 0; i < numberOfRectangles; i++)
        {
            // 格式说明
            boost::asio::write(socket, boost::asio::buffer(bufferRectangle, 12));
            // 帧数据
            boost::asio::write(socket, boost::asio::buffer(frameData, frameSize));
        }

        delete[] frameData;
    }

    void keyEvent(tcp::socket& socket) {
        BOOST_LOG_TRIVIAL(info) << "KeyEvent";

        unsigned char data[7] = { 0 };
        boost::system::error_code ec;
        size_t length = socket.read_some(boost::asio::buffer(data), ec);

        unsigned char downFlag = data[0];
        unsigned short padding = ArrayToValue<unsigned short>(data, 1);
        unsigned int key = ArrayToValue<unsigned int>(data, 3);

        BOOST_LOG_TRIVIAL(debug) << "down-flag:[" << (int)downFlag << "]";
        BOOST_LOG_TRIVIAL(debug) << "padding:[" << padding << "]";
        BOOST_LOG_TRIVIAL(debug) << "key:[" << key << "]";

        // 键盘输入处理
        keyHandle(downFlag, key);
    }

    void pointerEvent(tcp::socket& socket) {
        BOOST_LOG_TRIVIAL(info) << "PointerEvent";

        unsigned char data[5] = { 0 };
        boost::system::error_code ec;
        size_t length = socket.read_some(boost::asio::buffer(data), ec);

        unsigned char buttonMask = data[0];
        unsigned short xPosition = ArrayToValue<unsigned short>(data, 1);
        unsigned short yPosition = ArrayToValue<unsigned short>(data, 3);

        BOOST_LOG_TRIVIAL(debug) << "button-mask:[" << (int)buttonMask << "]";
        // 蒙板转成bit数组
        int buttons[8] = {
            (buttonMask & 0x01) >> 0,
            (buttonMask & 0x02) >> 1,
            (buttonMask & 0x04) >> 2,
            (buttonMask & 0x08) >> 3,
            (buttonMask & 0x10) >> 4,
            (buttonMask & 0x20) >> 5,
            (buttonMask & 0x40) >> 6,
            (buttonMask & 0x80) >> 7
        };
        int i = 0;
        for (bool v : buttons)
        {
            BOOST_LOG_TRIVIAL(debug) << "button-mask " << i << ":[" << v << "]";
            i++;
        }
        BOOST_LOG_TRIVIAL(debug) << "x-position:[" << xPosition << "]";
        BOOST_LOG_TRIVIAL(debug) << "y-position:[" << yPosition << "]";

        pointerHandle(buttons, xPosition, yPosition);
    }

    void clientCutText(tcp::socket& socket) {
        BOOST_LOG_TRIVIAL(info) << "ClientCutText";
    }

};

// https://learn.microsoft.com/zh-cn/windows/win32/gdi/capturing-an-image
// https://github.com/microsoft/Windows-classic-samples/tree/main/Samples/DXGIDesktopDuplication
