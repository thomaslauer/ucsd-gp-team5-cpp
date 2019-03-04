cmake_minimum_required(VERSION 2.8)
project( UCSD-GP-Team5 )
find_package( OpenCV REQUIRED )

add_executable( CaptureImage CaptureImage.cpp )
target_link_libraries( CaptureImage ${OpenCV_LIBS} )

#add_executable( DisplayImage DisplayImage.cpp )
#target_link_libraries( DisplayImage ${OpenCV_LIBS} )
