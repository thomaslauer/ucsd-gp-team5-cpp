#include <stdio.h>
#include <string>
#include <iostream>
#include "opencv2/opencv.hpp"
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

#include "constants.hpp"
#include "calibration.hpp"

using namespace std;
using namespace cv;
using namespace boost::filesystem;

int main(int argc, char** argv) {
    if(argc != 4) {
        cout << "Incorrect usage:" << endl;
        cout << argv[0] << " [source dir] [homographyImg] [output.xml]" << endl;
        return 1;
    }
    
    string path = argv[1];
    string homographyImg = argv[2];
    string outputFile = argv[3];

    Calibration cb;

    cb.generateCalibration(path, homographyImg);
    cb.save(outputFile);

}
