#ifndef AZURE_SPEECH_STT_H
#define AZURE_SPEECH_STT_H

#include "rawdata/rawdata_audio_helper_interface.h"
#include "zoom_sdk_def.h"
#include <speechapi_cxx.h> // Microsoft Speech SDK header
#include <string>
#include <memory>

class AzureSpeechSTT : public ZOOM_SDK_NAMESPACE::IAudioRawDataDelegate {
public:
    AzureSpeechSTT(const std::string& outputFilePath, const std::string& subscriptionKey, const std::string& region);
    virtual ~AzureSpeechSTT();

    // Callback for receiving mixed audio raw data
    virtual void onMixedAudioRawDataReceived(ZOOM_SDK_NAMESPACE::AudioRawData* audioRawData) override;

    // Callback for receiving one-way audio raw data (optional, for completeness)
    virtual void onOneWayAudioRawDataReceived(ZOOM_SDK_NAMESPACE::AudioRawData* audioRawData, uint32_t node_id) override;

    // Placeholder for other required virtual methods (left empty for simplicity)
    virtual void onShareAudioRawDataReceived(ZOOM_SDK_NAMESPACE::AudioRawData* data_) override {}
    virtual void onOneWayInterpreterAudioRawDataReceived(ZOOM_SDK_NAMESPACE::AudioRawData* data_, const zchar_t* pLanguageName) override {}

private:
    std::string pcmFilePath_;
    std::string subscriptionKey_;
    std::string region_;
    std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechConfig> speechConfig_;
    std::shared_ptr<Microsoft::CognitiveServices::Speech::Audio::AudioConfig> audioConfig_;
    std::shared_ptr<Microsoft::CognitiveServices::Speech::SpeechRecognizer> recognizer_;
    std::shared_ptr<Microsoft::CognitiveServices::Speech::Audio::PushAudioInputStream> pushStream_;

    void initializeAzureSpeech();
    void processAndSendAudio(ZOOM_SDK_NAMESPACE::AudioRawData* audioRawData);
    void sendToAzureSTT(const char* buffer, unsigned int bufferLen);
    void saveToPCMFile(const char* buffer, unsigned int bufferLen);
};

#endif // AZURE_SPEECH_STT_H