// AzureSTTProcessor.cpp
#include "AzureSTTProcessor.h"
#include <iostream>

using namespace std;
using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Audio;

AzureSTTProcessor::AzureSTTProcessor(const string& subscriptionKey, const string& region)
    : subscriptionKey_(subscriptionKey), region_(region) {
    InitializeSTT();
}

AzureSTTProcessor::~AzureSTTProcessor() {
    // Smart pointers handle cleanup
}

void AzureSTTProcessor::InitializeSTT() {
    try {
        speechConfig_ = SpeechConfig::FromSubscription(subscriptionKey_, region_);
        auto format = AudioStreamFormat::GetWaveFormatPCM(44100, 16, 1); // 44.1kHz, 16-bit, 1 channel
        speechConfig_->SetSpeechRecognitionLanguage("en-US");

        pushStream_ = AudioInputStream::CreatePushStream(format);
        audioConfig_ = AudioConfig::FromStreamInput(pushStream_);
        recognizer_ = SpeechRecognizer::FromConfig(speechConfig_, audioConfig_);
    } catch (const exception& e) {
        cerr << "Failed to initialize STT: " << e.what() << endl;
    }
}

void AzureSTTProcessor::ProcessAudioBuffer(const char* buffer, unsigned int bufferLen) {
    if (!recognizer_ || !pushStream_) {
        cerr << "STT not initialized" << endl;
        return;
    }

    try {
        pushStream_->Write((uint8_t*)buffer, bufferLen);
        auto resultFuture = recognizer_->RecognizeOnceAsync();
        auto result = resultFuture.get();

        if (result->Reason == ResultReason::RecognizedSpeech) {
            cout << "Transcription: " << result->Text << endl;
        } else if (result->Reason == ResultReason::NoMatch) {
            cout << "No speech recognized" << endl;
        } else {
            // cerr << "Recognition failed: " << result->Reason << endl;
        }
    } catch (const exception& e) {
        cerr << "Error processing audio: " << e.what() << endl;
    }
}