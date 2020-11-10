#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int, char**)
{
    Mat frame;
    Mat data;
    
    //--- INITIALIZE VIDEOCAPTURES
    int apiID = cv::CAP_MSMF; // 0 = autodetect default API
    //congigurate VideoCapture for video
    VideoCapture cap;
    // open selected camera using selected API
    cap.open("file_name.format", apiID);
    if (!cap.isOpened()) {
        cerr << "ERROR! Can't to open file\n";
        return -1;
    }
    if(!cap.set(CAP_SWITCH_AUDIO_STREAM, true))// If second argument is true, than audio - on and video - off. If second argument is false, than video - on and audio - off
    {
        cerr << "ERROR! The format has not been switched\n";
        return -1;
    }
    if(!cap.set(CAP_PROP_BPS, 16))// Use for formats using lossy compression. Set bit_per_sample for audio. The default will be the one set by the decoder
    {
        cerr << "ERROR! Parameter was not set\n";
        return -1;
    }  

    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl;
    for (;;)
    {
        //--- GRABBING VIDEO
        // wait for a new frame from audio and store it into 'frame'
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
            cout << frame << endl;
        }
    }
    return 0;
}