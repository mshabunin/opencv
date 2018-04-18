// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.
#include "test_precomp.hpp"
#include "opencv2/videoio/experimental.hpp"
#include <utility>

CV_TEST_MAIN("highgui")

namespace opencv_test {

TEST(Experimental, Props)
{
    using namespace cv::experimental;
    PropMap props;
    props.set<CP_FPS>(make_pair(30, 1));
    props.set<CP_Name>("abcde");
    props.set<CP_Size>(make_pair(300, 200));
    props.set<CP_Gstreamer_Sync>(true);
    props.print(std::cout);
    EXPECT_DOUBLE_EQ(props.get<CP_FPS>().toFPS(), 30.0 / 1.0);
    EXPECT_EQ(props.get<CP_Name>(), string("abcde"));
    EXPECT_EQ(props.get<CP_Size>(), IntPair::create(300, 200));
    EXPECT_EQ(props.get<CP_Gstreamer_Sync>(), true);
}

} // cvtest::

#ifdef HAVE_GSTREAMER

#include "opencv2/videoio/be_gstreamer.hpp"

namespace opencv_test {

TEST(Experimental, GStreamer)
{
    using namespace cv::experimental;
    GStreamerCapture cap;
    cap.be->openFile("/home/mshabuni/work/gstreamer/experiment/frames/video.mp4");
    Mat frame;
    cap >> frame >> frame >> frame;
    EXPECT_EQ((int)frame.total(), 320 * 240 * 3);
}

}

#endif

