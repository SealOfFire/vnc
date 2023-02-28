# CMakeList.txt: VNC_CPP 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#

##python环境
##set(${boost_python_DIR} D:/ProgramData/Anaconda3)
#find_package(PythonLibs 3.9 REQUIRED)
#message(Python\ version: ${PYTHONLIBS_VERSION_STRING})
#message(Python\ includes: ${PYTHON_INCLUDE_DIRS})
#message(Python\ libs: ${PYTHON_LIBRARIES})
#include_directories(${PYTHON_INCLUDE_DIRS})
##link_directories(${PYTHON_LIBRARIES})
##target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})

find_package(Boost COMPONENTS regex log_setup log)
#find_package(OpenSSL REQUIRED)


#linux编译时修改未linux的cryptopp源码保存路径
set(CRYPTOPP_SOURCES D:/MyProgram/cryptopp870)
include_directories(${CRYPTOPP_SOURCES})
#link_directories(D:/MyProgram/cryptopp870/x64/DLL_Output/Release)
#add_executable(CryptoPP ../test/test.cpp ../include/fillingCloseCurve.h)

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
#add_executable (VNC_CPP "VNC_CPP.cpp" "VNC_CPP.h" "VNCService.h" "PixelFormat.h")
#add_executable (VNC_CPP "VNC_CPP.cpp" "PixelFormat.h" "logger.h" "screen_win.cpp")
if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
	add_executable (VNC_CPP VNC_CPP.cpp logger.h logger.cpp Common.h PixelFormat.h VNCService.h VNCService.cpp template.hpp screen_linux.h screen_linux.cpp)
	#add_library (VNC_CPP SHARED logger.h logger.cpp Common.h PixelFormat.h VNCService.h VNCService.cpp template.hpp screen_linux.h screen_linux.cpp python.cpp)
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
	add_executable (VNC_CPP VNC_CPP.cpp logger.h logger.cpp Common.h PixelFormat.h VNCService.h VNCService.cpp template.hpp screen_win.h screen_win.cpp)
	#add_library (VNC_CPP SHARED logger.h logger.cpp Common.h PixelFormat.h VNCService.h VNCService.cpp template.hpp screen_win.h screen_win.cpp python.cpp)
endif()


if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VNC_CPP PROPERTY CXX_STANDARD 20)
endif()

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

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

target_link_libraries(${PROJECT_NAME} cryptopp)

#
#target_link_libraries(${PROJECT_NAME} ${PYTHON_LIBRARIES})
