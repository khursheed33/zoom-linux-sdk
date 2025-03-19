#include "ZoomSDKVideoSource.h"
#include <iostream>
#include <cstring>

using namespace ZOOMSDK;

ZoomSDKVideoSource::ZoomSDKVideoSource() {
    frameBuffer_ = new uint8_t[VIDEO_WIDTH * VIDEO_HEIGHT * 3];
    memset(frameBuffer_, 0, VIDEO_WIDTH * VIDEO_HEIGHT * 3);
    videoSender_ = nullptr;
}

ZoomSDKVideoSource::~ZoomSDKVideoSource() {
    delete[] frameBuffer_;
}

void ZoomSDKVideoSource::onInitialize(IZoomSDKVideoSender* sender, IList<VideoSourceCapability>* support_cap_list, VideoSourceCapability& suggest_cap) {
    std::cout << "Video source initialized" << std::endl;
    videoSender_ = sender;
    suggest_cap.width = VIDEO_WIDTH;
    suggest_cap.height = VIDEO_HEIGHT;
    suggest_cap.frame = 30;
}

void ZoomSDKVideoSource::onPropertyChange(IList<VideoSourceCapability>* support_cap_list, VideoSourceCapability suggest_cap) {
    std::cout << "Video source property changed: " << suggest_cap.width << "x" << suggest_cap.height << " @ " << suggest_cap.frame << "fps" << std::endl;
}

void ZoomSDKVideoSource::onStartSend() {
    std::cout << "Started sending video" << std::endl;
    if (videoSender_) {
        videoSender_->sendVideoFrame(reinterpret_cast<char*>(frameBuffer_), VIDEO_WIDTH, VIDEO_HEIGHT, VIDEO_WIDTH * 3, 0);
    }
}

void ZoomSDKVideoSource::onStopSend() {
    std::cout << "Stopped sending video" << std::endl;
}

void ZoomSDKVideoSource::onUninitialized() {
    std::cout << "Video source uninitialized" << std::endl;
    videoSender_ = nullptr;
}