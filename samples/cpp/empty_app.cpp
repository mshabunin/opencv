#include "opencv2/core.hpp"

int main()
{
    cv::Mat m(100, 100, CV_8UC3, cv::Scalar(0));
    // m = m + m;
    cv::add(m, m, m);
    return 0;
}

