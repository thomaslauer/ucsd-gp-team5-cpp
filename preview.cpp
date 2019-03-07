#include <stdio.h>
#include "opencv2/opencv.hpp"

#include "constants.hpp"
#include "calibration.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {

    VideoCapture stream(0);
    
    stream.set(CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    stream.set(CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
    stream.set(CAP_PROP_FPS, FRAME_RATE);
    stream.set(CV_CAP_PROP_BUFFERSIZE, BUFFER_SIZE);

    Calibration cb;
    cb.load(argv[1]);

    Mat cameraFrame;
    while(true) {
        stream.read(cameraFrame);
        cb.undistort(cameraFrame);
        imshow("frame", cameraFrame);
        if(waitKey(1) == KEY_ESC) break;
    }

    return 0;
}
