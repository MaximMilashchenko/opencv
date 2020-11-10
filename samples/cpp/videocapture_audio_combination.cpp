#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int, char**)
{
    Mat frame_video;
    Mat frame_audio;
    Mat audio_data;
    
    //--- INITIALIZE VIDEOCAPTURES
    int apiID = cv::CAP_MSMF; // 0 = autodetect default API
    //congigurate VideoCapture for video
    VideoCapture cap_video;
    // open selected camera using selected API
    cap_video.open("file_name.format", apiID);//"C:\\Users\\mmilashc\\Desktop\\OCVextra\\opencv_extra\\testdata\\highgui\\audio\\mp4\\test_16bit_2channels_44100hz.mp4", apiID);
    if (!cap_video.isOpened()) {
        cerr << "ERROR! Can't to open file\n";
        return -1;
    }
    //congigurate VideoCapture for audio
    VideoCapture cap_audio;
    // open selected camera using selected API
    cap_audio.open("file_name.format", apiID);//("C:\\Users\\mmilashc\\Desktop\\OCVextra\\opencv_extra\\testdata\\highgui\\audio\\mp4\\test_16bit_2channels_44100hz.mp4", apiID);
    if (!cap_audio.isOpened()) {
        cerr << "ERROR! Can't to open file\n";
        return -1;
    }
    if(!cap_audio.set(CAP_SWITCH_AUDIO_STREAM, true))// If second argument is true, than audio - on and video - off. If second argument is false, than video - on and audio - off
    {
        cerr << "ERROR! The format has not been switched\n";
        return -1;
    }
    if(!cap_audio.set(CAP_PROP_BPS, 16))// Use for formats using lossy compression. Set bit_per_sample for audio. The default will be the one set by the decoder
    {
        cerr << "ERROR! Parameter was not set\n";
        return -1;
    }  

    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl;
    for (;;)
    {
        //--- GRABBING VIDEO
        cap_video.read(frame_video);
        cap_audio.read(frame_audio);
        // check if we succeeded
        if (frame_video.empty() && frame_audio.empty()) {
            cerr << "ERROR! blank frame_video and frame_audio grabbed\n";
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
            cout << frame_audio << endl;
        }
    }
    return 0;
}