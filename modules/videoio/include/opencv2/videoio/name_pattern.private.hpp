// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#ifndef NAME_PATTERN_HPP
#define NAME_PATTERN_HPP

#ifndef __OPENCV_BUILD
#  error this is a private header which should not be used from outside of the OpenCV library
#endif

#include <string>

namespace cv
{

class NamePattern
{
public:
    NamePattern();
    bool isValid() const;
    std::string getName() const;
    unsigned getOffset() const { return offset; }
    unsigned getIdx() const { return idx; }
    void next() { idx++; }
    void bumpOffset() { offset++; }
    void setIdx(unsigned new_idx) { idx = new_idx; }
    static NamePattern parse(const std::string & filename);
private:
    bool initWithPattern(const std::string &filename, std::string::size_type pos);
    bool initWithNumber(const std::string &filename);
private:
    std::string pattern;
    unsigned offset;
    unsigned idx;
    bool valid;
};

} // cv::

#endif // NAME_PATTERN_HPP
