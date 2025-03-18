#include "ZoomSDKAudioRawData.h"
#include <iostream>

using namespace ZOOMSDK;

ZoomSDKAudioRawData::ZoomSDKAudioRawData(const char* filePath) {
    mixedAudioFile = fopen(filePath, "wb");
}

ZoomSDKAudioRawData::~ZoomSDKAudioRawData() {
    if (mixedAudioFile) fclose(mixedAudioFile);
}

void ZoomSDKAudioRawData::onMixedAudioRawDataReceived(AudioRawData* audioRawData) {
    std::cout << "onMixedAudioRawDataReceived: " << audioRawData->GetBufferLen() << " bytes" << std::endl;
    if (mixedAudioFile) {
        fwrite(audioRawData->GetBuffer(), sizeof(char), audioRawData->GetBufferLen(), mixedAudioFile);
        fflush(mixedAudioFile);
    }
}