#include <stdio.h>
#include <string>
#include <iostream>
#include "opencv2/opencv.hpp"
#include <boost/filesystem.hpp>

#include "constants.h"

using namespace std;
using namespace cv;

int main(int argc, char** argv) {
    if(argc < 3) {
        cout << "Incorrect usage:" << endl;
        cout << "CalibrateCamera [source dir] [output.xml]" << endl;
        return 1;
    }
    
    string path = argv[1];
    string outputFile = argv[2];
    
}
