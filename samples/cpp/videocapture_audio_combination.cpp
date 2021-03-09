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
    std::vector<Mat> video_data;
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
    cout << "Start grabbing" << endl;
    for (;;)
    {
        cap_video.read(frame_video);
        cap_audio.read(frame_audio);
        if (frame_video.empty() && frame_audio.empty()) {
            cerr << "ERROR! blank frame video and frame audio grabbed\n";
            break;
        }
        // write video frame
        if(!frame_video.empty())
        {
            video_data.push_back(frame_video);
        }
        // write audio PCM in MAT
        if(!frame_audio.empty())
        {
            audio_data.push_back(frame_audio);
        }
    }
    return 0;
}
