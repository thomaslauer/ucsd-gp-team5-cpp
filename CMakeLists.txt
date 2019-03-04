cmake_minimum_required(VERSION 2.8)
project( UCSD-GP-Team5 )

find_package( OpenCV REQUIRED )
find_package( Boost 1.61 COMPONENTS filesystem REQUIRED )

include_directories( include ${Boost_INCLUDE_DIR} )

add_executable( CaptureImage captureImage.cpp )
target_link_libraries( CaptureImage ${OpenCV_LIBS} )

add_executable( CameraCalibration cameraCalibration.cpp )
target_link_libraries( CameraCalibration ${OpenCV_LIBS} Boost::filesystem )
