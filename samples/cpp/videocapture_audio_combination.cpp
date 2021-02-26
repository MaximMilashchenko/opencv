#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>
#include <thread>

using namespace cv;
using namespace std;

int main(int, char**)
{
    Mat frame_video;
    Mat frame_audio;
    Mat audio_data;

    int apiID = cv::CAP_MSMF;
    //congigurate VideoCapture for video and audio
    VideoCapture cap_video;
    VideoCapture cap_audio;
    // open selected camera using selected API
    cap_video.open(0, apiID);
    if (!cap_video.isOpened()) {
        cerr << "ERROR! Can't to open camera\n";
        return -1;
    }  
    // open selected micro using selected API
    std::vector<int> params { CAP_PROP_AUDIO_ENABLE , static_cast<int>(1) };
    cap_audio.open(0, apiID, params);
    if (!cap_audio.isOpened()) {
        cerr << "ERROR! Can't to open micro\n";
        return -1;
    } 
    // GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl;
    for (;;)
    {
        //--- GRABBING VIDEO
        cap_video.read(frame_video);
        cap_audio.read(frame_audio);
        // check if we succeeded
        if (frame_video.empty() && frame_audio.empty()) {
            cerr << "ERROR! blank frame video and frame audio grabbed\n";
            break;
        }
        // show live and wait for a key with timeout long enough to show images
        if(!frame_video.empty())
        {
            imshow("Live", frame_video);
            if (waitKey(5) >= 0)
                break;
        }
        // write audio PCM in MAT
        if(!frame_audio.empty())
        {
            audio_data.push_back(frame_audio);
        }
    }
    return 0;
}
