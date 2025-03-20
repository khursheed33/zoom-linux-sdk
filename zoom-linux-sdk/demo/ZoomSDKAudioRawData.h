// ZoomSDKAudioRawData.h
#ifndef ZOOM_SDK_AUDIO_RAW_DATA_H
#define ZOOM_SDK_AUDIO_RAW_DATA_H

#include "rawdata/rawdata_audio_helper_interface.h"
#include "zoom_sdk_raw_data_def.h"
#include "AzureSTTProcessor.h"

USING_ZOOM_SDK_NAMESPACE

class ZoomSDKAudioRawData : public IZoomSDKAudioRawDataDelegate {
public:
    ZoomSDKAudioRawData();
    virtual ~ZoomSDKAudioRawData();
    virtual void onMixedAudioRawDataReceived(AudioRawData* data_);
    virtual void onOneWayAudioRawDataReceived(AudioRawData* data_, uint32_t node_id);
    virtual void onShareAudioRawDataReceived(AudioRawData* data_);
    virtual void onOneWayInterpreterAudioRawDataReceived(AudioRawData* data_, const zchar_t* pLanguageName);

private:
    AzureSTTProcessor* sttProcessor_;
};

#endif // ZOOM_SDK_AUDIO_RAW_DATA_H