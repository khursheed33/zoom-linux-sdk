// ZoomSDKVideoSource.h
#pragma once
#include "zoom_sdk.h" // Base SDK definitions
#include "h/rawdata/rawdata_video_source_helper_interface.h" // Defines IZoomSDKVideoSource, IZoomSDKVideoSender, VideoSourceCapability
#include <cstdint> // For uint8_t

using namespace ZOOMSDK;

// Constants defined only here
constexpr int WIDTH = 640;
constexpr int HEIGHT = 480;

class ZoomSDKVideoSource : public IZoomSDKVideoSource {
public:
    ZoomSDKVideoSource();
    virtual ~ZoomSDKVideoSource();

    // IZoomSDKVideoSource methods (from rawdata_video_source_helper_interface.h)
    virtual void onInitialize(IZoomSDKVideoSender* sender, IList<VideoSourceCapability>* support_cap_list, VideoSourceCapability& suggest_cap);
    virtual void onPropertyChange(IList<VideoSourceCapability>* support_cap_list, VideoSourceCapability suggest_cap);
    virtual void onStartSend();
    virtual void onStopSend();
    virtual void onUninitialized();

private:
    IZoomSDKVideoSender* videoSender_;
    uint8_t* frameBuffer_; // Example buffer for video frames
};