#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <chrono>

using namespace cv;
using namespace std;

int main(int, char**)
{
    Mat frame;
    Mat data;
    int apiID = cv::CAP_MSMF;
    //configurate VideoCapture for audio
    VideoCapture cap;
    std::vector<int> params { CAP_PROP_AUDIO_ENABLE, 1 };
    // open selected micro using selected API
    cap.open(0, apiID, params);
    if (!cap.isOpened()) {
        cerr << "ERROR! Can't to open file\n";
        return -1;
    }
    cout << "Start grabbing" << endl;
    for (;;)
    {
        cap.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        // write audio PCM in MAT
        if(!frame.empty())
        {
            data.push_back(frame);
        }
    }
    return 0;
}
