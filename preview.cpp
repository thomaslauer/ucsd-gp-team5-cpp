#include <stdio.h>
#include "opencv2/opencv.hpp"
#include <time.h>

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

    clock_t t;
    Mat cameraFrame;
    while(true) {
        t = clock();
        stream.read(cameraFrame);
        cb.undistort(cameraFrame);
        cb.hTransform(cameraFrame);
        imshow("frame", cameraFrame);
        if(waitKey(1) == KEY_ESC) break;

        t = clock() - t;
        double time_taken = ((double)t) / CLOCKS_PER_SEC;
        cout << 1/time_taken << " fps" << endl;

    }

    return 0;
}
