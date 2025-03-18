#include "ZoomSDKAudioRawData.h"
#include <iostream>

using namespace ZOOMSDK;

ZoomSDKAudioRawData::ZoomSDKAudioRawData(const char* filePath) {
    mixedAudioFile = filePath ? fopen(filePath, "wb") : nullptr;
}

ZoomSDKAudioRawData::~ZoomSDKAudioRawData() {
    if (mixedAudioFile) fclose(mixedAudioFile);
}

void ZoomSDKAudioRawData::setAudioCallback(std::function<void(const char*, unsigned int)> cb) {
    audioCallback_ = cb;
}

void ZoomSDKAudioRawData::onMixedAudioRawDataReceived(AudioRawData* audioRawData) {
    std::cout << "onMixedAudioRawDataReceived: " << audioRawData->GetBufferLen() << " bytes" << std::endl;
    if (mixedAudioFile) {
        fwrite(audioRawData->GetBuffer(), sizeof(char), audioRawData->GetBufferLen(), mixedAudioFile);
        fflush(mixedAudioFile);
    }
    if (audioCallback_) {
        audioCallback_(audioRawData->GetBuffer(), audioRawData->GetBufferLen());
    }
}