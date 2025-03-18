#include "ZoomSDKVideoSource.h"
#include <iostream>
#include <cstring> // For memset

using namespace ZOOMSDK;

ZoomSDKVideoSource::ZoomSDKVideoSource() {
    frameBuffer_ = new uint8_t[WIDTH * HEIGHT * 3]; // RGB buffer, adjust as needed
    memset(frameBuffer_, 0, WIDTH * HEIGHT * 3); // Initialize to black
    videoSender_ = nullptr;
}

ZoomSDKVideoSource::~ZoomSDKVideoSource() {
    delete[] frameBuffer_;
}

void ZoomSDKVideoSource::onInitialize(IZoomSDKVideoSender* sender, IList<VideoSourceCapability>* support_cap_list, VideoSourceCapability& current_cap) {
    std::cout << "Video source initialized" << std::endl;
    videoSender_ = sender;
    current_cap.width = WIDTH;
    current_cap.height = HEIGHT;
    current_cap.frameRate = 30; // Example frame rate
}

void ZoomSDKVideoSource::onStartSend() {
    std::cout << "Started sending video" << std::endl;
    if (videoSender_) {
        // Example: Send a black frame (expand this with actual video data)
        videoSender_->sendVideoFrame(frameBuffer_, WIDTH, HEIGHT, WIDTH * 3, 30);
    }
}

void ZoomSDKVideoSource::onStopSend() {
    std::cout << "Stopped sending video" << std::endl;
}

void ZoomSDKVideoSource::onUninitialized() {
    std::cout << "Video source uninitialized" << std::endl;
    videoSender_ = nullptr;
}