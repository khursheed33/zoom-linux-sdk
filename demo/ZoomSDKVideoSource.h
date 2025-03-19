// ZoomSDKVideoSource.h
#pragma once
#include "zoom_sdk.h"
#include "rawdata/rawdata_video_source_helper_interface.h"
#include <cstdint>

using namespace ZOOMSDK;

constexpr int VIDEO_WIDTH = 640;  // Moved outside class
constexpr int VIDEO_HEIGHT = 480; // Moved outside class

class ZoomSDKVideoSource : public IZoomSDKVideoSource {
public:
    ZoomSDKVideoSource();
    virtual ~ZoomSDKVideoSource();

    virtual void onInitialize(IZoomSDKVideoSender* sender, IList<VideoSourceCapability>* support_cap_list, VideoSourceCapability& suggest_cap);
    virtual void onPropertyChange(IList<VideoSourceCapability>* support_cap_list, VideoSourceCapability suggest_cap);
    virtual void onStartSend();
    virtual void onStopSend();
    virtual void onUninitialized();

private:
    IZoomSDKVideoSender* videoSender_;
    uint8_t* frameBuffer_;
};