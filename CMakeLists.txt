cmake_minimum_required(VERSION 3.10)
project(cv_test)

set(CMAKE_CXX_STANDARD 17)

set(OpenCV_DIR "D:/opencv_mingw/OpenCV-MinGW-Build-OpenCV-4.5.5-x64")
find_package(OpenCV REQUIRED)

add_executable(cv_test main.cpp)
target_include_directories(cv_test PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(cv_test PRIVATE ${OpenCV_LIBS})