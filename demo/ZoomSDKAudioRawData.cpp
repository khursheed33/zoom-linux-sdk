#include "rawdata/rawdata_audio_helper_interface.h"
#include "ZoomSDKAudioRawData.h"
#include "zoom_sdk_def.h"
#include <iostream>
#include <functional>

void ZoomSDKAudioRawData::onOneWayAudioRawDataReceived(AudioRawData* audioRawData, uint32_t node_id) {}

void ZoomSDKAudioRawData::onMixedAudioRawDataReceived(AudioRawData* audioRawData) {
    std::cout << "Received onMixedAudioRawDataReceived" << std::endl;
    if (audioCallback_ && audioRawData) {
        audioCallback_(audioRawData->GetBuffer(), audioRawData->GetBufferLen());
    }
}

void ZoomSDKAudioRawData::onShareAudioRawDataReceived(AudioRawData* data_) {}
void ZoomSDKAudioRawData::onOneWayInterpreterAudioRawDataReceived(AudioRawData* data_, const zchar_t* pLanguageName) {}