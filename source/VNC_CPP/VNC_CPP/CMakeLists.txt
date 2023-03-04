# CMakeList.txt: VNC_CPP 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

#python环境
set(PYTHON_ROOT D:/ProgramData/Anaconda3/envs/DeepAIEnviroment32)
set(PYTHON_INCLUDE_DIR ${PYTHON_ROOT}/include)
set(PYTHON_LIBRARY ${PYTHON_ROOT}/libs/python38.lib)
find_package(PythonLibs 3.8 REQUIRED)
message(Python\ version: ${PYTHONLIBS_VERSION_STRING})
message(Python\ includes: ${PYTHON_INCLUDE_DIRS})
message(Python\ libs: ${PYTHON_LIBRARIES})
include_directories(${PYTHON_INCLUDE_DIRS})
# 打包python的时候使用静态连接
if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
	# win上打包时候使用动态链接库
	set(Boost_USE_STATIC_LIBS ON)
	# cryptopp源码保存路径
	set(CRYPTOPP_SOURCES D:/MyProgram/cryptopp870)
	add_definitions(-DWIN)
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
	# cryptopp源码保存路径
	set(CRYPTOPP_SOURCES /home/xus/cryptopp870)
	add_definitions(-DLINUX)
endif()

set(BOOST_ROOT /home/xus/boost) # 设置boost库的路劲
find_package(Boost 1.81.0 COMPONENTS regex log_setup log iostreams thread python38)
message(Boost\ version: ${Boost_VERSION_STRING})
message(Boost\ include\ dir: ${Boost_INCLUDE_DIRS})
message(Boost\ library\ dir: ${Boost_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIRS})

#include_directories(${PYTHON_INCLUDE_DIRS})

#find_package(OpenSSL REQUIRED)

include_directories(${CRYPTOPP_SOURCES})

#find_package(PythonLibs 3.10 REQUIRED)
#include_directories(${PYTHON_INCLUDE_DIRS})
#link_directories(${PYTHON_LIBRARIES})

#link_directories(D:/MyProgram/cryptopp870/Win32/DLL_Output/Release)

#find_package(CryptoPP REQUIRED D:/MyProgram/cryptopp870)
#target_link_libraries(${PROJECT_NAME} cryptopp-static)


if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
	find_package(X11 REQUIRED)
	link_libraries(${X11_LIBRARIES})
	include_directories(${X11_INCLUDE_DIR})
	message(X11_libaries ${X11_LIBRARIES})
	message(X11_include ${X11_INCLUDE_DIR})
	link_libraries(Xtst)
endif()


# 将源代码添加到此项目的可执行文件。
list(APPEND source VNC_CPP.cpp)
list(APPEND source logger.h)
list(APPEND source logger.cpp)
list(APPEND source Common.h)
list(APPEND source PixelFormat.h)
list(APPEND source VNCService.h)
list(APPEND source VNCService.cpp)
list(APPEND source template.hpp)
#list(APPEND source python.cpp)
if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
	list(APPEND source screen_linux.h)
	list(APPEND source screen_linux.cpp)
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
	list(APPEND source screen_win.h)
	list(APPEND source screen_win.cpp)
endif()

# 打包成exe
add_executable(VNC_CPP ${source})
# 打包成python模块
#add_library(VNC_CPP MODULE ${source})

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VNC_CPP PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(VNC_CPP ${Boost_LIBRARIES})

#add_library(CryptoPP D:/MyProgram/cryptopp870/x64/DLL_Output/Release/cryptopp.dll)
#set_target_properties(CryptoPP PROPERTIES LINKER_LANGUAGE C)
#target_link_libraries(${PROJECT_NAME} CryptoPP)

# TODO: 如有需要，请添加测试并安装目标。
#add_subdirectory(${CMAKE_SOURCE_DIR}/DirectXTK ${CMAKE_BINARY_DIR}/bin/CMake/DirectXTK)
#target_link_libraries(${PROJECT_NAME} PRIVATE DirectXTK)

#target_link_libraries(${PROJECT_NAME} PRIVATE d3d11)


#option(CRYPTOPP_SOURCES "" D:/MyProgram/cryptopp870)
add_subdirectory(cryptopp-cmake)
target_link_libraries(VNC_CPP cryptopp)

#python 打包配置，打包成exe的时候需要注释掉
if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
	# linux上打包去掉开头的lib
	set_target_properties(VNC_CPP PROPERTIES PREFIX "")
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
	# 设置win上打包的后缀名
	# 打包成exe的时候需要注释掉
	#set_target_properties(VNC_CPP PROPERTIES SUFFIX ".pyd")
endif()
target_link_libraries(VNC_CPP ${PYTHON_LIBRARIES})


# zlib引用
set(zilib_dll ${PROJECT_SOURCE_DIR}/VNC_CPP/zlib/libs/win_x64/zlibd.dll)
set(zilib_lib ${PROJECT_SOURCE_DIR}/VNC_CPP/zlib/libs/win_x64/zlibd.lib)
message(${zilib_dll})
message(${zilib_lib})
include_directories(${PROJECT_SOURCE_DIR}/zlib/include)
target_link_libraries(${PROJECT_NAME} ${zilib_lib})
# dll文件复制到指定目录
add_custom_command(TARGET VNC_CPP POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different
	${zilib_dll} $<TARGET_FILE_DIR:${PROJECT_NAME}>)
