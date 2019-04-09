cmake_minimum_required(VERSION 2.8)
project(UCSD-GP-Team5)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenCV REQUIRED)
find_package(Boost 1.61 COMPONENTS filesystem REQUIRED)

find_package(OpenMP)

if(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif()

include_directories(include src ${Boost_INCLUDE_DIR})
file(GLOB SOURCES "src/*.cpp")

add_executable(CaptureImage captureImage.cpp ${SOURCES})
target_link_libraries(CaptureImage ${OpenCV_LIBS} Boost::filesystem wiringPi)

add_executable(CameraCalibration cameraCalibration.cpp ${SOURCES})
target_link_libraries(CameraCalibration ${OpenCV_LIBS} Boost::filesystem wiringPi)

add_executable(Preview preview.cpp ${SOURCES})
target_link_libraries(Preview ${OpenCV_LIBS} Boost::filesystem wiringPi)

add_executable(WiringPiTest wiringPiTest.cpp)
target_link_libraries(WiringPiTest wiringPi)
