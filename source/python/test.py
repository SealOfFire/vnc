import VNC_CPP
print("start")

service = VNC_CPP.VNCService()
# 本地启动监听
# 参数1 接听端口
# 参数2 连接到本机的密码
#service.start(5900, "123456")

# 注册到中继器
# 参数1 注册到中继器的id
# 参数2 中继器的地址
# 参数3 中继器的端口
# 参数4 连接到本机的密码
service.startRepeater("456", "DESKTOP-WORK", 5500, "123456")

print("end")