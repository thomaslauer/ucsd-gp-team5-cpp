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
    
    for(auto& entry : boost::make_iterator_range(directory_iterator(path), {})) {
        cout << "loading file " << entry << endl;
        Mat image = imread(entry.path().string());
        imshow("img", image);
        waitKey(0);
    }
}
