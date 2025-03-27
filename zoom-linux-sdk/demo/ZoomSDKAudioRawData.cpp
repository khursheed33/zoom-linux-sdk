#include "ZoomSDKAudioRawData.h"
#include <iostream>
#include <cstdlib>

ZoomSDKAudioRawData::ZoomSDKAudioRawData() : sttProcessor_(nullptr) {
    const char* subscriptionKey = std::getenv("AZURE_SUBSCRIPTION_KEY");
    const char* region = std::getenv("AZURE_REGION");
    if (subscriptionKey && region) {
        std::cout << "Initializing STT with key: " << subscriptionKey << ", region: " << region << std::endl;
        sttProcessor_ = new AzureSTTProcessor(subscriptionKey, region);
        if (sttProcessor_) {
            std::cout << "STT processor initialized successfully" << std::endl;
        } else {
            std::cerr << "Failed to initialize STT processor" << std::endl;
        }
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
        std::cout << "Processing audio buffer, length: " << audioRawData->GetBufferLen() << std::endl;
        sttProcessor_->ProcessAudioBuffer((char*)audioRawData->GetBuffer(), audioRawData->GetBufferLen());
    } else {
        std::cerr << "STT processor or audio data invalid" << std::endl;
        if (!sttProcessor_) std::cerr << "STT processor is null" << std::endl;
        if (!audioRawData) std::cerr << "Audio raw data is null" << std::endl;
        if (audioRawData && !audioRawData->GetBuffer()) std::cerr << "Audio buffer is null" << std::endl;
    }
}

void ZoomSDKAudioRawData::onShareAudioRawDataReceived(AudioRawData* data_) {
    // Unchanged
}

void ZoomSDKAudioRawData::onOneWayInterpreterAudioRawDataReceived(AudioRawData* data_, const zchar_t* pLanguageName) {
    // Unchanged
}