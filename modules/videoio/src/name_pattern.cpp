// This file is part of OpenCV project.
// It is subject to the license terms in the LICENSE file found in the top-level directory
// of this distribution and at http://opencv.org/license.html.

#include "opencv2/videoio/name_pattern.private.hpp"
#include "precomp.hpp"

namespace cv {

NamePattern::NamePattern()
    : offset(0), idx(0), valid(false)
{
}

bool NamePattern::isValid() const
{
    return valid;
}

std::string NamePattern::getName() const
{
    CV_Assert(isValid());
    return cv::format(pattern.c_str(), (int)(offset + idx));
}

NamePattern NamePattern::parse(const std::string & filename)
{
    NamePattern res;
    if (filename.empty())
        return res;
    std::string::size_type pos = filename.find('%');
    if (pos != std::string::npos)
    {
        if (res.initWithPattern(filename, pos))
            res.valid = true;
    }
    else
    {
        if (res.initWithNumber(filename)) // no pattern filename was given - extract the pattern
            res.valid = true;
    }
    if (res.valid)
    {
        CV_LOG_INFO(NULL, "Deduced pattern: " << res.pattern << " @ " << res.offset);
    }
    return res;
}

bool NamePattern::initWithPattern(const std::string &filename, std::string::size_type pos)
{
    const std::string::size_type len = filename.size();
    pos++; // skip '%''
    if (pos >= len)
        return false; // bad pattern
    if (filename[pos] == '0') // optional zero prefix
    {
        pos++; // skip '0'
        if (pos >= len)
            return false; // bad pattern
    }
    if (filename[pos] >= '1' && filename[pos] <= '9') // optional numeric size (1..9) (one symbol only)
    {
        pos++; // skip '1'..'9'
        if (pos >= len)
            return false; // bad pattern
    }
    if (filename[pos] == 'd' || filename[pos] == 'u')
    {
        pos++; // skip 'd' or 'u'
        if (pos > len)
            return false;
        if (pos == len || filename.find('%', pos) == std::string::npos)
        {
            // end of string '...%5d' OR no more patterns
            pattern = filename;
            return true;
        }
        // invalid multiple patterns
    }
    // error, expected '0?[1-9][du]' pattern, got: %s
    return false;
}

bool NamePattern::initWithNumber(const std::string &filename)
{
    const std::string::size_type len = filename.size();
    std::string::size_type pos = filename.rfind('/');
#ifdef _WIN32
    if (pos == std::string::npos)
        pos = filename.rfind('\\');
#endif
    if (pos != std::string::npos)
        pos++;
    else
        pos = 0;

    while (pos < len && !isdigit(filename[pos])) pos++;

    if (pos == len)
        return false; // can't find starting number (in the name of file)

    std::string::size_type pos0 = pos;

    const int64_t max_number = 1000000000;
    if (max_number >= INT_MAX)
        return false; // offset is 'int'

    int number_str_size = 0;
    uint64_t number = 0;
    while (pos < len && isdigit(filename[pos]))
    {
        char ch = filename[pos];
        number = (number * 10) + (uint64_t)((int)ch - (int)'0');
        if (number >= max_number)
            return false;
        number_str_size++;
        if (number_str_size > 64)
            return false; // don't allow huge zero prefixes
        pos++;
    }
    if (number_str_size <= 0)
        return false;

    offset = (int)number;

    std::string result;
    if (pos0 > 0)
        result += filename.substr(0, pos0);
    result += cv::format("%%0%dd", number_str_size);
    if (pos < len)
        result += filename.substr(pos);
    pattern = result;
    return true;
}

} // cv::
