#include "ZoomSDKVideoSource.h"
#include <iostream>
#include <cstring> // For memset

using namespace ZOOMSDK;

ZoomSDKVideoSource::ZoomSDKVideoSource() {
    frameBuffer_ = new uint8_t[WIDTH * HEIGHT * 3]; // RGB buffer
    memset(frameBuffer_, 0, WIDTH * HEIGHT * 3); // Initialize to black
    videoSender_ = nullptr;
}

ZoomSDKVideoSource::~ZoomSDKVideoSource() {
    delete[] frameBuffer_;
}

void ZoomSDKVideoSource::onInitialize(IZoomSDKVideoSender* sender, IList<VideoSourceCapability>* support_cap_list, VideoSourceCapability& suggest_cap) {
    std::cout << "Video source initialized" << std::endl;
    videoSender_ = sender;
    suggest_cap.width = WIDTH;
    suggest_cap.height = HEIGHT;
    suggest_cap.frame = 30; // Corrected from frameRate to frame
}

void ZoomSDKVideoSource::onPropertyChange(IList<VideoSourceCapability>* support_cap_list, VideoSourceCapability suggest_cap) {
    std::cout << "Video source property changed: " << suggest_cap.width << "x" << suggest_cap.height << " @ " << suggest_cap.frame << "fps" << std::endl;
    // Update sender settings if needed
}

void ZoomSDKVideoSource::onStartSend() {
    std::cout << "Started sending video" << std::endl;
    if (videoSender_) {
        videoSender_->sendVideoFrame(reinterpret_cast<char*>(frameBuffer_), WIDTH, HEIGHT, WIDTH * 3, 0); // Added rotation (0), cast to char*
    }
}

void ZoomSDKVideoSource::onStopSend() {
    std::cout << "Stopped sending video" << std::endl;
}

void ZoomSDKVideoSource::onUninitialized() {
    std::cout << "Video source uninitialized" << std::endl;
    videoSender_ = nullptr;
}