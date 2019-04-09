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
    
    // generate size for the chessboard
    Size patternSize(CHESSBOARD_WIDTH, CHESSBOARD_HEIGHT);

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

    cout << "camera matrix:" << endl;
    cout << cameraMatrix << endl;
    cout << "distortion coeffs:" << endl;
    cout << distCoeffs << endl;

    imwrite("coverage.png", coverage);

    // start calculating homography
    Mat hSource = imread(homographyImg);
    resize(hSource, hSource, imageSize);
    cvtColor(hSource, hSource, CV_BGR2GRAY);

    Mat hTarget = targetChessboard();
    cvtColor(hTarget, hTarget, CV_BGR2GRAY);

    vector<Vec2f> sourcePts;
    vector<Vec2f> targetPts;

    findChessboardCorners(hSource, patternSize, sourcePts, 
            CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE);
    findChessboardCorners(hTarget, patternSize, targetPts, 
            CALIB_CB_ADAPTIVE_THRESH + CALIB_CB_NORMALIZE_IMAGE);

    cornerSubPix(hSource, sourcePts, Size(11, 11), Size(-1, -1), 
            TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
    cornerSubPix(hTarget, targetPts, Size(11, 11), Size(-1, -1), 
            TermCriteria(CV_TERMCRIT_EPS + CV_TERMCRIT_ITER, 30, 0.1));
    
    H = findHomography(sourcePts, targetPts);

    cout << "H:" << endl;
    cout << H << endl;

}

void Calibration::load(string filename) {
    FileStorage fs(filename, FileStorage::READ);

    fs["cameraMatrix"] >> cameraMatrix;
    fs["distCoeffs"] >> distCoeffs;
    fs["H"] >> H;

    cout << "camera matrix is" << endl;
    cout << cameraMatrix << endl;
    cout << "distCoeffs is" << endl;
    cout << distCoeffs << endl;
    cout << "H is" << endl;
    cout << H << endl;

    fs.release();
}

void Calibration::save(string filename) {
    FileStorage fs(filename, FileStorage::WRITE);

    fs << "cameraMatrix" << cameraMatrix;
    fs << "distCoeffs" << distCoeffs;
    fs << "H" << H;

    fs.release();
}

void Calibration::undistort(Mat& image) {
    if(!initMaps) {
        cout << "building undistortion maps" << endl;
        initMaps = true;
        Size imageSize(FRAME_WIDTH, FRAME_HEIGHT);
        initUndistortRectifyMap(cameraMatrix, distCoeffs, Mat::eye(3,3,CV_32F), 
                cameraMatrix, imageSize, CV_32FC1, map1, map2);
    }

    remap(image, image, map1, map2, INTER_NEAREST);
}
void Calibration::hTransform(Mat& image) {
    warpPerspective(image, image, H, image.size(), INTER_NEAREST);
}

vector<Vec3f> Calibration::chessboardWorldPoints() {
    vector<Vec3f> pts;
    for(int i = 0; i < CHESSBOARD_HEIGHT; i++) {
        for(int j = 0; j < CHESSBOARD_WIDTH; j++) {
            pts.push_back(Vec<float, 3>(i, j));
        }
    }
    return pts;

}

Mat Calibration::targetChessboard() {
    Mat img(FRAME_HEIGHT, FRAME_WIDTH, CV_8UC3, Scalar(128,128,128));
    
    int startX = (FRAME_WIDTH - (CHESSBOARD_WIDTH + 1) * TARGET_CELL_SIZE)/2;
    int startY = FRAME_HEIGHT - ((CHESSBOARD_HEIGHT + 1) * TARGET_CELL_SIZE)
        - TARGET_BOTTOM_OFFSET;

    int currentX = startX;
    int currentY = startY;

    int rowToggle = 1;

    for(int i = 0; i < CHESSBOARD_HEIGHT + 1; i++) {
        int colorToggle = rowToggle;
        for(int j = 0; j < CHESSBOARD_WIDTH + 1; j++) {
            Point p1(currentX, currentY);
            Point p2(currentX + TARGET_CELL_SIZE, currentY + TARGET_CELL_SIZE);
            Scalar color(255 * colorToggle, 255 * colorToggle, 255 * colorToggle);

            rectangle(img, p1, p2, color, CV_FILLED);

            colorToggle = (colorToggle) ? 0 : 1;
            currentX += TARGET_CELL_SIZE;
        }
        currentX = startX;
        currentY += TARGET_CELL_SIZE;
        rowToggle = (rowToggle) ? 0 : 1;
    }

    return img;
}
