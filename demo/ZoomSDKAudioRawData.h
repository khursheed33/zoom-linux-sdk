// ZoomSDKAudioRawData.h
#pragma once
#include "zoom_sdk.h"
#include "h/rawdata/zoom_rawdata_api.h" // Defines AudioRawData
#include "h/rawdata/rawdata_audio_helper_interface.h" // Defines IAudioRawDataDelegate and IZoomSDKAudioRawDataDelegate
#include <functional>

using namespace ZOOMSDK;

// Ensure inheritance from the correct interface
class ZoomSDKAudioRawData : public IZoomSDKAudioRawDataDelegate { // Changed to IZoomSDKAudioRawDataDelegate
private:
    FILE* mixedAudioFile;
    std::function<void(const char*, unsigned int)> audioCallback_;

public:
    ZoomSDKAudioRawData(const char* filePath = nullptr);
    virtual ~ZoomSDKAudioRawData();

    void setAudioCallback(std::function<void(const char*, unsigned int)> cb);

    virtual void onMixedAudioRawDataReceived(AudioRawData* data_);
    virtual void onOneWayAudioRawDataReceived(AudioRawData* data_, uint32_t node_id) {}
    virtual void onShareAudioRawDataReceived(AudioRawData* data_) {}
    virtual void onOneWayInterpreterAudioRawDataReceived(AudioRawData* data_, const zchar_t* pLanguageName) {}
};