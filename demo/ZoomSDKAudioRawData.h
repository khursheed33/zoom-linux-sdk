// ZoomSDKAudioRawData.h
#pragma once
#include "zoom_sdk.h"
#include "h/rawdata/zoom_rawdata_api.h" // Defines AudioRawData
#include "h/rawdata/rawdata_audio_helper_interface.h" // Defines IAudioRawDataDelegate and IZoomSDKAudioRawDataDelegate
#include <functional>

using namespace ZOOMSDK;

class ZoomSDKAudioRawData : public IAudioRawDataDelegate {
private:
    FILE* mixedAudioFile;
    std::function<void(const char*, unsigned int)> audioCallback_; // Add callback support

public:
    ZoomSDKAudioRawData(const char* filePath = nullptr); // Default parameter
    virtual ~ZoomSDKAudioRawData();

    void setAudioCallback(std::function<void(const char*, unsigned int)> cb); // Add setter for callback

    virtual void onMixedAudioRawDataReceived(AudioRawData* data_);
    virtual void onOneWayAudioRawDataReceived(AudioRawData* data_, uint32_t node_id) {}
    virtual void onShareAudioRawDataReceived(AudioRawData* data_) {}
    virtual void onOneWayInterpreterAudioRawDataReceived(AudioRawData* data_, const zchar_t* pLanguageName) {}
};