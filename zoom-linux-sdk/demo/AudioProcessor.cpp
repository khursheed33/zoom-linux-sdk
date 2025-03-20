#include "AudioProcessor.h"
#include <iostream>
#include <fstream>

AudioProcessor::AudioProcessor(const std::string& outputFilePath) : pcmFilePath_(outputFilePath) {
    std::cout << "AudioProcessor initialized with file: " << pcmFilePath_ << std::endl;
}

AudioProcessor::~AudioProcessor() {
    std::cout << "AudioProcessor destroyed" << std::endl;
}

void AudioProcessor::onMixedAudioRawDataReceived(ZOOM_SDK_NAMESPACE::AudioRawData* audioRawData) {
    if (!audioRawData) {
        std::cout << "Received null audio raw data" << std::endl;
        return;
    }

    std::cout << "Received mixed audio raw data, buffer length: " << audioRawData->GetBufferLen() << std::endl;
    processAndSendAudio(audioRawData);
}

void AudioProcessor::onOneWayAudioRawDataReceived(ZOOM_SDK_NAMESPACE::AudioRawData* audioRawData, uint32_t node_id) {
    if (!audioRawData) {
        std::cout << "Received null one-way audio raw data" << std::endl;
        return;
    }

    std::cout << "Received one-way audio raw data from node " << node_id << ", buffer length: " << audioRawData->GetBufferLen() << std::endl;
    processAndSendAudio(audioRawData);
}

void AudioProcessor::processAndSendAudio(ZOOM_SDK_NAMESPACE::AudioRawData* audioRawData) {
    const char* buffer = audioRawData->GetBuffer();
    unsigned int bufferLen = audioRawData->GetBufferLen();

    if (buffer && bufferLen > 0) {
        // Send to dummy API before saving
        sendToDummyAPI(buffer, bufferLen);

        // Save to PCM file after processing
        saveToPCMFile(buffer, bufferLen);
    } else {
        std::cout << "Invalid audio buffer or length" << std::endl;
    }
}

void AudioProcessor::sendToDummyAPI(const char* buffer, unsigned int bufferLen) {
    // Simulate sending data to a dummy API
    std::cout << "Sending " << bufferLen << " bytes to Dummy API..." << std::endl;
    // For now, just print the first few bytes as a dummy action
    std::cout << "Sample data: ";
    for (unsigned int i = 0; i < std::min(bufferLen, 10u); ++i) {
        std::cout << static_cast<int>(buffer[i]) << " ";
    }
    std::cout << "..." << std::endl;

    // In a real scenario, this would be replaced with an actual API call, e.g.:
    // DummyAPI::sendAudio(buffer, bufferLen);
}

void AudioProcessor::saveToPCMFile(const char* buffer, unsigned int bufferLen) {
    std::ofstream pcmFile(pcmFilePath_, std::ios::out | std::ios::binary | std::ios::app);
    if (!pcmFile.is_open()) {
        std::cout << "Failed to open PCM file: " << pcmFilePath_ << std::endl;
        return;
    }

    pcmFile.write(buffer, bufferLen);
    pcmFile.close();
    std::cout << "Saved " << bufferLen << " bytes to " << pcmFilePath_ << std::endl;
}