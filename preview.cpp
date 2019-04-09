#include <stdio.h>
#include "opencv2/opencv.hpp"
#include <time.h>

#include "constants.hpp"
#include "calibration.hpp"
#include "filter.hpp"

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

    Filter f;

    clock_t t;
    Mat cameraFrame;
    Mat gray;
    while(true) {
        t = clock();
        stream.read(cameraFrame);
        cvtColor(cameraFrame, gray, CV_BGR2GRAY);
        //cb.undistort(cameraFrame);
        cb.hTransform(cameraFrame);
        int centerPixel = (int)f.findCenter(gray, FRAME_HEIGHT / 2, 0, FRAME_WIDTH);
        cout << "center is " << centerPixel << endl;
        if(USE_GUI) {
            imshow("frame", cameraFrame);
        }
        if(waitKey(1) == KEY_ESC) break;

        t = clock() - t;
        double time_taken = ((double)t) / CLOCKS_PER_SEC;
        cout << "took " << time_taken << " " << 1/time_taken << " fps" << endl;
    }

    return 0;
}
