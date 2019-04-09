#include "filter.hpp"

#include <vector>
#include <algorithm>
#include "opencv2/opencv.hpp"

#include "constants.hpp"

using namespace std;
using namespace cv;

Filter::Filter() {}
Filter::~Filter() {}

int Filter::findCenter(Mat image, int row, int startX, int endX) {
    // make sure this is the right datatype, maybe need to use float?
    vector<uchar> array;

    int bufferSize = (median_width - 1) / 2;

    for(int i = 0; i < bufferSize; i++) {
        array.push_back(image.at<uchar>(row, startX));
    }
    for(int i = startX; i < startX + endX; i++) {
        array.push_back(image.at<uchar>(row, i));
    }
    for(int i = 0; i < bufferSize; i++) {
        array.push_back(image.at<uchar>(row, endX - 1));
    }

    array.insert(array.begin(), bufferSize, array.front());
    array.insert(array.end(), bufferSize, array.back());

    vector<uchar> meanFiltered;

    for(int i = 0; i < endX - startX; i++) {
        vector<uchar> currentWindow(array.begin() + i, array.begin() + i + median_width);
        size_t n = currentWindow.size() / 2;
        nth_element(currentWindow.begin(), currentWindow.begin()+n, currentWindow.end());

        meanFiltered.push_back(currentWindow[n]);
    }

    vector<int> gradientFiltered;

    for(int i = 0; i < meanFiltered.size() - 1; i++) {
        gradientFiltered.push_back(meanFiltered[i+1] - meanFiltered[i]);
    }
    
    gradientFiltered.push_back(0);

    auto result = minmax_element(gradientFiltered.begin(), gradientFiltered.end());
    float minIndex = result.first - gradientFiltered.begin();
    float maxIndex = result.second - gradientFiltered.begin();

    return (minIndex + maxIndex) / 2;
}
