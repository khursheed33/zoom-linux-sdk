// AzureSTTProcessor.h
#ifndef AZURE_STT_PROCESSOR_H
#define AZURE_STT_PROCESSOR_H

#include <string>
#include <memory>
#include "speechapi_cxx.h"

using namespace Microsoft::CognitiveServices::Speech;

class AzureSTTProcessor {
public:
    AzureSTTProcessor(const std::string& subscriptionKey, const std::string& region);
    ~AzureSTTProcessor();

    void ProcessAudioBuffer(const char* buffer, unsigned int bufferLen);

private:
    std::string subscriptionKey_;
    std::string region_;
    std::shared_ptr<SpeechConfig> speechConfig_;
    std::shared_ptr<Audio::PushAudioInputStream> pushStream_; // Fully qualified
    std::shared_ptr<Audio::AudioConfig> audioConfig_;        // Fully qualified
    std::shared_ptr<SpeechRecognizer> recognizer_;

    void InitializeSTT();
};

#endif // AZURE_STT_PROCESSOR_H