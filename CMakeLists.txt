cmake_minimum_required(VERSION 2.8)
project(UCSD-GP-Team5)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)
find_package(Boost 1.61 COMPONENTS filesystem REQUIRED)

include_directories(include src ${Boost_INCLUDE_DIR})
file(GLOB SOURCES "src/*.cpp")

add_executable(CaptureImage captureImage.cpp ${SOURCES})
target_link_libraries(CaptureImage ${OpenCV_LIBS} Boost::filesystem)

add_executable(CameraCalibration cameraCalibration.cpp ${SOURCES})
target_link_libraries(CameraCalibration ${OpenCV_LIBS} Boost::filesystem)

add_executable(Preview preview.cpp ${SOURCES})
target_link_libraries(Preview ${OpenCV_LIBS} Boost::filesystem)
