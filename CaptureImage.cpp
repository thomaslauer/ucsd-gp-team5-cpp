#include <stdio.h>

#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    if(argc < 2) {
        cout << "Error: need path to save file" << endl;
        return 1;
    }

    VideoCapture stream(0);
    stream.set(CV_CAP_PROP_BUFFERSIZE, 3);

    Mat cameraFrame;
    for(int i = 0; i < 30; i++) {
        stream.read(cameraFrame);
        imshow("frame", cameraFrame);
        waitKey(1);
    }

    imwrite(argv[1], cameraFrame);

    return 0;
}
