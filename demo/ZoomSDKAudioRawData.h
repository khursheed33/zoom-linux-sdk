// ZoomSDKAudioRawData.h
#pragma once
#include "zoom_sdk.h"
#include "zoom_sdk_raw_data_def.h"              // Fixed: Removed "h/" prefix
#include "rawdata/zoom_rawdata_api.h"           // Fixed: Removed "h/" prefix
#include "rawdata/rawdata_audio_helper_interface.h" // Fixed: Removed "h/" prefix
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