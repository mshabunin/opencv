// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "precomp.hpp"

CV_IMPL int cvNamedWindow( const char* name, int flags )
{
    cv::namedWindow(name, flags);
    return 1;
}

CV_IMPL void cvSetWindowProperty(const char* name, int prop_id, double prop_value)
{
    cv::setWindowProperty(name, prop_id, prop_value);
}

CV_IMPL double cvGetWindowProperty(const char* name, int prop_id)
{
    return cv::getWindowProperty(name, prop_id);
}

CV_IMPL void cvShowImage( const char* name, const CvArr* arr )
{
    cv::Mat img = cv::cvarrToMat(arr, true);
    showImageImpl(name, img);
}

CV_IMPL void cvResizeWindow( const char* name, int width, int height )
{
    cv::resizeWindow(name, width, height);
}

CV_IMPL void cvMoveWindow( const char* name, int x, int y )
{
    cv::moveWindow(name, x, y);
}

CV_IMPL void cvDestroyWindow( const char* name )
{
    cv::destroyWindow(name);
}

CV_IMPL void cvDestroyAllWindows(void)
{
    cv::destroyAllWindows();
}

CV_IMPL void* cvGetWindowHandle( const char* )
{
    return NULL;
}

CV_IMPL const char* cvGetWindowName( void* )
{
    return NULL;
}

static void legacyTrackbarCallbackProxy(int pos, void * data)
{
    CV_Assert(data != NULL);
    CvTrackbarCallback * actual_callback = (CvTrackbarCallback*)data;
    (*actual_callback)(pos);
}

CV_IMPL int cvCreateTrackbar( const char* trackbar_name, const char* window_name,
                             int* value, int count, CvTrackbarCallback on_change)
{
    return cv::createTrackbar(trackbar_name, window_name,
                              value, count, &legacyTrackbarCallbackProxy, (void*)on_change);
}

CV_IMPL int cvCreateTrackbar2( const char* trackbar_name, const char* window_name,
                              int* value, int count, CvTrackbarCallback2 on_change,
                              void* userdata)
{
    return cv::createTrackbar(trackbar_name, window_name, value, count, on_change, userdata);
}


CV_IMPL int cvGetTrackbarPos( const char* trackbar_name, const char* window_name )
{
    return cv::getTrackbarPos(trackbar_name, window_name);
}

CV_IMPL void cvSetTrackbarPos( const char* trackbar_name, const char* window_name, int pos )
{
    cv::setTrackbarPos(trackbar_name, window_name, pos);
}

CV_IMPL void cvSetTrackbarMax(const char* trackbar_name, const char* window_name, int maxval)
{
    cv::setTrackbarMax(trackbar_name, window_name, maxval);
}

CV_IMPL void cvSetTrackbarMin(const char* trackbar_name, const char* window_name, int minval)
{
    cv::setTrackbarMin(trackbar_name, window_name, minval);
}

CV_IMPL void cvSetMouseCallback( const char* window_name, CvMouseCallback on_mouse, void* param)
{
    cv::setMouseCallback(window_name, on_mouse, param);
}

CV_IMPL int cvWaitKey( int delay )
{
    return cv::waitKey(delay);
}

CV_IMPL int cvInitSystem( int , char** )
{
    return 0;
}

CV_IMPL int cvStartWindowThread()
{
    return cv::startWindowThread();
}

CV_IMPL void cvAddText(const CvArr* arr, const char* text, CvPoint org, CvFont *arg2)
{
    cv::Mat mat = cv::cvarrToMat(arr, true);

    cv::QtFont font;
    if (arg2)
        font = { arg2->nameFont, arg2->color, arg2->font_face, arg2->ascii, arg2->greek,
                 arg2->cyrillic, arg2->hscale, arg2->vscale, arg2->shear, arg2->thickness,
                 arg2->dx, arg2->line_type };

    return cv::addText(mat, text, org, font);
}

CV_IMPL void cvDisplayOverlay(const char* name, const char* text, int delayms)
{
    cv::displayOverlay(name, text, delayms);
}

CV_IMPL void cvDisplayStatusBar(const char* name, const char* text, int delayms)
{
    cv::displayStatusBar(name, text, delayms);
}

CV_IMPL void cvSaveWindowParameters(const char* name)
{
    cv::saveWindowParameters(name);
}

CV_IMPL void cvLoadWindowParameters(const char* name)
{
    cv::loadWindowParameters(name);
}

CV_IMPL int cvStartLoop(int (*pt2Func)(int argc, char *argv[]), int argc, char* argv[])
{
    return cv::startLoop(pt2Func, argc, argv);
}

CV_IMPL void cvStopLoop( void )
{
    cv::stopLoop();
}
