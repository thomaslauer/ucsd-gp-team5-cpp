#include <stdio.h>
#include <string>
#include <iostream>
#include "opencv2/opencv.hpp"
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

#include "constants.hpp"

using namespace std;
using namespace cv;
using namespace boost::filesystem;

int main(int argc, char** argv) {
    if(argc < 3) {
        cout << "Incorrect usage:" << endl;
        cout << argv[0] << " [source dir] [output.xml]" << endl;
        return 1;
    }
    
    string path = argv[1];
    string outputFile = argv[2];
    
    Mat coverage(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3, Scalar(0,0,0));
    for(auto& entry : boost::make_iterator_range(directory_iterator(path), {})) {
        cout << "loading file " << entry << endl;
        Mat img = imread(entry.path().string());
        Size imageSize(FRAME_WIDTH, FRAME_HEIGHT);
        resize(img, img, imageSize);
        Mat gray;
        cvtColor(img, gray, CV_BGR2GRAY);
        
        Size patternSize(CHESSBOARD_WIDTH, CHESSBOARD_HEIGHT);
        vector<Point2f> corners;

        bool patternFound = findChessboardCorners(gray, patternSize, corners, 
                CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
                + CALIB_CB_FAST_CHECK);

        if(patternFound) {
            cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),
                    TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

            drawChessboardCorners(img, patternSize, Mat(corners), patternFound);
            drawChessboardCorners(coverage, patternSize, Mat(corners), patternFound);
        }

        if(USE_GUI) {
            imshow("img", img);
            if(waitKey(1000) == KEY_ESC) break;
        }
    }
    imwrite("coverage.png", coverage);
}
