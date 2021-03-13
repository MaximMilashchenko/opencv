// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "test_precomp.hpp"

namespace opencv_test { namespace {

typedef std::tuple<std::string, double, std::pair<std::string, int> > Param;

const Param audio_params[] =
{
    Param("wav", 0.0001, {"CAP_MSMF", cv::CAP_MSMF}),
    Param("mp3", 0.1, {"CAP_MSMF", cv::CAP_MSMF}),
    Param("mp4", 0.15, {"CAP_MSMF", cv::CAP_MSMF})
};

class AudioTestFixture : public testing::TestWithParam <Param>
{
public:
    AudioTestFixture(): 
        format(get<0>(GetParam())),
        epsilon(get<1>(GetParam())),
        backend(get<2>(GetParam())),
        root("audio/"),
        fileName("test_audio"),
        params({ CAP_PROP_AUDIO_STREAM, 0,
                 CAP_PROP_VIDEO_STREAM, -1, 
                 CAP_PROP_AUDIO_DATA_DEPTH, CV_16S }) {};
    void doTest()
    {
        getValidData();
        getFileData();
        comparison();
    }

private:
    void getValidData()
    {
        const double step = 3.14/22050;
        double value = 0;
        for(int j = 0; j < 3; j++)
        {
            value = 0;
            for(int i = 0; i < 44100; i++)
            {
                validData.push_back(sin(value));
                value += step;
            }
        }
    }
    void getFileData()
    {
        ASSERT_TRUE(cap.open(findDataFile(root + fileName + "." + format), backend.second, params));
        const int audioBaseIndex = static_cast<int>(cap.get(cv::CAP_PROP_AUDIO_BASE_INDEX));
        double f = 0;
        for (;;)
        {
            if(cap.grab())
            {
                cap.retrieve(frame, audioBaseIndex);
                for(int i = 0; i < frame.cols; i++)
                {
                    f = ((double) frame.at<signed short>(0,i)) / (double) 32768;
                    fileData.push_back(f);
                }
            }
            else
            {
                break;
            }
        }
        ASSERT_FALSE(fileData.empty());
    }
    void comparison()
    {
        for(int i = 0; i < validData.size(); i++)
        {
            EXPECT_LE(fabs(validData[i] - fileData[i]), epsilon) << "sample index " << i;
        }
    }
protected:
    const std::string format;
    const std::pair<std::string, int> backend;
    const std::string root;
    const std::string fileName;
    const double epsilon;
    std::vector<int> params;
    std::vector<double> validData;
    std::vector<double> fileData;

    Mat frame;
    VideoCapture cap;
};

class Audio : public AudioTestFixture{};

TEST_P(Audio, audio)
{
    if (!videoio_registry::hasBackend(cv::VideoCaptureAPIs(backend.second)))
        throw SkipTestException(backend.first + " backend was not found");

    doTest();
}

INSTANTIATE_TEST_CASE_P(/**/, Audio, testing::ValuesIn(audio_params));
}} //namespace
