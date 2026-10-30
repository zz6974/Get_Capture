cmake_minimum_required(VERSION 3.10)
project(cv_test)

set(CMAKE_CXX_STANDARD 17)
add_subdirectory(cc_-sdk/CC_SDK)

set(OpenCV_DIR "D:/opencv_mingw/OpenCV-MinGW-Build-OpenCV-4.5.5-x64")
find_package(OpenCV REQUIRED)

add_executable(cv_test main.cpp
        Comm_Protocol.cpp
        Comm_Protocol.h
        Image_Sender.cpp
        Image_Sender.h
        Ser_Camera.cpp
        Ser_Camera.h
        Ser_Motor.cpp
        Ser_Motor.h
        Ser_WebSock.cpp
        Ser_WebSock.h)


set(MCONTROLLER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Mcontroller-v7-FanciSwarm")
target_include_directories(cv_test PRIVATE
        ${MCONTROLLER_DIR}/Clibrary/include
        ${MCONTROLLER_DIR}/Cpplibrary/include)
target_link_libraries(cv_test PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libMcontroller-v7-FanciSwarm.a")

target_include_directories(cv_test PUBLIC ${CC_API_INC} ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(cv_test PUBLIC CC_API)

target_include_directories(cv_test PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(cv_test PRIVATE ${OpenCV_LIBS})
