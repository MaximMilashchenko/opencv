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
    //C:\\Users\\mmilashc\\Desktop\\OCVExtra\\opencv_extra\\testdata\\highgui\\video\\big_buck_bunny.avi
    //C:\\Users\\mmilashc\\Desktop\\OCVExtra\\opencv_extra\\testdata\\highgui\\audio\\mp4\\test_16bit_2channels_44100hz.mp4
    //--- INITIALIZE VIDEOCAPTURES
    int apiID = cv::CAP_MSMF; // 0 = autodetect default API
    //congigurate VideoCapture for video
    VideoCapture cap;
    // open selected camera using selected API
    std::vector<int> params { CAP_PROP_AUDIO_ENABLE , static_cast<int>(1) };
    cap.open(0, apiID, params);
    //ASSERT_TRUE(cap.set(CAP_PROP_BPS, 0));
    if (!cap.isOpened()) {
        cerr << "ERROR! Can't to open file\n";
        return -1;
    }
    /*if(!cap.set(CAP_PROP_AUDIO_ENABLE , true))// If second argument is true, than audio - on and video - off. If second argument is false, than video - on and audio - off
    {
        cerr << "ERROR! The format has not been switched\n";
        return -1;
    }*/
    //if(!cap.set(CAP_PROP_BPS, 16))// Use for formats using lossy compression. Set bit_per_sample for audio. The default will be the one set by the decoder
    //{
    //    cerr << "ERROR! Parameter was not set\n";
    //    return -1;
    //}  

    //--- GRAB AND WRITE LOOP
    cout << "Start grabbing" << endl;
    char* data1 = NULL; 
    for (;;)
    {
        //--- GRABBING AUDIO
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
            /*
                audio processing
            */
            data.push_back(frame);
            char* data1 = new char[frame.rows];
            std::fstream mm("C:\\Users\\mmilashc\\Desktop\\bin\\wszBinFile.bin" , std::ios::app | std::ios::in | std::ios::out | std::ios::binary);
            for(unsigned int i = 0; i < frame.rows; i++)
            {
                data1[i] = frame.at<unsigned char>(i,0);
            } 
            mm.write(data1, frame.rows);
            mm.close();
            //cout << frame << endl;
        }
    }
    return 0;
}