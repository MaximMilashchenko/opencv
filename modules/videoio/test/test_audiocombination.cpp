#include "test_precomp.hpp"

#include <iostream>
namespace opencv_test { namespace {

//file format, epsilon, video type, weight, height, number of frame, psnr Threshold, backend
typedef std::tuple<std::string, double, int, int, int, int, double, std::pair<std::string, int> > Param;

const Param audio_params[] =
{
    Param("mp4", 0.15, CV_8UC3, 240, 320, 90, 30., {"CAP_MSMF", cv::CAP_MSMF})
};

class AudioTestFixture : public testing::TestWithParam <Param>
{
public:
    AudioTestFixture(): 
        format(get<0>(GetParam())),
        epsilon(get<1>(GetParam())),
        videoType(get<2>(GetParam())),
        height(get<3>(GetParam())),
        width(get<4>(GetParam())),
        numberOfFrames(get<5>(GetParam())),
        psnrThreshold(get<6>(GetParam())),
        backend(get<7>(GetParam())),
        root("audio/"),
        fileName("test_audio"),
        params({ CAP_PROP_AUDIO_STREAM, 0,
                 CAP_PROP_VIDEO_STREAM, 0, 
                 CAP_PROP_AUDIO_DATA_DEPTH, CV_16S  }) {};
    void doTest()
    {
        getValidAudioData();
        getValidVideoData();
        getFileData();
        comparisonAudio();
        comparisonVideo();
    }

private:
    void getValidAudioData()
    {
        const double step = 3.14/22050;
        double value = 0;
        for (int j = 0; j < 3; j++)
        {
            value = 0;
            for (int i = 0; i < 44100; i++)
            {
                validAudioData.push_back(sin(value));
                value += step;
            }
        }
    }
    void getValidVideoData()
    {   
        Mat img(height, width, videoType);
        for (int i = 0; i < numberOfFrames; ++i)
        {
            generateFrame(i, numberOfFrames, img);
            validVideoData.push_back(img);
        }
    }
    void getFileData()
    {
        ASSERT_TRUE(cap.open(findDataFile(root + fileName + "." + format), backend.second, params));
        const int audioBaseIndex = static_cast<int>(cap.get(cv::CAP_PROP_AUDIO_BASE_INDEX));
        double f = 0;
        for (;;)
        {
            if (cap.grab())
            {
                cap.retrieve(videoFrame);
                cap.retrieve(audioFrame, audioBaseIndex);
                if (!videoFrame.empty())
                    videoData.push_back(videoFrame);
                for (int i = 0; i < audioFrame.cols; i++)
                {
                    f = ((double) audioFrame.at<signed short>(0,i)) / (double) 32768;
                    audioData.push_back(f);
                }
            }
            else
            {
                break;
            }
        }
        ASSERT_FALSE(audioData.empty() || videoData.empty());
    }
    void comparisonAudio()
    {
        for (int i = 0; i < validAudioData.size(); i++)
        {
            EXPECT_LE(fabs(validAudioData[i] - audioData[i]), epsilon) << "sample index " << i;
        }
    }
    void comparisonVideo()
    {
        ASSERT_EQ(validVideoData.size(), videoData.size());
        double minPsnrOriginal = 1000;
        for (int i = 0; i < validVideoData.size(); i++)
        {
            ASSERT_EQ(videoData[i].rows, validVideoData[i].rows) << "The dimension of the rows does not match. Frame index: " << i;
            ASSERT_EQ(videoData[i].cols, validVideoData[i].cols) << "The dimension of the cols does not match. Frame index: " << i;
            double psnr = cvtest::PSNR(validVideoData[i], videoData[i]);
            if (psnr < minPsnrOriginal)
                minPsnrOriginal = psnr;
        }
        EXPECT_GE(minPsnrOriginal, psnrThreshold);
    }
protected:
    const std::string format;
    const std::pair<std::string, int> backend;
    const std::string root;
    const std::string fileName;

    const double epsilon;
    const int videoType;
    const int height;
    const int width;
    const int numberOfFrames;
    const double psnrThreshold;

    std::vector<Mat> validVideoData;
    std::vector<double> validAudioData;
    std::vector<Mat> videoData;
    std::vector<double> audioData;

    Mat audioFrame;
    Mat videoFrame;
    std::vector<int> params;
    VideoCapture cap;
};

class Media : public AudioTestFixture{};

TEST_P(Media, audio)
{
    if (!videoio_registry::hasBackend(cv::VideoCaptureAPIs(backend.second)))
        throw SkipTestException(backend.first + " backend was not found");

    doTest();
}

INSTANTIATE_TEST_CASE_P(/**/, Media, testing::ValuesIn(audio_params));

}}