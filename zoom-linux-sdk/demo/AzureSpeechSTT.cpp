#include "AzureSpeechSTT.h"
#include <iostream>
#include <fstream>

using namespace Microsoft::CognitiveServices::Speech;
using namespace Microsoft::CognitiveServices::Speech::Audio;

AzureSpeechSTT::AzureSpeechSTT(const std::string& outputFilePath, const std::string& subscriptionKey, const std::string& region)
    : pcmFilePath_(outputFilePath), subscriptionKey_(subscriptionKey), region_(region) {
    std::cout << "AzureSpeechSTT initialized with file: " << pcmFilePath_ << std::endl;
    initializeAzureSpeech();
}

AzureSpeechSTT::~AzureSpeechSTT() {
    std::cout << "AzureSpeechSTT destroyed" << std::endl;
}

void AzureSpeechSTT::initializeAzureSpeech() {
    try {
        // Create Speech configuration with subscription key and region
        speechConfig_ = SpeechConfig::FromSubscription(subscriptionKey_, region_);
        speechConfig_->SetSpeechRecognitionLanguage("en-US"); // Set language (adjust as needed)

        // Create a push stream for real-time audio input
        pushStream_ = AudioInputStream::CreatePushStream();
        audioConfig_ = AudioConfig::FromStreamInput(pushStream_);

        // Initialize the Speech Recognizer
        recognizer_ = SpeechRecognizer::FromConfig(speechConfig_, audioConfig_);

        // Set up event handlers for recognition results
        recognizer_->Recognizing.Connect([](const SpeechRecognitionEventArgs& e) {
            std::cout << "Recognizing: " << e.Result->Text << std::endl;
        });

        recognizer_->Recognized.Connect([](const SpeechRecognitionEventArgs& e) {
            if (e.Result->Reason == ResultReason::RecognizedSpeech) {
                std::cout << "Recognized: " << e.Result->Text << std::endl;
            } else {
                std::cout << "Recognition failed" << std::endl;
            }
        });

        recognizer_->Canceled.Connect([](const SpeechRecognitionCanceledEventArgs& e) {
            std::cout << "Recognition canceled: " << e.ErrorDetails << std::endl;
        });

        // Start continuous recognition
        recognizer_->StartContinuousRecognitionAsync().get();
        std::cout << "Azure Speech STT initialized and started" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error initializing Azure Speech: " << e.what() << std::endl;
    }
}

void AzureSpeechSTT::onMixedAudioRawDataReceived(ZOOM_SDK_NAMESPACE::AudioRawData* audioRawData) {
    if (!audioRawData) {
        std::cout << "Received null audio raw data" << std::endl;
        return;
    }

    std::cout << "Received mixed audio raw data, buffer length: " << audioRawData->GetBufferLen() << std::endl;
    processAndSendAudio(audioRawData);
}

void AzureSpeechSTT::onOneWayAudioRawDataReceived(ZOOM_SDK_NAMESPACE::AudioRawData* audioRawData, uint32_t node_id) {
    if (!audioRawData) {
        std::cout << "Received null one-way audio raw data" << std::endl;
        return;
    }

    std::cout << "Received one-way audio raw data from node " << node_id << ", buffer length: " << audioRawData->GetBufferLen() << std::endl;
    processAndSendAudio(audioRawData);
}

void AzureSpeechSTT::processAndSendAudio(ZOOM_SDK_NAMESPACE::AudioRawData* audioRawData) {
    const char* buffer = audioRawData->GetBuffer();
    unsigned int bufferLen = audioRawData->GetBufferLen();

    if (buffer && bufferLen > 0) {
        // Send to Azure Speech-to-Text before saving
        sendToAzureSTT(buffer, bufferLen);

        // Save to PCM file after processing
        saveToPCMFile(buffer, bufferLen);
    } else {
        std::cout << "Invalid audio buffer or length" << std::endl;
    }
}

void AzureSpeechSTT::sendToAzureSTT(const char* buffer, unsigned int bufferLen) {
    if (!pushStream_) {
        std::cout << "Push stream not initialized" << std::endl;
        return;
    }

    try {
        // Push the audio data to the Azure Speech service
        pushStream_->Write(reinterpret_cast<const uint8_t*>(buffer), bufferLen);
        std::cout << "Sent " << bufferLen << " bytes to Azure STT" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error sending to Azure STT: " << e.what() << std::endl;
    }
}

void AzureSpeechSTT::saveToPCMFile(const char* buffer, unsigned int bufferLen) {
    std::ofstream pcmFile(pcmFilePath_, std::ios::out | std::ios::binary | std::ios::app);
    if (!pcmFile.is_open()) {
        std::cout << "Failed to open PCM file: " << pcmFilePath_ << std::endl;
        return;
    }

    pcmFile.write(buffer, bufferLen);
    pcmFile.close();
    std::cout << "Saved " << bufferLen << " bytes to " << pcmFilePath_ << std::endl;
}