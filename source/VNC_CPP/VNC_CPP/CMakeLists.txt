# CMakeList.txt: VNC_CPP 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

#python环境
find_package(PythonLibs 3.9 REQUIRED)
message(Python\ version: ${PYTHONLIBS_VERSION_STRING})
message(Python\ includes: ${PYTHON_INCLUDE_DIRS})
message(Python\ libs: ${PYTHON_LIBRARIES})
include_directories(${PYTHON_INCLUDE_DIRS})
#add_definitions(-DBOOST_PYTHON_STATIC_LIB)
# 打包python的时候使用静态连接
if(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
	# win上打包时候使用动态链接库
	set(Boost_USE_STATIC_LIBS ON)
	# cryptopp源码保存路径
	set(CRYPTOPP_SOURCES D:/MyProgram/cryptopp870)
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
	# cryptopp源码保存路径
	set(CRYPTOPP_SOURCES /home/xus/cryptopp870)
endif()

find_package(Boost COMPONENTS regex log_setup log python39)
#find_package(OpenSSL REQUIRED)

include_directories(${CRYPTOPP_SOURCES})

#find_package(PythonLibs 3.10 REQUIRED)
#include_directories(${PYTHON_INCLUDE_DIRS})
#link_directories(${PYTHON_LIBRARIES})

#link_directories(D:/MyProgram/cryptopp870/Win32/DLL_Output/Release)

#find_package(CryptoPP REQUIRED D:/MyProgram/cryptopp870)
#target_link_libraries(${PROJECT_NAME} cryptopp-static)


message(Boost\ version: ${Boost_VERSION_STRING})
message(Boost\ include\ dir: ${Boost_INCLUDE_DIRS})
message(Boost\ library\ dir: ${Boost_LIBRARY_DIRS})


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
list(APPEND source python.cpp)
if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
	list(APPEND source screen_linux.h)
	list(APPEND source screen_linux.cpp)
	add_definitions(-DLINUX)
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
	list(APPEND source screen_win.h)
	list(APPEND source screen_win.cpp)
	add_definitions(-DWIN)
endif()

# 打包成exe
#add_executable (VNC_CPP ${source})
# 打包成python模块
add_library(VNC_CPP MODULE ${source})

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

#target_link_libraries(${PROJECT_NAME} ${Boost_LOG_SETUP_LIBRARY})
#target_link_libraries(${PROJECT_NAME} ${Boost_LOG_LIBRARY})
#add_definitions(-DBOOST_LOG_DYN_LINK)


#option(CRYPTOPP_SOURCES "" D:/MyProgram/cryptopp870)
add_subdirectory(cryptopp-cmake)
target_link_libraries(VNC_CPP cryptopp)

#python 打包配置
if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
	# linux上打包去掉开头的lib
	set_target_properties(VNC_CPP PROPERTIES PREFIX "")
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
	# 设置win上打包的后缀名
	set_target_properties(VNC_CPP PROPERTIES SUFFIX ".pyd")
endif()
target_link_libraries(VNC_CPP ${PYTHON_LIBRARIES})
#target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES}/python39.lib)
