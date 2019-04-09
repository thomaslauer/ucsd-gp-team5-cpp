#ifndef FILTER_HPP
#define FILTER_HPP

#include "opencv2/opencv.hpp"

using namespace cv;

class Filter {
public:
    Filter();
    ~Filter();

    int median_width = 7;

    int findCenter(Mat image, int row, int startX, int endX);
};

#endif
