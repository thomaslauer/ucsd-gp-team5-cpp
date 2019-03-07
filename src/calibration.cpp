#include "calibration.hpp" 

#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include "opencv2/opencv.hpp"

#include "constants.hpp"

using namespace std;
using namespace cv;
using namespace boost::filesystem;

Calibration::Calibration() {}
Calibration::~Calibration() {}

void Calibration::generateCalibration(string calibFolder, string homographyImg) {

    vector< vector<Vec2f> > imagePoints;
    vector< vector<Vec3f> > worldPoints;
    
    Size imageSize(FRAME_WIDTH, FRAME_HEIGHT);
    Mat coverage(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3, Scalar(0,0,0));

    for(auto& entry : boost::make_iterator_range(directory_iterator(calibFolder))) {

        // load image from file
        cout << "loading file " << entry << endl;
        Mat img = imread(entry.path().string());
        if(img.data == nullptr) continue;

        // resize image to needed dimensions
        resize(img, img, imageSize);

        // make grayscale version
        Mat gray;
        cvtColor(img, gray, CV_BGR2GRAY);
        
        // generate size for the chessboard
        Size patternSize(CHESSBOARD_WIDTH, CHESSBOARD_HEIGHT);
        
        // vector to store the corners
        vector<Vec2f> corners;

        bool patternFound = findChessboardCorners(gray, patternSize, corners, 
                CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE
                + CALIB_CB_FAST_CHECK);

        if(patternFound) {
            cornerSubPix(gray, corners, Size(11, 11), Size(-1, -1),
                    TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));

            imagePoints.push_back(corners);
            worldPoints.push_back(chessboardWorldPoints());

            // draw the chessboard corners on the images for display
            drawChessboardCorners(img, patternSize, corners, patternFound);
            drawChessboardCorners(coverage, patternSize, corners, patternFound);
        }

        if(USE_GUI) {
            imshow("img", img);
            if(waitKey(0) == KEY_ESC) break;
        }
    }
    
    vector<Mat> rvecs, tvecs;

    // generate calibration from points
    calibrateCamera(worldPoints, imagePoints, imageSize, cameraMatrix, distCoeffs, rvecs, tvecs);

    cout << cameraMatrix << endl;
    cout << distCoeffs << endl;

    imwrite("coverage.png", coverage);
}

void Calibration::load(string filename) {}
void Calibration::save(string filename) {}

void Calibration::undistort(Mat& image) {}
void Calibration::hTransform(Mat& image) {}

vector<Vec3f> Calibration::chessboardWorldPoints() {
    vector<Vec3f> pts;

    for(int i = 0; i < CHESSBOARD_HEIGHT; i++) {
        for(int j = 0; j < CHESSBOARD_WIDTH; j++) {
            pts.push_back(Vec<float, 3>(i, j));

        }
    }

    return pts;

}
