// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

// Note: all tests here are DISABLED by default due specific requirements.
// Don't use #if 0 - these tests should be tested for compilation at least.
//
// Usage: opencv_test_videoio --gtest_also_run_disabled_tests --gtest_filter=*videoio_camera*<tested case>*

#include "test_precomp.hpp"
#include <opencv2/core/utils/configuration.private.hpp>

namespace opencv_test { namespace {

const int bit_per_sample[] = { 8, 16, 32 };

static void test_readFrames(VideoCapture& capture, double framePeriod, const int N = 100)
{
    Mat frame;
    int64 sysTimePrev = cv::getTickCount();
    double camTimePrev = 0.0;
    const double cvTickFreq = cv::getTickFrequency();
    const bool validTick = cvTickFreq != 0;

    for (int i = 0; i < N; i++)
    {
        SCOPED_TRACE(cv::format("frame=%d", i));

        capture >> frame;
        const int64 sysTimeCurr = cv::getTickCount();
        const double camTimeCurr = capture.get(cv::CAP_PROP_POS_MSEC);
        ASSERT_FALSE(frame.empty());

        if (i > 1 && validTick)
        {
            const double sysTimeElapsedSecs = (sysTimeCurr - sysTimePrev) / cvTickFreq;
            const double camTimeElapsedSecs = (camTimeCurr - camTimePrev) / 1000.;
            EXPECT_NEAR(sysTimeElapsedSecs, camTimeElapsedSecs, framePeriod);        
        }

        sysTimePrev = sysTimeCurr;
        camTimePrev = camTimeCurr;
    }
}

TEST(DISABLED_videoio_micro, basic)
{
    const int nChannels = 2;
    const int nSamplesPerSec = 44100;

    int FrameCursize = 0;
    int TmpFrameSize = 0;
    double framePeriod = 0;
    Mat frame;

    std::vector<int> params { CAP_PROP_AUDIO_ENABLE, 1 };
    VideoCapture capture;

    for(const int bps : bit_per_sample)
    {
        capture.open(0, cv::CAP_MSMF, params);
        ASSERT_TRUE(capture.isOpened());
        ASSERT_TRUE(capture.set(CAP_PROP_BPS, bps));
        for(int i = 0; i < 20; i++)
        {
            capture >> frame;
            TmpFrameSize = frame.rows*nChannels*(bps/8);
            if(TmpFrameSize > FrameCursize)
                FrameCursize = TmpFrameSize;
        }
        if(FrameCursize != 0)
        {
            framePeriod = 1./((nSamplesPerSec*nChannels*bps)/(FrameCursize*bps));
            test_readFrames(capture, framePeriod, 1000);
        }
        capture.release();
    }   
}

}} // namespace
