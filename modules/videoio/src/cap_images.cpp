/*M///////////////////////////////////////////////////////////////////////////////////////
//
//  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.
//
//  By downloading, copying, installing or using the software you agree to this license.
//  If you do not agree to this license, do not download, install,
//  copy or use the software.
//
//
//                        Intel License Agreement
//                For Open Source Computer Vision Library
//
// Copyright (C) 2008, Nils Hasler, all rights reserved.
// Third party copyrights are property of their respective owners.
//
// Redistribution and use in source and binary forms, with or without modification,
// are permitted provided that the following conditions are met:
//
//   * Redistribution's of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//
//   * Redistribution's in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//
//   * The name of Intel Corporation may not be used to endorse or promote products
//     derived from this software without specific prior written permission.
//
// This software is provided by the copyright holders and contributors "as is" and
// any express or implied warranties, including, but not limited to, the implied
// warranties of merchantability and fitness for a particular purpose are disclaimed.
// In no event shall the Intel Corporation or contributors be liable for any direct,
// indirect, incidental, special, exemplary, or consequential damages
// (including, but not limited to, procurement of substitute goods or services;
// loss of use, data, or profits; or business interruption) however caused
// and on any theory of liability, whether in contract, strict liability,
// or tort (including negligence or otherwise) arising in any way out of
// the use of this software, even if advised of the possibility of such damage.
//
//M*/

// Author: Nils Hasler <hasler@mpi-inf.mpg.de>
//
//         Max-Planck-Institut Informatik

//
// capture video from a sequence of images
// the filename when opening can either be a printf pattern such as
// video%04d.png or the first frame of the sequence i.e. video0001.png
//

#include "precomp.hpp"

#include "opencv2/core/utils/filesystem.hpp"
#include "opencv2/videoio/name_pattern.private.hpp"

#if 0
#define CV_WARN(message)
#else
#define CV_WARN(message) CV_LOG_INFO(NULL, "CAP_IMAGES warning: %s (%s:%d)" << message)
#endif

using namespace cv;
namespace cv {

class CvCapture_Images : public CvCapture
{
public:
    CvCapture_Images()
    {
        length = 0;
        frame = NULL;
        grabbedInOpen = false;
    }

    virtual ~CvCapture_Images() CV_OVERRIDE
    {
        close();
    }

    virtual bool open(const char* _filename);
    virtual void close();
    virtual double getProperty(int) const CV_OVERRIDE;
    virtual bool setProperty(int, double) CV_OVERRIDE;
    virtual bool grabFrame() CV_OVERRIDE;
    virtual IplImage* retrieveFrame(int) CV_OVERRIDE;

    int getCaptureDomain() /*const*/ CV_OVERRIDE { return cv::CAP_IMAGES; }
private:
    unsigned countImages(NamePattern probe) const;
protected:
    NamePattern pattern;
    unsigned length; // length of sequence

    IplImage* frame;
    bool grabbedInOpen;
};


void CvCapture_Images::close()
{
    length = 0;
    pattern = NamePattern();
    cvReleaseImage(&frame);
}


bool CvCapture_Images::grabFrame()
{
    std::string filename = pattern.getName();
    CV_Assert(!filename.empty());

    if (grabbedInOpen)
    {
        grabbedInOpen = false;
        pattern.next();
        return frame != NULL;
    }

    cvReleaseImage(&frame);
    frame = cvLoadImage(filename.c_str(), CV_LOAD_IMAGE_UNCHANGED);
    if (frame)
        pattern.next();

    return frame != NULL;
}

IplImage* CvCapture_Images::retrieveFrame(int)
{
    return grabbedInOpen ? NULL : frame;
}

double CvCapture_Images::getProperty(int id) const
{
    switch(id)
    {
    case CV_CAP_PROP_POS_MSEC:
        CV_WARN("collections of images don't have framerates");
        return 0;
    case CV_CAP_PROP_POS_FRAMES:
        return pattern.getIdx();
    case CV_CAP_PROP_FRAME_COUNT:
        return length;
    case CV_CAP_PROP_POS_AVI_RATIO:
        return (double)pattern.getIdx() / (double)(length - 1);
    case CV_CAP_PROP_FRAME_WIDTH:
        return frame ? frame->width : 0;
    case CV_CAP_PROP_FRAME_HEIGHT:
        return frame ? frame->height : 0;
    case CV_CAP_PROP_FPS:
        CV_WARN("collections of images don't have framerates");
        return 1;
    case CV_CAP_PROP_FOURCC:
        CV_WARN("collections of images don't have 4-character codes");
        return 0;
    }
    return 0;
}

bool CvCapture_Images::setProperty(int id, double value)
{
    switch(id)
    {
    case CV_CAP_PROP_POS_MSEC:
    case CV_CAP_PROP_POS_FRAMES:
        if(value < 0) {
            CV_WARN("seeking to negative positions does not work - clamping");
            value = 0;
        }
        if(value >= length) {
            CV_WARN("seeking beyond end of sequence - clamping");
            value = length - 1;
        }
        pattern.setIdx(cvRound(value));
        if (pattern.getIdx() != 0)
            grabbedInOpen = false; // grabbed frame is not valid anymore
        return true;
    case CV_CAP_PROP_POS_AVI_RATIO:
        if(value > 1) {
            CV_WARN("seeking beyond end of sequence - clamping");
            value = 1;
        } else if(value < 0) {
            CV_WARN("seeking to negative positions does not work - clamping");
            value = 0;
        }
        pattern.setIdx(cvRound((length - 1) * value));
        if (pattern.getIdx() != 0)
            grabbedInOpen = false; // grabbed frame is not valid anymore
        return true;
    }
    CV_WARN("unknown/unhandled property");
    return false;
}

unsigned CvCapture_Images::countImages(NamePattern probe) const
{
    probe.setIdx(0);
    for (;;)
    {
        std::string filename = probe.getName();
        if (!utils::fs::exists(filename))
        {
            break;
        }
        if (!cvHaveImageReader(filename.c_str()))
        {
            CV_LOG_INFO(NULL, "CAP_IMAGES: Stop scanning. Can't read image file: " << filename);
            break;
        }
        probe.next();
    }
    return probe.getIdx();
}

bool CvCapture_Images::open(const char * _filename)
{
    close();

    pattern = NamePattern::parse(_filename);
    if (!pattern.isValid())
        return false;

    // determine the length of the sequence
    length = countImages(pattern);
    if (length == 0)
    {
        if (pattern.getIdx() == 0 && pattern.getOffset() == 0) // allow starting with 0 or 1
        {
            pattern.bumpOffset();
            length = countImages(pattern);
        }
    }
    if (length == 0)
    {
        close();
        return false;
    }

    // grab frame to enable properties retrieval
    bool grabRes = grabFrame();
    grabbedInOpen = true;

    return grabRes;
}


CvCapture* cvCreateFileCapture_Images(const char * filename)
{
    CvCapture_Images* capture = new CvCapture_Images;

    try
    {
        if (capture->open(filename))
            return capture;
        delete capture;
    }
    catch (...)
    {
        delete capture;
        throw;
    }

    return NULL;
}

//
//
// image sequence writer
//
//
class CvVideoWriter_Images CV_FINAL : public CvVideoWriter
{
public:
    CvVideoWriter_Images()
    {
    }
    virtual ~CvVideoWriter_Images() { close(); }

    virtual bool open( const char* _filename );
    virtual void close();
    virtual bool setProperty( int, double ); // FIXIT doesn't work: IVideoWriter interface only!
    virtual bool writeFrame( const IplImage* ) CV_OVERRIDE;

    int getCaptureDomain() const CV_OVERRIDE { return cv::CAP_IMAGES; }
protected:
    NamePattern pattern;
    std::vector<int> params;
};

bool CvVideoWriter_Images::writeFrame( const IplImage* image )
{
    std::string filename = pattern.getName();

    std::vector<int> image_params = params;
    image_params.push_back(0); // append parameters 'stop' mark
    image_params.push_back(0);
    int ret = cvSaveImage(filename.c_str(), image, &image_params[0]);

    pattern.next();

    return ret > 0;
}

void CvVideoWriter_Images::close()
{
    pattern = NamePattern();
    params.clear();
}


bool CvVideoWriter_Images::open( const char* _filename )
{
    close();

    pattern = NamePattern::parse(_filename);
    if (!pattern.isValid())
    {
        return false;
    }

    std::string filename = pattern.getName();
    if (!cvHaveImageWriter(filename.c_str()))
    {
        close();
        return false;
    }
    params.clear();
    return true;
}


bool CvVideoWriter_Images::setProperty( int id, double value )
{
    if (id >= cv::CAP_PROP_IMAGES_BASE && id < cv::CAP_PROP_IMAGES_LAST)
    {
        params.push_back( id - cv::CAP_PROP_IMAGES_BASE );
        params.push_back( static_cast<int>( value ) );
        return true;
    }
    return false; // not supported
}


CvVideoWriter* cvCreateVideoWriter_Images( const char* filename )
{
    CvVideoWriter_Images *writer = new CvVideoWriter_Images;

    try
    {
        if (writer->open(filename))
            return writer;
        delete writer;
    }
    catch (...)
    {
        delete writer;
        throw;
    }

    return 0;
}


} // namespace
