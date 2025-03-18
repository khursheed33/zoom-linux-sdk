// ZoomSDKVideoSource.h
#pragma once
#include "zoom_sdk.h"
#include "h/zoom_sdk_raw_data_def.h" // For VideoSourceCapability, etc.
#include "h/rawdata/zoom_rawdata_api.h" // For IZoomSDKVideoSource
#include <cstdint> // For uint8_t

using namespace ZOOMSDK;

// Constants defined only here
constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;

class ZoomSDKVideoSource : public IZoomSDKVideoSource {
public:
    ZoomSDKVideoSource();
    virtual ~ZoomSDKVideoSource();

    // Core IZoomSDKVideoSource methods (standard in Zoom SDK)
    virtual void onInitialize(IZoomSDKVideoSender* sender, IList<VideoSourceCapability>* support_cap_list, VideoSourceCapability& current_cap);
    virtual void onStartSend();
    virtual void onStopSend();
    virtual void onUninitialized();

private:
    IZoomSDKVideoSender* videoSender_;
    uint8_t* frameBuffer_; // Example buffer for video frames
};