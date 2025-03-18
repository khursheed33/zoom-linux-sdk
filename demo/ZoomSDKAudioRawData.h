// ZoomSDKAudioRawData.h
#pragma once
#include "zoom_sdk.h"
#include "rawdata/zoom_rawdata_api.h" // For IAudioRawDataDelegate and AudioRawData
#include <functional>

using namespace ZOOMSDK;

class ZoomSDKAudioRawData : public IAudioRawDataDelegate {
private:
    FILE* mixedAudioFile;

public:
    ZoomSDKAudioRawData(const char* filePath);
    virtual ~ZoomSDKAudioRawData();

    virtual void onMixedAudioRawDataReceived(AudioRawData* data_); // Declaration only
    virtual void onOneWayAudioRawDataReceived(AudioRawData* data_, uint32_t node_id) {}
    virtual void onShareAudioRawDataReceived(AudioRawData* data_) {}
    virtual void onOneWayInterpreterAudioRawDataReceived(AudioRawData* data_, const zchar_t* pLanguageName) {}
};