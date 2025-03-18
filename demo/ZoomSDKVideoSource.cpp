// SendVideoRawData
#include "ZoomSDKVideoSource.h"
#include <iostream>
#include <thread>
#include <string>
#include <chrono>

using namespace std;

constexpr auto WIDTH = 640;
constexpr auto HEIGHT = 480;

void PlayVideoFileToVirtualCamera(IZoomSDKVideoSender* video_sender, const std::string& video_source) {
    // Placeholder: Implement video file reading and sending logic here
    // Example: Use FFmpeg to read YUV420P frames and send via video_sender
    std::cout << "Playing video from " << video_source << " (placeholder)" << endl;
}

class ZoomSDKVideoSource : public IZoomSDKVideoSource {
private:
    IZoomSDKVideoSender* video_sender_ = nullptr;
    std::string video_source_;
    int video_play_flag_ = -1; // Moved from global to instance scope
    int width_ = WIDTH;
    int height_ = HEIGHT;

public:
    ZoomSDKVideoSource(string video_source) : video_source_(video_source) {}

    void onInitialize(IZoomSDKVideoSender* sender, IList<VideoSourceCapability>* support_cap_list, VideoSourceCapability& suggest_cap) {
        std::cout << "ZoomSDKVideoSource onInitialize waiting for turnOn chat command" << endl;
        video_sender_ = sender;
    }

    void onPropertyChange(IList<VideoSourceCapability>* support_cap_list, VideoSourceCapability suggest_cap) {
        std::cout << "onPropertyChange" << endl;
        std::cout << "suggest frame: " << suggest_cap.frame << endl;
        std::cout << "suggest size: " << suggest_cap.width << "x" << suggest_cap.height << endl;
        width_ = suggest_cap.width;
        height_ = suggest_cap.height;
        std::cout << "calculated frameLen: " << height_ / 2 * 3 * width_ << endl;
    }

    void onStartSend() {
        std::cout << "onStartSend" << endl;
        if (video_sender_ && video_play_flag_ != 1) {
            while (video_play_flag_ > -1) {}
            video_play_flag_ = 1;
            thread(PlayVideoFileToVirtualCamera, video_sender_, video_source_).detach();
        } else {
            std::cout << "video_sender_ is null or already sending" << endl;
        }
    }

    void onStopSend() {
        std::cout << "onCameraStopSend" << endl;
        video_play_flag_ = 0;
    }

    void onUninitialized() {
        std::cout << "onUninitialized" << endl;
        video_sender_ = nullptr;
    }
};