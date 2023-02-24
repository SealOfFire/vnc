using Microsoft.Extensions.Logging;
using NLog;
using System.Diagnostics;
using System.Drawing;
using System.Net;
using System.Net.Sockets;
using System.Reflection;
using System.Security.Cryptography;
using X11;

namespace VNC
{
    public class Service
    {
        // private readonly ILogger<Service> logger;
        public static Logger logger = LogManager.GetCurrentClassLogger();

        private TcpListener listener;
        private TcpClient client;
        private NetworkStream stream;
        private Avatar avatar = new Avatar();
        private string password;
        private int port = 5900;

        private ushort width = 1920;
        private ushort height = 1080;

        private struct PoxelFormat
        {
            byte bitsPerPixel = 32;
            byte depth = 24;
            bool bigEndinaFlag = false;
            bool tureColorFlag = true;
            ushort redMax = 0xFF;
            ushort greenMax = 0xFF;
            ushort blueMax = 0xFF;
            byte redShift = 16;
            byte greenShift = 8;
            byte blueShift = 0;
            byte[] padding = new byte[3];

            public PoxelFormat()
            {
            }

            public byte[] ToBytes()
            {
                byte[] bytes = new byte[16];
                bytes[0] = this.bitsPerPixel;
                bytes[1] = this.depth;
                bytes[2] = Convert.ToByte(this.bigEndinaFlag);
                bytes[3] =Convert.ToByte(this.tureColorFlag);

                this.redMax.CopyToArray(bytes, 4);
                this.greenMax.CopyToArray(bytes, 6);
                this.blueMax.CopyToArray(bytes, 8);

                bytes[10] =redShift;
                bytes[11] =greenShift;
                bytes[12] =blueShift;
                bytes[13] =this.padding[0];
                bytes[14] =this.padding[1];
                bytes[15] =this.padding[2];

                return bytes;
            }
        }

        //public Service()
        //{
        //    //ILogger logger = LogManager.GetLogger();
        //    //NLog.Logger Log = NLog.LogManager.GetCurrentClassLogger();
        //    // Log.Debug();

        //    // Logger logger= LogManager.GetLogger("*");
        //    //this.logger = LogManager.GetLogger("*");
        //}

        //public Service(ILogger<Service> logger)
        //{
        //    //this.logger=logger;
        //}

        /// <summary>
        /// 
        /// </summary>
        /// <param name="port"></param>
        /// <param name="password"></param>
        /// <param name="timeout">毫秒</param>
        public void Start(int port, string password, int timeout)
        {
            logger.Info($"启动vnc服务，监听端口{port}");

            this.password=password;
            this.port = port;

            this.listener = new TcpListener(IPAddress.Any, this.port);
            this.listener.Start();

            int count = 0;
            // 断开连接后不终止程序，继续监听端口的连接
            while (true)
            {
                logger.Info($"重连次数{count}");
                count++;
                try
                {
                    this.Lister(timeout);
                }
                catch (SocketException e)
                {
                    logger.Error("SocketException:{0}", e);
                    //logger.Warn($"关闭连接");
                    //this.client.Close();
                }
                catch (IOException e)
                {

                    logger.Error("IOException:{0}", e);
                    if (e.InnerException is SocketException)
                    {
                        int code = ((SocketException)e.InnerException).NativeErrorCode;
                        if (code == 10053)
                        {
                            // 断开连接
                        }
                    }
                    //logger.Warn($"关闭连接");
                    //this.client.Close();
                }
                finally
                {

                }
            }
        }

        /// <summary>
        /// 监听端口的数据
        /// </summary>
        private void Lister(int timeout)
        {
            logger.Info("等待客户端接入");
            this.client = this.listener.AcceptTcpClient();
            //this.client.ReceiveTimeout=10000; // 等待接收的超时时间
            logger.Info("客户端已连接");

            this.stream = client.GetStream();

            // 交互版本信息
            this.ProtocolVersion();

            // 安全协议
            this.Security();

            // 初始化信息
            this.InitialisationMessages();

            // 处理客户端消息循环
            this.ServiceMessageLoop(timeout);
        }

        /// <summary>
        /// 和客户端交互版本信息
        /// </summary>
        private void ProtocolVersion()
        {
            // 发送版本号
            logger.Info("版本号:服务器->客户端");
            byte[] protocolVersion = System.Text.Encoding.ASCII.GetBytes(string.Format("RFB 003.008\n"));
            this.stream.Write(protocolVersion);

            // 接受版本号
            byte[] buffer = this.stream.ReadBuffer(12);
            string clientProtocolVersion = System.Text.Encoding.ASCII.GetString(buffer);
            logger.Info($"版本号:客户端->服务器:[{clientProtocolVersion}]");
        }

        /// <summary>
        /// 安全协议内容
        /// </summary>
        private void Security()
        {
            // 发送安全协议
            logger.Info("安全协议:服务器->客户端:列表");
            byte[] types = { 0, 1, 2 }; // Security Types
            this.stream.WriteByte((byte)types.Length);
            for (int i = 0; i < types.Length; i++)
            {
                this.stream.WriteByte(types[i]);
            }

            // 读取客户端选择的协议
            byte[] buffer = new byte[1];
            int length = this.stream.Read(buffer);
            byte clientSecurityType = buffer[0];
            logger.Info($"安全协议:客户端->服务器:种类[{clientSecurityType}]");

            bool authOK = true;
            if (clientSecurityType == 2)
            {
                // 接收密码
                // 16字节随机challenge
                byte[] bChallenge = new byte[16];
                bChallenge = Guid.NewGuid().ToByteArray();
                //Random rand = new Random(DateTime.Now.Millisecond);
                //rand.NextBytes(bChallenge);
                logger.Info($"安全协议:服务器->客户端:Challenge[{bChallenge}]");
                foreach (byte item in bChallenge)
                {
                    logger.Debug($"安全协议:客户端->服务器:Challenge[{item}]");
                }
                this.stream.Write(bChallenge);
                this.stream.Flush();

                // 接收客户端密码，客户端密码用发送的challenge进行des加密
                buffer = new byte[16];
                length = this.stream.Read(buffer);
                //byte[] clientPassword = reader.ReadBytes(16);
                logger.Info($"安全协议:客户端->服务器:密码[{buffer}]");
                foreach (byte item in buffer)
                {
                    logger.Debug($"安全协议:客户端->服务器:密码[{item}]");
                }

                // 本地密码加密后和客户端发送的密码对比
                byte[] key = new byte[8];
                // Key limited to 8 bytes max.
                if (password.Length >= 8)
                {
                    System.Text.Encoding.ASCII.GetBytes(password, 0, 8, key, 0);
                }
                else
                {
                    System.Text.Encoding.ASCII.GetBytes(password, 0, password.Length, key, 0);
                }

                // 每个bit反向
                for (int i = 0; i < 8; i++)
                {
                    key[i]=(byte)((key[i] & 0xaa)>>1 | (key[i] & 0x55)<<1);
                    key[i]=(byte)((key[i] & 0xcc)>>2 | (key[i] & 0x33)<<2);
                    key[i]=(byte)((key[i] & 0xf0)>>4 | (key[i] & 0x0f)<<4);
                }

                // DES des = new DESCryptoServiceProvider();
                DES des = DES.Create();
                des.Padding = PaddingMode.None;
                des.Mode = CipherMode.ECB;
                ICryptoTransform enc = des.CreateEncryptor(key, null);
                byte[] ourBytes = new byte[16];
                enc.TransformBlock(bChallenge, 0, bChallenge.Length, ourBytes, 0);

                logger.Info($"安全协议:比对密码");
                for (int i = 0; i < ourBytes.Length; i++)
                {
                    if (buffer[i] != ourBytes[i])
                    {
                        authOK = false;
                        break;
                    }
                }
            }

            // SecurityResult
            // 发送安全结果
            // TODO 密码验证成功
            uint ok = 0;
            if (authOK)
            {
                logger.Info($"安全协议:密码正确");
                ok = 0;
            }
            else
            {
                logger.Warn($"安全协议:密码错误");
                ok = 1;
            }
            this.stream.Write(BitConverter.GetBytes(ok));
            this.stream.Flush();
        }

        /// <summary>
        /// 初始化数据
        /// </summary>
        private void InitialisationMessages()
        {
            logger.Info($"初始化:客户端->服务器");
            //byte[] buffer = new byte[1];

            // 接收clientInit
            //int length = this.stream.Read(buffer);
            byte sharedFlag = this.stream.ReadU8();
            // byte clientInitSharedFlag = buffer[0];
            logger.Info($"初始化:客户端->服务器:shared flag[{sharedFlag}]");

            // 发送serverInit
            logger.Info($"初始化:服务器->客户端");
            logger.Info($"初始化:服务器->客户端:屏幕宽:[{this.width}]");
            logger.Info($"初始化:服务器->客户端:屏幕高:[{this.height}]");
            ushort frameBufferWidth = this.width; // 屏幕的宽
            ushort frameBufferHeight = this.height; // 屏幕的高

            // 像素格式
            PoxelFormat pf = new PoxelFormat();
            byte[] pfBuffer = pf.ToBytes();

            // 桌面名字长度
            byte[] nameString = System.Text.Encoding.ASCII.GetBytes(string.Format("test"));
            uint nameLength = (uint)nameString.Length; // 名字字符串

            byte[] buffer = new byte[2+2+16+4];
            frameBufferWidth.CopyToArray(buffer, 0);
            frameBufferHeight.CopyToArray(buffer, 2);
            Array.Copy(pfBuffer, 0, buffer, 4, pfBuffer.Length);
            nameLength.CopyToArray(buffer, 20);

            this.stream.Write(buffer);
            this.stream.Write(nameString);
            this.stream.Flush();
        }

        /// <summary>
        /// 接收处理客户端消息
        /// </summary>
        private void ServiceMessageLoop(int timeout)
        {
            logger.Info($"客户端发送的消息循环:客户端->服务器");
            int i;
            byte[] type = new byte[1];
            Stopwatch t = Stopwatch.StartNew();
            while (true)
            {
                if (this.stream.DataAvailable)
                {
                    t.Restart();
                    i = this.stream.Read(type, 0, 1);
                    // Console.WriteLine($"Received length: {i}");
                    if (i>0)
                    {
                        logger.Debug($"客户端发送的消息循环:客户端->服务器:[{type[0]}]");
                        switch (type[0])
                        {
                            case 0:
                                // SetPixelFormat
                                this.SetPixelFormat();
                                break;
                            case 1:
                                //
                                this.ColorMapEntry();
                                break;
                            case 2:
                                // SetEncodings
                                this.SetEncodings();
                                break;
                            case 3:
                                // FramebufferUpdateRequest
                                this.FramebufferUpdateRequest();
                                break;
                            case 4:
                                // KeyEvent
                                this.KeyEvent();
                                break;
                            case 5:
                                // PointerEvent
                                this.PointerEvent();
                                break;
                            case 6:
                                // ClientCutText
                                this.ClientCutText();
                                break;
                            default:
                                // Console.WriteLine($"Received message-type: {type[0]}");
                                logger.Warn($"未接入的客户端消息处理:客户端->服务器:[{type[0]}]");
                                break;
                        }
                    }
                }
                else
                {
                    // Console.WriteLine($"连接断开");
                    // 累积到超时 断开重连
                    if (timeout >0 && t.ElapsedMilliseconds >=timeout)
                    {
                        logger.Warn($"客户端发送的消息循环:客户端->服务器:长时间没有消息[{t.ElapsedMilliseconds}]");
                        t.Stop();
                        throw new SocketException(10053);
                    }
                }

            }
            // t.Stop();
            // Console.WriteLine($"连接断开");
        }

        #region 连接中继

        /// <summary>
        /// 连接中继器
        /// </summary>
        /// <param name="host"></param>
        /// <param name="id">只能用数字</param>
        /// <param name="password"></param>
        /// <param name="timeout"></param>
        public void StartRepeater(string host, string id, string password, int timeout)
        {
            // openGL截图测试
            //Extent.aa();
            //Extent.GrabScreenshot();

            logger.Info($"连接到中继器:[{host}],id:[{id}]");
            this.password = password;

            IPEndPoint remoteEP;
            if (host.Contains(":"))
            {
                string[] parts = host.Split(':');
                IPHostEntry ipHostInfo = Dns.GetHostEntry(parts[0]);
                IPAddress ipAddress = ipHostInfo.AddressList
                    .Where(q => q.AddressFamily== AddressFamily.InterNetwork).First();
                remoteEP = new IPEndPoint(ipAddress, Int32.Parse(parts[1]));
            }
            else
            {
                IPHostEntry ipHostInfo = Dns.GetHostEntry(host);
                IPAddress ipAddress = ipHostInfo.AddressList
                    .Where(q => q.AddressFamily== AddressFamily.InterNetwork).First();
                remoteEP = new IPEndPoint(ipAddress, 5500);
            }


            //this.client.ReceiveTimeout=10000; // 等待接收的超时时间

            int count = 0;
            // 断开连接后不终止程序，继续监听端口的连接
            while (true)
            {
                logger.Info($"连接到中继器:重连次数[{count}]");
                count++;
                try
                {

                    this.client = new TcpClient(AddressFamily.InterNetwork);
                    this.client.Connect(remoteEP);
                    this.client.NoDelay=true;

                    // this.client.Close();

                    this.stream = client.GetStream();

                    this.WriteRepeaterID(id);

                    // 交互版本信息
                    this.ProtocolVersion();

                    // 安全协议
                    this.Security();

                    // 初始化信息
                    this.InitialisationMessages();

                    // 处理客户端消息循环
                    this.ServiceMessageLoop(timeout);
                }
                catch (SocketException e)
                {
                    logger.Error("SocketException:{0}", e);
                    //this.client.Close();
                }
                catch (IOException e)
                {
                    logger.Error("IOException:{0}", e);
                    if (e.InnerException is SocketException)
                    {
                        int code = ((SocketException)e.InnerException).NativeErrorCode;
                        if (code == 10053 || code== 10060)
                        {
                            // 断开连接
                        }
                    }
                    //this.client.Close();
                }
                finally
                {

                }
            }

        }

        private void WriteRepeaterID(string id)
        {
            logger.Info("服务器信息:服务器->中继器");
            byte[] buffer = System.Text.Encoding.ASCII.GetBytes($"ID:{id}".PadRight(250, '\0'));
            this.stream.Write(buffer, 0, buffer.Length);
            this.stream.Flush();

            //byte[] bytes = new byte[256];
            //int length = this.stream.Read(buffer);
            //int debug = 0;
            //debug++;
        }

        #endregion

        #region servcice message

        private void SetPixelFormat()
        {
            logger.Info("SetPixelFormat");
            byte[] buffer = new byte[19];
            int length = this.stream.Read(buffer, 0, buffer.Length);
            byte[] padding = buffer.ReadBuffer(0, 3);
            foreach (byte item in padding)
            {
                logger.Debug($"SetPixelFormat:客户端->服务器:padding:[{item}]");
            }
            byte bitsPerPixel = buffer[3];
            byte depth = buffer[4];
            byte bigEndianFlag = buffer[5];
            byte trueColourFlag = buffer[6];
            ushort redMax = buffer.ReadBuffer(7);
            ushort greenMax = buffer.ReadBuffer(9);
            ushort blueMax = buffer.ReadBuffer(11);
            byte redShift = buffer[13];
            byte greenShift = buffer[14];
            byte blueShift = buffer[15];
            padding = buffer.ReadBuffer(16, 3);
            logger.Debug($"SetPixelFormat:客户端->服务器:pixel-format->bits-per-pixel:[{bitsPerPixel}]");
            logger.Debug($"SetPixelFormat:客户端->服务器:pixel-format->depth:[{depth}]");
            logger.Debug($"SetPixelFormat:客户端->服务器:pixel-format->big-endian-flag:[{bigEndianFlag}]");
            logger.Debug($"SetPixelFormat:客户端->服务器:pixel-format->true-colour-flag:[{trueColourFlag}]");
            logger.Debug($"SetPixelFormat:客户端->服务器:pixel-format->red-max:[{redMax}]");
            logger.Debug($"SetPixelFormat:客户端->服务器:pixel-format->green-max:[{greenMax}]");
            logger.Debug($"SetPixelFormat:客户端->服务器:pixel-format->blue-max:[{blueMax}]");
            logger.Debug($"SetPixelFormat:客户端->服务器:pixel-format->red-shift:[{redShift}]");
            logger.Debug($"SetPixelFormat:客户端->服务器:pixel-format->green-shift:[{greenShift}]");
            logger.Debug($"SetPixelFormat:客户端->服务器:pixel-format->blue-shift:[{blueShift}]");
            foreach (byte item in padding)
            {
                logger.Debug($"SetPixelFormat:客户端->服务器:pixel-format->padding:[{item}]");
            }

            //buffer = new byte[16];
            //length = this.stream.Read(buffer, 0, buffer.Length);
            //logger.Debug($"SetPixelFormat:服务器->客户端:PIXEL_FORMAT");
            //foreach (byte item in buffer)
            //{
            //    logger.Debug($"SetPixelFormat:服务器->客户端:PIXEL_FORMAT:[{item}]");
            //}
        }

        private void SetEncodings()
        {
            logger.Info("SetEncodings");

            byte[] buffer = new byte[3];
            // byte[] buffer = new byte[1];
            int length = this.stream.Read(buffer, 0, buffer.Length);
            byte padding = buffer[0];
            logger.Debug($"SetEncodings:客户端->服务器:padding:[{padding}]");

            ushort numberOfEncodings = BitConverter.ToUInt16(buffer.ReadBuffer(1, 2));
            logger.Debug($"SetEncodings:客户端->服务器:number-of-encodings:[{numberOfEncodings}]");

            buffer = new byte[numberOfEncodings * 4];
            length = this.stream.Read(buffer, 0, buffer.Length);
            for (int i = 0; i< numberOfEncodings; i++)
            {
                int encodingType = BitConverter.ToInt32(buffer.ReadBuffer(i, 4));
                logger.Debug($"SetEncodings:客户端->服务器: encoding-type:[{encodingType}]");
            }
        }

        private void FramebufferUpdateRequest()
        {
            logger.Debug("FramebufferUpdateRequest:客户端->服务器");

            byte[] buffer = new byte[9];
            int i = this.stream.Read(buffer, 0, buffer.Length);

            // 请求从x,y起始 width和height大小的区域的图像

            byte incremental = buffer[0];
            ushort xPosition = BitConverter.ToUInt16(buffer.ReadBuffer(1, 2));
            ushort yPosition = BitConverter.ToUInt16(buffer.ReadBuffer(3, 2));
            ushort width = BitConverter.ToUInt16(buffer.ReadBuffer(5, 2));
            ushort height = BitConverter.ToUInt16(buffer.ReadBuffer(7, 2));

            logger.Debug($"FramebufferUpdateRequest:客户端->服务器:incremental:[{incremental}]");
            logger.Debug($"FramebufferUpdateRequest:客户端->服务器:x-position:[{xPosition}]");
            logger.Debug($"FramebufferUpdateRequest:客户端->服务器:y-position:[{yPosition}]");
            logger.Debug($"FramebufferUpdateRequest:客户端->服务器:width:[{width}]");
            logger.Debug($"FramebufferUpdateRequest:客户端->服务器:height:[{height}]");

            // 恢复截屏请求
            this.FramebufferUpdate(xPosition, yPosition, width, height);
        }

        private void FramebufferUpdate(ushort xPosition, ushort yPosition, ushort width, ushort height)
        {
            logger.Debug("FramebufferUpdateRequest:服务器->客户端");

            // 截屏
            Bitmap bitmap = Extent.ScreenCapture(xPosition, yPosition, width, height);
            byte[] pixel = Extent.GrabPixels(bitmap, xPosition, yPosition, width, height, bitmap.PixelFormat);

            byte[] buffer = new byte[4];
            buffer[0] = 0; // message-type
            buffer[1] = 0; //padding
            ushort numberOfRectangles = 1;
            numberOfRectangles.CopyToArray(buffer, 2);

            // 
            byte[] bufferRectangle = new byte[12];
            int encodingType = 0;
            xPosition.CopyToArray(bufferRectangle, 0);
            yPosition.CopyToArray(bufferRectangle, 2);
            width.CopyToArray(bufferRectangle, 4);
            height.CopyToArray(bufferRectangle, 6);
            encodingType.CopyToArray(bufferRectangle, 8);

            this.stream.Write(buffer);
            for (int i = 0; i< numberOfRectangles; i++)
            {
                this.stream.Write(bufferRectangle);
                this.stream.Write(pixel);
            }
        }

        private void KeyEvent()
        {
            logger.Debug("KeyEvent:客户端->服务器");

            byte[] buffer = new byte[7];
            int length = this.stream.Read(buffer, 0, buffer.Length);
            byte downFlag = buffer[0];
            ushort padding = BitConverter.ToUInt16(buffer.ReadBuffer(1, 2));
            uint key = BitConverter.ToUInt16(buffer.ReadBuffer(3, 4));

            logger.Debug($"KeyEvent:客户端->服务器:down-flag:[{downFlag}]");
            logger.Debug($"KeyEvent:客户端->服务器:padding:[{padding}]");
            logger.Debug($"KeyEvent:客户端->服务器:key:[{key}]");

            this.avatar.KeyEvent(downFlag, key);
        }

        private void PointerEvent()
        {
            logger.Debug("PointerEvent:客户端->服务器");

            byte[] buffer = new byte[5];
            int length = this.stream.Read(buffer, 0, buffer.Length);

            byte buttonMask = buffer[0]; // 1-8
            ushort xPosition = BitConverter.ToUInt16(buffer.ReadBuffer(1, 2));
            ushort yPosition = BitConverter.ToUInt16(buffer.ReadBuffer(3, 2));

            logger.Debug($"PointerEvent:客户端->服务器:button-mask:[{buttonMask}]");
            logger.Debug($"PointerEvent:客户端->服务器:x-position:[{xPosition}]");
            logger.Debug($"PointerEvent:客户端->服务器:y-position:[{yPosition}]");

            this.avatar.PointerEvent(buttonMask, xPosition, yPosition);
        }

        private void ClientCutText()
        {
            logger.Info("PointerEvent:客户端->服务器");

            byte[] buffer = new byte[3];
            int i = this.stream.Read(buffer, 0, buffer.Length);
            logger.Debug("PointerEvent:客户端->服务器:padding");
            // Console.WriteLine($"ClientCutText padding");
            foreach (byte item in buffer)
            {
                logger.Debug("PointerEvent:客户端->服务器:padding:[{item}]");
            }

            buffer = new byte[4];
            i = this.stream.Read(buffer, 0, buffer.Length);
            Array.Reverse(buffer);
            uint length = BitConverter.ToUInt32(buffer);
            Console.WriteLine($"PointerEvent length:{length}");

            buffer = new byte[length];
            i = this.stream.Read(buffer, 0, buffer.Length);
            Array.Reverse(buffer);
            string text = System.Text.Encoding.UTF8.GetString(buffer);
            Console.WriteLine($"PointerEvent text:{text}");
        }

        private void ColorMapEntry()
        {
            logger.Info("ColorMapEntry:客户端->服务器");

            byte[] buffer = new byte[1];
            int i = this.stream.Read(buffer, 0, buffer.Length);
            byte padding = buffer[0];
            Console.WriteLine($"ColorMapEntry padding");
            foreach (byte item in buffer)
            {
                Console.WriteLine($"ColorMapEntry padding {padding}");
            }

            buffer = new byte[2];
            i = this.stream.Read(buffer, 0, buffer.Length);
            Array.Reverse(buffer);
            ushort firstColor = BitConverter.ToUInt16(buffer);
            Console.WriteLine($"SetEncodings firstColor:{firstColor}");

            buffer = new byte[2];
            i = this.stream.Read(buffer, 0, buffer.Length);
            Array.Reverse(buffer);
            ushort nbColors = BitConverter.ToUInt16(buffer);
            Console.WriteLine($"SetEncodings nbColors:{nbColors}");

            buffer = new byte[firstColor * nbColors];
            i = this.stream.Read(buffer, 0, buffer.Length);

            this.stream.ReadU16();

        }

        #endregion
    }

}
// https://github.com/rfbproto/rfbproto/blob/master/rfbproto.rst#setpixelformat
// https://github.com/ajnewlands/X11.Net