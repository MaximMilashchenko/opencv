#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <fstream>

using namespace cv;
using namespace std;

int main(int, char**)
{
    Mat frame;
    Mat data;
    // INITIALIZE VIDEOCAPTURES
    int apiID = cv::CAP_MSMF; // 0 = autodetect default API
    //congigurate VideoCapture for video
    VideoCapture cap;
    // open selected camera using selected API
    std::vector<int> params { CAP_PROP_AUDIO_ENABLE, 1, CAP_PROP_BPS, 32 };
    cap.open("example_WAV.wav", apiID, params);
    std::cout << "gy" << std::endl;
    //if(!setProperty(CAP_PROP_AUDIO_ENABLE, 1))
    //    return -1;
    if (!cap.isOpened()) {
        cerr << "ERROR! Can't to open file\n";
        return -1;
    }
    // GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl;
    for (;;)
    {
        // GRABBING AUDIOs
        cap.read(frame);
        // check if we succeeded
        if (frame.empty()) {
            cerr << "ERROR! blank frame grabbed\n";
            break;
        }
        // write audio PCM in MAT
        if(!frame.empty())
        {
            cout << frame << endl;
        }
    }
    return 0;
}

//data.push_back(frame);
/*char* data1 = new char[frame.rows];
std::fstream mm("C:\\Users\\mmilashc\\Desktop\\bin\\wszBinFile.bin" , std::ios::app | std::ios::in | std::ios::out | std::ios::binary);
for(unsigned int i = 0; i < frame.rows; i++)
{
    data1[i] = frame.at<unsigned char>(i,0);
} 
mm.write(data1, frame.rows);
mm.close();*/