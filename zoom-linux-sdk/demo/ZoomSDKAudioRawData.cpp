// ZoomSDKAudioRawData.cpp
#include "ZoomSDKAudioRawData.h"
#include <iostream>
#include <cstdlib>

ZoomSDKAudioRawData::ZoomSDKAudioRawData() : sttProcessor_(nullptr) {
    const char* subscriptionKey = std::getenv("AZURE_SUBSCRIPTION_KEY");
    const char* region = std::getenv("AZURE_REGION");
    if (subscriptionKey && region) {
        sttProcessor_ = new AzureSTTProcessor(subscriptionKey, region);
    } else {
        std::cerr << "Error: AZURE_SUBSCRIPTION_KEY or AZURE_REGION not set" << std::endl;
    }
}

ZoomSDKAudioRawData::~ZoomSDKAudioRawData() {
    delete sttProcessor_;
}

void ZoomSDKAudioRawData::onOneWayAudioRawDataReceived(AudioRawData* audioRawData, uint32_t node_id) {
    // Unchanged
}

void ZoomSDKAudioRawData::onMixedAudioRawDataReceived(AudioRawData* audioRawData) {
    std::cout << "Received onMixedAudioRawDataReceived" << std::endl;
    if (sttProcessor_ && audioRawData && audioRawData->GetBuffer()) {
        sttProcessor_->ProcessAudioBuffer((char*)audioRawData->GetBuffer(), audioRawData->GetBufferLen());
    }
}

void ZoomSDKAudioRawData::onShareAudioRawDataReceived(AudioRawData* data_) {
    // Unchanged
}

void ZoomSDKAudioRawData::onOneWayInterpreterAudioRawDataReceived(AudioRawData* data_, const zchar_t* pLanguageName) {
    // Unchanged
}