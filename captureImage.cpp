#include <stdio.h>
#include "opencv2/opencv.hpp"

#include "constants.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    if(argc < 2) {
        cout << "Error: need path to save file" << endl;
        return 1;
    }

    VideoCapture stream(0);

    
    stream.set(CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
    stream.set(CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
    stream.set(CAP_PROP_FPS, FRAME_RATE);
    stream.set(CV_CAP_PROP_BUFFERSIZE, BUFFER_SIZE);

    Mat cameraFrame;
    for(int i = 0; i < FRAME_RATE * 4; i++) {
        stream.read(cameraFrame);
        imshow("frame", cameraFrame);
        waitKey(1);
    }

    imwrite(argv[1], cameraFrame);

    return 0;
}
