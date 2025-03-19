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
#include <memory>
#include <chrono>

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

class ZoomMeeting {
public:
    ZoomMeeting(const MeetingConfig& config) : config_(config) {
        std::cout << "Constructing ZoomMeeting for meeting number: " << config_.meeting_number << std::endl;
        audioFile_ = std::ofstream(config_.meeting_number + "_raw_audio.pcm", std::ios::binary);
        if (!audioFile_.is_open()) {
            std::cerr << "Failed to open audio file for meeting " << config_.meeting_number << std::endl;
        } else {
            std::cout << "Audio file opened: " << config_.meeting_number + "_raw_audio.pcm" << std::endl;
        }
        audio_source = new ZoomSDKAudioRawData((config_.meeting_number + "_raw_audio_sdk.pcm").c_str());
        std::cout << "Audio source created for file: " << config_.meeting_number + "_raw_audio_sdk.pcm" << std::endl;
        InitMeetingSDK();
    }

    ~ZoomMeeting() {
        std::cout << "Destroying ZoomMeeting for meeting number: " << config_.meeting_number << std::endl;
        LeaveMeeting();
        CleanSDK();
        if (audioFile_.is_open()) {
            audioFile_.close();
            std::cout << "Audio file closed for meeting: " << config_.meeting_number << std::endl;
        }
    }

    void Start() {
        std::cout << "Starting ZoomMeeting for meeting number: " << config_.meeting_number << std::endl;
        AuthMeetingSDK();
        thread_ = std::thread(&ZoomMeeting::Run, this);
        std::cout << "Thread started for meeting: " << config_.meeting_number << std::endl;
        thread_.detach();
    }

private:
    MeetingConfig config_;
    std::thread thread_;
    std::ofstream audioFile_;
    IAuthService* m_pAuthService = nullptr;
    IMeetingService* m_pMeetingService = nullptr;
    ISettingService* m_pSettingService = nullptr;
    IMeetingRecordingController* m_pRecordController = nullptr;
    IMeetingParticipantsController* m_pParticipantsController = nullptr;
    ZoomSDKAudioRawData* audio_source = nullptr;
    IZoomSDKAudioRawDataHelper* audioHelper = nullptr;

    void InitMeetingSDK() {
        std::cout << "Initializing SDK for meeting: " << config_.meeting_number << std::endl;
        InitParam initParam;
        initParam.strWebDomain = "https://zoom.us";
        initParam.strSupportUrl = "https://zoom.us";
        initParam.emLanguageID = LANGUAGE_English;
        initParam.enableLogByDefault = true;
        initParam.enableGenerateDump = true;

        SDKError err = InitSDK(initParam);
        if (err != SDKERR_SUCCESS) {
            std::cerr << "Init SDK failed for meeting " << config_.meeting_number << ": " << err << std::endl;
        } else {
            std::cout << "SDK initialized successfully for meeting: " << config_.meeting_number << std::endl;
        }
    }

    void AuthMeetingSDK() {
        std::cout << "Authenticating SDK for meeting: " << config_.meeting_number << std::endl;
        SDKError err = CreateAuthService(&m_pAuthService);
        if (err != SDKERR_SUCCESS) {
            std::cerr << "CreateAuthService failed for meeting " << config_.meeting_number << ": " << err << std::endl;
            return;
        }
        std::cout << "Auth service created for meeting: " << config_.meeting_number << std::endl;

        m_pAuthService->SetEvent(new AuthServiceEventListener([this]() { OnAuthenticationComplete(); }));
        std::cout << "Auth event listener set for meeting: " << config_.meeting_number << std::endl;

        AuthContext param;
        param.jwt_token = config_.token.c_str();
        std::cout << "Using JWT token: " << config_.token << " for meeting: " << config_.meeting_number << std::endl;
        err = m_pAuthService->SDKAuth(param);
        if (err != SDKERR_SUCCESS) {
            std::cerr << "SDKAuth failed for meeting " << config_.meeting_number << ": " << err << std::endl;
        } else {
            std::cout << "SDKAuth initiated successfully for meeting: " << config_.meeting_number << std::endl;
        }
    }

    void OnAuthenticationComplete() {
        std::cout << "Authentication completed for meeting: " << config_.meeting_number << std::endl;
        JoinMeeting();
    }

    void JoinMeeting() {
        std::cout << "Attempting to join meeting: " << config_.meeting_number << std::endl;
        SDKError err = CreateMeetingService(&m_pMeetingService);
        if (err != SDKERR_SUCCESS) {
            std::cerr << "CreateMeetingService failed for meeting " << config_.meeting_number << ": " << err << std::endl;
            return;
        }
        std::cout << "Meeting service created for meeting: " << config_.meeting_number << std::endl;

        err = CreateSettingService(&m_pSettingService);
        if (err != SDKERR_SUCCESS) {
            std::cerr << "CreateSettingService failed for meeting " << config_.meeting_number << ": " << err << std::endl;
            return;
        }
        std::cout << "Setting service created for meeting: " << config_.meeting_number << std::endl;

        m_pMeetingService->SetEvent(new ZOOMSDK::MeetingServiceEventListener(
            [this]() { onMeetingJoined(); },
            [this]() { onMeetingEndsQuitApp(); },
            [this]() { onInMeeting(); }
        ));
        std::cout << "Meeting service event listener set for meeting: " << config_.meeting_number << std::endl;

        m_pParticipantsController = m_pMeetingService->GetMeetingParticipantsController();
        m_pParticipantsController->SetEvent(new MeetingParticipantsCtrlEventListener(
            [this]() { onIsHost(); },
            [this]() { onIsCoHost(); }
        ));
        std::cout << "Participants controller event listener set for meeting: " << config_.meeting_number << std::endl;

        m_pRecordController = m_pMeetingService->GetMeetingRecordingController();
        m_pRecordController->SetEvent(new MeetingRecordingCtrlEventListener(
            [this]() { onIsGivenRecordingPermission(); }
        ));
        std::cout << "Recording controller event listener set for meeting: " << config_.meeting_number << std::endl;

        JoinParam joinParam;
        joinParam.userType = SDK_UT_WITHOUT_LOGIN;
        JoinParam4WithoutLogin& withoutloginParam = joinParam.param.withoutloginuserJoin;
        withoutloginParam.meetingNumber = std::stoull(config_.meeting_number);
        withoutloginParam.userName = "LinuxChun";
        withoutloginParam.psw = config_.meeting_password.c_str();
        withoutloginParam.app_privilege_token = config_.recording_token.empty() ? nullptr : config_.recording_token.c_str();
        withoutloginParam.isVideoOff = !config_.sendVideoRawData;
        withoutloginParam.isAudioOff = !config_.sendAudioRawData;

        std::cout << "Joining meeting " << config_.meeting_number << " with username LinuxChun" << std::endl;
        err = m_pMeetingService->Join(joinParam);
        if (err != SDKERR_SUCCESS) {
            std::cerr << "Failed to join meeting " << config_.meeting_number << ": " << err << std::endl;
        } else {
            std::cout << "Successfully initiated join for meeting: " << config_.meeting_number << std::endl;
        }
    }

    void Run() {
        std::cout << "Entering GMainLoop for meeting: " << config_.meeting_number << std::endl;
        GMainLoop* loop = g_main_loop_new(NULL, FALSE);
        g_main_loop_run(loop);
        g_main_loop_unref(loop);
        std::cout << "Exited GMainLoop for meeting: " << config_.meeting_number << std::endl;
    }

    void onInMeeting() {
        std::cout << "Checking meeting status for: " << config_.meeting_number << std::endl;
        if (m_pMeetingService->GetMeetingStatus() == MEETING_STATUS_INMEETING) {
            std::cout << "In meeting " << config_.meeting_number << std::endl;
            CheckAndStartRawRecording(config_.getVideoRawData, config_.getAudioRawData);
        } else {
            std::cout << "Not yet in meeting " << config_.meeting_number << ", status: " << m_pMeetingService->GetMeetingStatus() << std::endl;
        }
    }

    void onMeetingJoined() {
        std::cout << "Joining Meeting " << config_.meeting_number << "...\n";
    }

    void onMeetingEndsQuitApp() {
        std::cout << "Meeting " << config_.meeting_number << " ended\n";
    }

    void onIsHost() {
        std::cout << "Became host for meeting " << config_.meeting_number << std::endl;
        CheckAndStartRawRecording(config_.getVideoRawData, config_.getAudioRawData);
    }

    void onIsCoHost() {
        std::cout << "Became co-host for meeting " << config_.meeting_number << std::endl;
        CheckAndStartRawRecording(config_.getVideoRawData, config_.getAudioRawData);
    }

    void onIsGivenRecordingPermission() {
        std::cout << "Given recording permission for meeting " << config_.meeting_number << std::endl;
        CheckAndStartRawRecording(config_.getVideoRawData, config_.getAudioRawData);
    }

    void CheckAndStartRawRecording(bool isVideo, bool isAudio) {
        std::cout << "Checking raw recording for meeting: " << config_.meeting_number << " (Video: " << isVideo << ", Audio: " << isAudio << ")" << std::endl;
        if (!isAudio) {
            std::cout << "Audio recording disabled for meeting: " << config_.meeting_number << std::endl;
            return;
        }

        m_pRecordController = m_pMeetingService->GetMeetingRecordingController();
        SDKError err = m_pRecordController->CanStartRawRecording();
        if (err != SDKERR_SUCCESS) {
            std::cout << "Cannot start raw recording for meeting " << config_.meeting_number 
                      << ": No permission (err: " << err << ")\n";
            return;
        }
        std::cout << "Permission granted to start raw recording for meeting: " << config_.meeting_number << std::endl;

        err = m_pRecordController->StartRawRecording();
        if (err != SDKERR_SUCCESS) {
            std::cout << "Failed to start raw recording for meeting " << config_.meeting_number 
                      << ": " << err << std::endl;
            return;
        }
        std::cout << "Raw recording started for meeting: " << config_.meeting_number << std::endl;

        audioHelper = GetAudioRawdataHelper();
        if (!audioHelper) {
            std::cerr << "Failed to get audio helper for meeting " << config_.meeting_number << std::endl;
            return;
        }
        std::cout << "Audio raw data helper obtained for meeting: " << config_.meeting_number << std::endl;

        err = audioHelper->subscribe(audio_source);
        if (err != SDKERR_SUCCESS) {
            std::cerr << "Failed to subscribe to audio for meeting " << config_.meeting_number 
                      << ": " << err << std::endl;
            return;
        }
        std::cout << "Subscribed to audio raw data for meeting: " << config_.meeting_number << std::endl;

        audio_source->setAudioCallback([this](const char* data, unsigned int length) {
            std::cout << "Audio callback triggered for meeting " << config_.meeting_number << ": " << length << " bytes" << std::endl;
            if (audioFile_.is_open()) {
                audioFile_.write(data, length);
                audioFile_.flush();
                std::cout << "Wrote " << length << " bytes to audio file for meeting: " << config_.meeting_number << std::endl;
            }
        });
        std::cout << "Started raw audio recording for meeting: " << config_.meeting_number << std::endl;
    }

    void LeaveMeeting() {
        std::cout << "Attempting to leave meeting: " << config_.meeting_number << std::endl;
        if (m_pMeetingService && m_pMeetingService->GetMeetingStatus() != MEETING_STATUS_IDLE) {
            SDKError err = m_pMeetingService->Leave(LEAVE_MEETING);
            if (err == SDKERR_SUCCESS) {
                std::cout << "Successfully left meeting: " << config_.meeting_number << std::endl;
            } else {
                std::cerr << "Failed to leave meeting " << config_.meeting_number << ": " << err << std::endl;
            }
        } else {
            std::cout << "No active meeting to leave for: " << config_.meeting_number << std::endl;
        }
    }

    void CleanSDK() {
        std::cout << "Cleaning up SDK for meeting: " << config_.meeting_number << std::endl;
        if (audioHelper && audio_source) {
            audioHelper->unSubscribe();
            std::cout << "Unsubscribed from audio for meeting: " << config_.meeting_number << std::endl;
            delete audio_source;
            audio_source = nullptr;
            std::cout << "Audio source deleted for meeting: " << config_.meeting_number << std::endl;
        }
        if (m_pAuthService) {
            DestroyAuthService(m_pAuthService);
            std::cout << "Auth service destroyed for meeting: " << config_.meeting_number << std::endl;
        }
        if (m_pMeetingService) {
            DestroyMeetingService(m_pMeetingService);
            std::cout << "Meeting service destroyed for meeting: " << config_.meeting_number << std::endl;
        }
        if (m_pSettingService) {
            DestroySettingService(m_pSettingService);
            std::cout << "Setting service destroyed for meeting: " << config_.meeting_number << std::endl;
        }
        SDKError err = CleanUPSDK();
        if (err != SDKERR_SUCCESS) {
            std::cerr << "CleanUPSDK failed for meeting " << config_.meeting_number << ": " << err << std::endl;
        } else {
            std::cout << "SDK cleaned up successfully for meeting: " << config_.meeting_number << std::endl;
        }
    }
};

std::vector<std::unique_ptr<ZoomMeeting>> meetings;
std::mutex meetings_mutex;

std::string getSelfDirPath() {
    std::cout << "Getting self directory path" << std::endl;
    char dest[PATH_MAX];
    memset(dest, 0, sizeof(dest));
    if (readlink("/proc/self/exe", dest, PATH_MAX) == -1) {
        std::cerr << "Failed to get executable path" << std::endl;
        return "";
    }
    char* tmp = strrchr(dest, '/');
    if (tmp) *tmp = 0;
    std::cout << "Self directory path: " << dest << std::endl;
    return std::string(dest);
}

void ReadTEXTSettings(std::vector<MeetingConfig>& configs) {
    std::string self_dir = getSelfDirPath() + "/config.txt";
    std::cout << "Reading config from: " << self_dir << std::endl;
    std::ifstream configFile(self_dir);
    if (!configFile) {
        std::cerr << "Error opening config file at " << self_dir << std::endl;
        return;
    }
    std::cout << "Config file opened successfully: " << self_dir << std::endl;

    std::map<std::string, std::string> configMap;
    std::string line;
    while (std::getline(configFile, line)) {
        std::cout << "Read line: " << line << std::endl;
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
            std::cout << "Parsed config: " << key << " = " << value << std::endl;
        }
    }

    int meetingIdx = 1;
    while (true) {
        std::string prefix = std::to_string(meetingIdx);
        if (configMap.find("meeting_number_" + prefix) == configMap.end()) {
            std::cout << "No more meetings found at index: " << meetingIdx << std::endl;
            break;
        }

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
        std::cout << "Loaded meeting " << meetingIdx << ": " << config.meeting_number 
                  << " (Token: " << config.token << ", Password: " << config.meeting_password << ")" << std::endl;
        meetingIdx++;
    }
    std::cout << "Total meetings loaded: " << configs.size() << std::endl;
}

void my_handler(int s) {
    std::cout << "Caught signal " << s << ", cleaning up...\n";
    std::lock_guard<std::mutex> lock(meetings_mutex);
    meetings.clear();
    std::cout << "Meetings cleared, exiting..." << std::endl;
    std::exit(0);
}

int main(int argc, char* argv[]) {
    std::cout << "Starting meetingSDKDemo..." << std::endl;
    std::vector<MeetingConfig> configs;
    ReadTEXTSettings(configs);

    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
    std::cout << "Signal handler set for SIGINT" << std::endl;

    std::cout << "Starting " << configs.size() << " meetings..." << std::endl;
    {
        std::lock_guard<std::mutex> lock(meetings_mutex);
        for (const auto& config : configs) {
            meetings.push_back(std::make_unique<ZoomMeeting>(config));
            meetings.back()->Start();
            std::cout << "Started meeting: " << config.meeting_number << std::endl;
        }
    }

    std::cout << "Entering main loop..." << std::endl;
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "Main loop tick..." << std::endl;
    }

    std::cout << "Exiting main (should not reach here without signal)" << std::endl;
    return 0;
}