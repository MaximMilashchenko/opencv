#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <fstream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    CommandLineParser parser(argc, argv, "{@audio||}");
    string file = parser.get<string>("@audio");

    if (file.empty())
    {
        return 1;
    }

    Mat frame;
    vector<vector<Mat>> audioData;
    VideoCapture cap;
    vector<int> params {    CAP_PROP_AUDIO_STREAM, 0,
                            CAP_PROP_VIDEO_STREAM, -1,
                            CAP_PROP_AUDIO_DATA_DEPTH, CV_16S   };

    cap.open(file, CAP_GSTREAMER, params);
    if (!cap.isOpened())
    {
        cerr << "ERROR! Can't to open file: " + file << endl;
        return -1;
    }

    const int audioBaseIndex = (int)cap.get(CAP_PROP_AUDIO_BASE_INDEX);
    const int numberOfChannels = (int)cap.get(CAP_PROP_AUDIO_TOTAL_CHANNELS);
    cout << "CAP_PROP_AUDIO_DATA_DEPTH: " << depthToString((int)cap.get(CAP_PROP_AUDIO_DATA_DEPTH)) << endl;
    cout << "CAP_PROP_AUDIO_SAMPLES_PER_SECOND: " << cap.get(CAP_PROP_AUDIO_SAMPLES_PER_SECOND) << endl;
    cout << "CAP_PROP_AUDIO_TOTAL_CHANNELS: " << numberOfChannels << endl;
    //cout << "CAP_PROP_AUDIO_TOTAL_STREAMS: " << cap.get(CAP_PROP_AUDIO_TOTAL_STREAMS) << endl;

    int numberOfSamples = 0;
    audioData.resize(numberOfChannels);
    for (;;)
    {
        if (cap.grab())
        {
            for (int nCh = 0; nCh < numberOfChannels; nCh++)
            {
                cap.retrieve(frame, audioBaseIndex+nCh);
                //std::cout << frame << std::endl;
                /*std::string::size_type bpos = file.find('/home/max/workSpace/GstreamerPipeline/opencv_extra/testdata/highgui/audio/');
                std::string::size_type epos = file.find('.');
                std::string file_ = file.substr (74, epos);
                std::string str = "/home/max/workSpace/audiomeasure/" + file_ + ".txt";
                std::ofstream out(str, std::ios::app);
                if (out.is_open())
                {
                    for (int i = 0; i < frame.cols; i++)
                    {
                        //out << frame.at <signed short> (0, i) << std::endl;
                    }
                } */
                audioData[nCh].push_back(frame);
                numberOfSamples+=frame.cols;
            }
        }
        else { break; }
    }

    cout << "Number of samples: " << numberOfSamples << endl;

    return 0;
}
