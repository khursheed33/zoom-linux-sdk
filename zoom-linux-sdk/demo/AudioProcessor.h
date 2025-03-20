#ifndef AUDIO_PROCESSOR_H
#define AUDIO_PROCESSOR_H

#include "rawdata/rawdata_audio_helper_interface.h"
#include "zoom_sdk_def.h"
#include <string>

class AudioProcessor : public ZOOM_SDK_NAMESPACE::IAudioRawDataDelegate {
public:
    AudioProcessor(const std::string& outputFilePath);
    virtual ~AudioProcessor();

    // Callback for receiving mixed audio raw data
    virtual void onMixedAudioRawDataReceived(ZOOM_SDK_NAMESPACE::AudioRawData* audioRawData) override;

    // Callback for receiving one-way audio raw data (optional, for completeness)
    virtual void onOneWayAudioRawDataReceived(ZOOM_SDK_NAMESPACE::AudioRawData* audioRawData, uint32_t node_id) override;

    // Placeholder for other required virtual methods (left empty for simplicity)
    virtual void onShareAudioRawDataReceived(ZOOM_SDK_NAMESPACE::AudioRawData* data_) override {}
    virtual void onOneWayInterpreterAudioRawDataReceived(ZOOM_SDK_NAMESPACE::AudioRawData* data_, const zchar_t* pLanguageName) override {}

private:
    std::string pcmFilePath_;
    void processAndSendAudio(ZOOM_SDK_NAMESPACE::AudioRawData* audioRawData);
    void sendToDummyAPI(const char* buffer, unsigned int bufferLen);
    void saveToPCMFile(const char* buffer, unsigned int bufferLen);
};

#endif // AUDIO_PROCESSOR_H