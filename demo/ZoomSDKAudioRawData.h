// ZoomSDKAudioRawData.h
#pragma once
#include "zoom_sdk.h"
#include "h/rawdata/zoom_rawdata_api.h" // Should define AudioRawData fully
#include "h/rawdata/rawdata_audio_helper_interface.h" // Defines IZoomSDKAudioRawDataDelegate
#include <functional>
#include <cstdio> // For FILE

using namespace ZOOMSDK;

class ZoomSDKAudioRawData : public IZoomSDKAudioRawDataDelegate {
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