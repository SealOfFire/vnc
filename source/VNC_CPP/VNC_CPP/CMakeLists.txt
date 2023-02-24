# CMakeList.txt: VNC_CPP 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
#set(CryptoPP D:/MyProgram/cryptopp870/x64/DLL_Output/Release)

find_package(Boost 1.74.0 REQUIRED COMPONENTS regex log_setup log)
#find_package(OpenSSL REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})



include_directories(D:/MyProgram/cryptopp870)
#link_directories(D:/MyProgram/cryptopp870/x64/DLL_Output/Release)
#add_executable(CryptoPP ../test/test.cpp ../include/fillingCloseCurve.h)


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
add_executable (VNC_CPP "VNC_CPP.cpp")

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


set(CRYPTOPP_SOURCES D:/MyProgram/cryptopp870)
#option(CRYPTOPP_SOURCES "" D:/MyProgram/cryptopp870)
add_subdirectory(cryptopp-cmake)

target_link_libraries(${PROJECT_NAME} cryptopp)