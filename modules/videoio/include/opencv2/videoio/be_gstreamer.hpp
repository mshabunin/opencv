#ifndef BE_GSTREAMER_HPP
#define BE_GSTREAMER_HPP

#ifndef OPENCV_WITH_GSTREAMER
#error \
Please add \
GStreamer to the \
dependencies
#endif

#include <gst/gst.h>
#include <gst/base/gstbasesink.h>
#include <gst/app/gstappsink.h>

#include <string>
#include <iostream>
#include <sstream>

#include "opencv2/core/utility.hpp"
#include "opencv2/videoio/experimental.hpp"

namespace cv { namespace experimental {


inline std::string gstStateName(GstState s)
{
    switch (s)
    {
    case GST_STATE_VOID_PENDING: return "GST_STATE_VOID_PENDING";
    case GST_STATE_NULL: return "GST_STATE_NULL";
    case GST_STATE_READY: return "GST_STATE_READY";
    case GST_STATE_PAUSED: return "GST_STATE_PAUSED";
    case GST_STATE_PLAYING: return "GST_STATE_PLAYING";
    }
    return "invalid";
}


inline bool changeState(GstPipeline *pipeline, GstState state)
{
    GstStateChangeReturn res = gst_element_set_state(GST_ELEMENT(pipeline), state);
    if (res == GST_STATE_CHANGE_ASYNC)
        res =  gst_element_get_state(GST_ELEMENT(pipeline), NULL, NULL, GST_CLOCK_TIME_NONE);
    if (res != GST_STATE_CHANGE_SUCCESS)
    {
        std::cerr << "Failed to go to state " << gstStateName(state) << ": " << res << std::endl;
        return false;
    }
    std::cout << "State changed to " << gstStateName(state) << std::endl;
    return true;
}


class CV_EXPORTS_W GStreamerBackend
{
public:
    static GStreamerBackend * create() { return new GStreamerBackend(); }
    static GStreamerBackend * create(const PropMap &) { return new GStreamerBackend(); }
    GStreamerBackend() : pipeline(0), sink(0)
    {
        using namespace std;
        gst_init(NULL, NULL);
    }

    bool openFile(const std::string &filename)
    {
        using namespace std;
        cout << "Filename: " << filename << endl;
        stringstream p;
        p << "uridecodebin uri=file://" << filename << " ! "
          << "appsink name=sink sync=false async=false drop=false max-buffers=1 emit-signals=false wait-on-eos=false";
        cout << "Pipeline: " << p.str() << endl;

        GError * err = NULL;
        GstElement * parsed = gst_parse_launch(p.str().c_str(), &err);
        if (!parsed)
        {
            cout << "Failed to parse: " << err->message << endl;
            return false;
        }
        else if (err)
        {
            cout << "Recoverable error: " << err->message << endl;
        }

        pipeline = GST_PIPELINE(parsed);
        sink = GST_APP_SINK(gst_bin_get_by_name(GST_BIN(pipeline), "sink"));
        if (!pipeline || !sink || !GST_IS_PIPELINE(pipeline) || !GST_IS_APP_SINK(sink))
        {
            cout << "Bad pipeline or sink!" << endl;
            return false;
        }

        gst_pipeline_use_clock(GST_PIPELINE(pipeline), NULL);

        cout << "SINK options" << endl << boolalpha;
        cout << "  sync: " << (bool)gst_base_sink_get_sync(GST_BASE_SINK(sink)) << endl;

        cout << "  max lateness: " << gst_base_sink_get_max_lateness(GST_BASE_SINK(sink)) << endl;
        cout << "  qos: " << (bool)gst_base_sink_is_qos_enabled(GST_BASE_SINK(sink)) << endl;
        cout << "  async: " << (bool)gst_base_sink_is_async_enabled(GST_BASE_SINK(sink)) << endl;
        cout << "  tuning sync: " << gst_base_sink_get_ts_offset(GST_BASE_SINK(sink)) << endl;
        cout << "  latency: " << gst_base_sink_get_latency(GST_BASE_SINK(sink)) << endl;

        cout << "  drop: " << (bool)gst_app_sink_get_drop(GST_APP_SINK(sink)) << endl;
        cout << "  wait on EOS: " << (bool)gst_app_sink_get_wait_on_eos(GST_APP_SINK(sink)) << endl;
        cout << "  max buffers: " << gst_app_sink_get_max_buffers(GST_APP_SINK(sink)) << endl;

        if (!changeState(pipeline, GST_STATE_PLAYING))
        {
            release();
            return false;
        }
        return true;
    }

    bool isOpened() const
    {
        return pipeline && sink;
    }
    bool read(cv::OutputArray out)
    {
        if (!isOpened())
            return false;

        bool res = true;
        using namespace std;
        gboolean eos = gst_app_sink_is_eos(GST_APP_SINK(sink));
        if (eos)
        {
            cerr << "EOS!" << endl;
            return false;
        }

        GstSample * sample = gst_app_sink_try_pull_sample(GST_APP_SINK(sink), 5 * GST_SECOND);
        if (!sample)
        {
            cerr << "Bad sample" << endl;
            return false;
        }

        GstBuffer * buf = gst_sample_get_buffer(sample);
        if (buf)
        {
            GstMapInfo info;
            gst_buffer_map(buf, &info, GST_MAP_READ);
            {
                cv::Mat temp(1, info.size, CV_8U, info.data);
                temp.copyTo(out);
            }
            gst_buffer_unmap(buf, &info);
        }
        else
        {
            cerr << "Map failed" << endl;
            res = false;
        }
        gst_sample_unref(sample);
        return res;
    }
    void release()
    {
        if (pipeline && GST_IS_OBJECT(pipeline))
        {
            gst_object_unref(GST_OBJECT(pipeline));
            pipeline = 0;
        }
        if (sink)
        {
            sink = 0;
        }
    }

public:
    GstPipeline*   pipeline;
    GstAppSink*   sink;
};


template class NewCapture<GStreamerBackend>;

typedef NewCapture<GStreamerBackend> GStreamerCapture;

}} //cv::experimental::

#endif // BE_GSTREAMER_HPP
