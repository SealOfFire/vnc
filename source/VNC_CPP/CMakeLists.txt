# CMakeList.txt: 顶层 CMake 项目文件，在此处执行全局配置
# 并包含子项目。
#
cmake_minimum_required (VERSION 3.8)

project ("VNC_CPP")

#option(Debug "debug" ON)

#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -DMY_DEBUG")
#SET(BOOST_ROOT "D:/Program Files/boost")
#SET(BOOST_LIBRARY_DIRS "D:/Program Files/boost/lib")

#find_package(Boost 1.74.0 REQUIRED COMPONENTS regex)
#find_package(Boost 1.81.0 REQUIRED COMPONENTS system thread)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

#message(Boost\ version: ${Boost_VERSION_STRING})
#message(Boost\ include\ dir: ${Boost_INCLUDE_DIRS})
#message(Boost\ library\ dir: ${Boost_LIBRARY_DIRS})

#message("CMAKE_HOST_SYSTEM_NAME:" ${CMAKE_HOST_SYSTEM_NAME})
#message("CMAKE_HOST_SYSTEM:" ${CMAKE_HOST_SYSTEM})
#message("CMAKE_SYSTEM_NAME:" ${CMAKE_SYSTEM_NAME})
#message("CMAKE_SYSTEM:" ${CMAKE_SYSTEM})
#message("CMAKE_HOST_UNIX:" ${CMAKE_HOST_UNIX})
#message("CMAKE_HOST_WIN32:" ${CMAKE_HOST_WIN32})
if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
	message("当前平台是 linux")
	#option(LINUX "平台" ON)
	add_definitions(-DLINUX)
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows")
	message("当前平台是 win")
	#option(WIN "平台" ON)
	add_definitions(-DWIN)
endif()


# 调用win api
add_definitions(-DUNICODE -D_UNICODE)

# 包含子项目。
add_subdirectory ("VNC_CPP")
#add_subdirectory ("cpp-netlib-0.13.0-final")

#set (CMAKE_PREFIX_PATH ${CMAKE_PREFIX_PATH} D:/MyProgram/VNC_CPP/cpp-netlib-0.13.0-final)

#find_package (cppnetlib 0.13.0 REQUIRED)
#include_directories (${CPPNETLIB_INCLUDE_DIRS} )
#target_link_libraries ("VNC_CPP" ${CPPNETLIB_LIBRARIES})

#FIND_PACKAGE(Boost)

