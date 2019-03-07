#ifndef CALIBRATION_CPP
#define CALIBRATION_CPP

#include <string>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include "opencv2/opencv.hpp"

using namespace std;
using namespace cv;

class Calibration {
private:
    Mat cameraMatrix;
    Mat distCoeffs;
    Mat H;

public:
    Calibration();
    ~Calibration();
    
    void generateCalibration(string calibFolder, string homographyImg);

    void load(string filename);
    void save(string filename);

    void undistort(Mat& img);
    void hTransform(Mat& img);

    vector<Vec3f> chessboardWorldPoints();
};

#endif
