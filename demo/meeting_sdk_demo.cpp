#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <glib.h>
#include <sstream>
#include <thread>
#include <mutex>
#include <vector>
#include <sys/syscall.h>
#include <fstream>
#include <iostream>
#include <map>
#include <algorithm>

#include "zoom_sdk.h"
#include "auth_service_interface.h"
#include "meeting_service_interface.h"
#include "meeting_service_components/meeting_audio_interface.h"
#include "meeting_service_components/meeting_participants_ctrl_interface.h"
#include "meeting_service_components/meeting_video_interface.h"
#include "setting_service_interface.h"
#include "MeetingReminderEventListener.h"
#include "MeetingServiceEventListener.h"
#include "AuthServiceEventListener.h"
#include "NetworkConnectionHandler.h"
#include "MeetingParticipantsCtrlEventListener.h"
#include "MeetingRecordingCtrlEventListener.h"
#include "ZoomSDKRenderer.h"
#include "rawdata/rawdata_renderer_interface.h"
#include "rawdata/zoom_rawdata_api.h"
#include "ZoomSDKAudioRawData.h"
#include "meeting_service_components/meeting_recording_interface.h"
#include "ZoomSDKVideoSource.h"
#include "ZoomSDKVirtualAudioMicEvent.h"

USING_ZOOM_SDK_NAMESPACE

GMainLoop* loop;

// Structure to hold meeting configuration
struct MeetingConfig {
    std::string meeting_number;
    std::string token;
    std::string meeting_password;
    std::string recording_token;
    bool getVideoRawData = true;
    bool getAudioRawData = true;
    bool sendVideoRawData = false;
    bool sendAudioRawData = false;
};

// Class to manage a single Zoom meeting instance
class ZoomMeeting {
public:
    ZoomMeeting(const MeetingConfig& config) : config_(config) {
        audioFile_ = std::ofstream(config_.meeting_number + "_raw_audio.pcm", std::ios::binary);
        InitMeetingSDK();
    }

    ~ZoomMeeting() {
        LeaveMeeting();
        CleanSDK();
        if (audioFile_.is_open()) audioFile_.close();
    }

    void Start() {
        AuthMeetingSDK();
        thread_ = std::thread(&ZoomMeeting::Run, this);
    }

    void Run() {
        g_main_loop_run(g_main_loop_new(NULL, FALSE));
    }

private:
    MeetingConfig config_;
    std::thread thread_;
    std::ofstream audioFile_; // File to save raw audio

    IAuthService* m_pAuthService = nullptr;
    IMeetingService* m_pMeetingService = nullptr;
    ISettingService* m_pSettingService = nullptr;
    IMeetingRecordingController* m_pRecordController = nullptr;
    IMeetingParticipantsController* m_pParticipantsController = nullptr;
    ZoomSDKAudioRawData* audio_source = new ZoomSDKAudioRawData();
    IZoomSDKAudioRawDataHelper* audioHelper = nullptr;

    void InitMeetingSDK() {
        InitParam initParam;
        initParam.strWebDomain = "https://zoom.us";
        initParam.strSupportUrl = "https://zoom.us";
        initParam.emLanguageID = LANGUAGE_English;
        initParam.enableLogByDefault = true;
        initParam.enableGenerateDump = true;

        SDKError err = InitSDK(initParam);
        if (err != SDKERR_SUCCESS) {
            std::cerr << "Init SDK failed for meeting " << config_.meeting_number << std::endl;
        }
    }

    void AuthMeetingSDK() {
        SDKError err = CreateAuthService(&m_pAuthService);
        if (err != SDKERR_SUCCESS) return;

        m_pAuthService->SetEvent(new AuthServiceEventListener([this]() { OnAuthenticationComplete(); }));
        AuthContext param;
        param.jwt_token = config_.token.c_str();
        m_pAuthService->SDKAuth(param);
    }

    void OnAuthenticationComplete() {
        JoinMeeting();
    }

    void JoinMeeting() {
        CreateMeetingService(&m_pMeetingService);
        CreateSettingService(&m_pSettingService);

        m_pMeetingService->SetEvent(new MeetingServiceEventListener(
            [this]() { onMeetingJoined(); },
            [this]() { onMeetingEndsQuitApp(); },
            [this]() { onInMeeting(); }
        ));

        m_pParticipantsController = m_pMeetingService->GetMeetingParticipantsController();
        m_pParticipantsController->SetEvent(new MeetingParticipantsCtrlEventListener(
            [this]() { onIsHost(); },
            [this]() { onIsCoHost(); }
        ));

        m_pRecordController = m_pMeetingService->GetMeetingRecordingController();
        m_pRecordController->SetEvent(new MeetingRecordingCtrlEventListener(
            [this]() { onIsGivenRecordingPermission(); }
        ));

        JoinParam joinParam;
        joinParam.userType = SDK_UT_WITHOUT_LOGIN;
        JoinParam4WithoutLogin& withoutloginParam = joinParam.param.withoutloginuserJoin;
        withoutloginParam.meetingNumber = std::stoull(config_.meeting_number);
        withoutloginParam.userName = "LinuxChun";
        withoutloginParam.psw = config_.meeting_password.c_str();
        withoutloginParam.app_privilege_token = config_.recording_token.empty() ? nullptr : config_.recording_token.c_str();
        withoutloginParam.isVideoOff = !config_.sendVideoRawData;
        withoutloginParam.isAudioOff = !config_.sendAudioRawData;

        SDKError err = m_pMeetingService->Join(joinParam);
        if (err != SDKERR_SUCCESS) {
            std::cerr << "Failed to join meeting " << config_.meeting_number << std::endl;
        }
    }

    void onInMeeting() {
        if (m_pMeetingService->GetMeetingStatus() == MEETING_STATUS_INMEETING) {
            CheckAndStartRawRecording(config_.getVideoRawData, config_.getAudioRawData);
        }
    }

    void onMeetingJoined() {
        std::cout << "Joining Meeting " << config_.meeting_number << "...\n";
    }

    void onMeetingEndsQuitApp() {}

    void onIsHost() { CheckAndStartRawRecording(config_.getVideoRawData, config_.getAudioRawData); }
    void onIsCoHost() { CheckAndStartRawRecording(config_.getVideoRawData, config_.getAudioRawData); }
    void onIsGivenRecordingPermission() { CheckAndStartRawRecording(config_.getVideoRawData, config_.getAudioRawData); }

    /*void CheckAndStartRawRecording(bool isVideo, bool isAudio) {
        if (!(isVideo || isAudio)) return;

        m_pRecordController = m_pMeetingService->GetMeetingRecordingController();
        if (m_pRecordController->CanStartRawRecording() == SDKERR_SUCCESS) {
            m_pRecordController->StartRawRecording();
            if (isAudio) {
                audioHelper = GetAudioRawdataHelper();
                audioHelper->subscribe(audio_source);
                audio_source->setAudioCallback([this](const char* data, unsigned int length) {
                    if (audioFile_.is_open()) {
                        audioFile_.write(data, length);
                    }
                });
            }
        }
    }*/
	void CheckAndStartRawRecording(bool isVideo, bool isAudio) {
        if (isAudio) {
            audioHelper = GetAudioRawdataHelper();
            audioHelper->subscribe(audio_source);
            audio_source->setAudioCallback([this](const char* data, unsigned int length) {
                if (audioFile_.is_open()) {
                    audioFile_.write(data, length);
                }
            });
        }
    }

    void LeaveMeeting() {
        if (m_pMeetingService && m_pMeetingService->GetMeetingStatus() != MEETING_STATUS_IDLE) {
            m_pMeetingService->Leave(LEAVE_MEETING);
        }
    }

    void CleanSDK() {
        if (m_pAuthService) DestroyAuthService(m_pAuthService);
        if (m_pMeetingService) DestroyMeetingService(m_pMeetingService);
        if (m_pSettingService) DestroySettingService(m_pSettingService);
        if (audioHelper) audioHelper->unSubscribe();
		
		if (videoSourceHelper_) {
            videoSourceHelper_->setExternalVideoSource(nullptr);
            delete video_source_;
            video_source_ = nullptr;
        }
		
        CleanUPSDK();
    }
};

// Global vector to hold meeting instances
std::vector<std::unique_ptr<ZoomMeeting>> meetings;
std::mutex meetings_mutex;

void ReadTEXTSettings(std::vector<MeetingConfig>& configs) {
    std::string self_dir = getSelfDirPath() + "/config.txt";
    std::ifstream configFile(self_dir);
    if (!configFile) {
        std::cerr << "Error opening config file." << std::endl;
        return;
    }

    std::map<std::string, std::string> configMap;
    std::string line;
    while (std::getline(configFile, line)) {
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);
            key.erase(0, key.find_first_not_of(" \t"));
            key.erase(key.find_last_not_of(" \t") + 1);
            value.erase(0, value.find_first_not_of(" \t"));
            value.erase(value.find_last_not_of(" \t") + 1);
            value.erase(std::remove_if(value.begin(), value.end(), [](char c) { return c == '"' || c == '\r'; }), value.end());
            configMap[key] = value;
        }
    }

    int meetingIdx = 1;
    while (true) {
        std::string prefix = std::to_string(meetingIdx);
        if (configMap.find("meeting_number_" + prefix) == configMap.end()) break;

        MeetingConfig config;
        config.meeting_number = configMap["meeting_number_" + prefix];
        config.token = configMap["token_" + prefix];
        config.meeting_password = configMap["meeting_password_" + prefix];
        config.recording_token = configMap["recording_token_" + prefix];
        config.getVideoRawData = configMap["GetVideoRawData_" + prefix] == "true";
        config.getAudioRawData = configMap["GetAudioRawData_" + prefix] == "true";
        config.sendVideoRawData = configMap["SendVideoRawData_" + prefix] == "true";
        config.sendAudioRawData = configMap["SendAudioRawData_" + prefix] == "true";
        configs.push_back(config);
        meetingIdx++;
    }
}

void my_handler(int s) {
    std::lock_guard<std::mutex> lock(meetings_mutex);
    meetings.clear(); // This will call destructors for each ZoomMeeting instance
    std::exit(0);
}

int main(int argc, char* argv[]) {
    std::vector<MeetingConfig> configs;
    ReadTEXTSettings(configs);

    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    {
        std::lock_guard<std::mutex> lock(meetings_mutex);
        for (const auto& config : configs) {
            meetings.push_back(std::make_unique<ZoomMeeting>(config));
            meetings.back()->Start();
        }
    }

    // Keep main thread alive
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}