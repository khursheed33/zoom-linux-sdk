#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <mutex>
#include <thread>
#include <glib.h>
#include <signal.h>
#include <atomic>

#include "zoom_sdk.h"
#include "auth_service_interface.h"
#include "meeting_service_interface.h"
#include "meeting_service_components/meeting_audio_interface.h"
#include "meeting_service_components/meeting_participants_ctrl_interface.h"
#include "meeting_service_components/meeting_recording_interface.h"
#include "rawdata/zoom_rawdata_api.h"

#include "AuthServiceEventListener.h"
#include "MeetingParticipantsCtrlEventListener.h"
#include "MeetingRecordingCtrlEventListener.h"
#include "ZoomSDKRenderer.h"
#include "ZoomSDKAudioRawData.h"
#include "ZoomSDKVideoSource.h"
#include "ZoomSDKVirtualAudioMicEvent.h"

USING_ZOOM_SDK_NAMESPACE

struct MeetingConfig {
    std::string meeting_number;
    std::string token;
    std::string password;
    std::string recording_token;
    bool getVideoRawData = false;
    bool getAudioRawData = false;
    bool sendVideoRawData = false;
    bool sendAudioRawData = false;
};

std::mutex meetings_mutex;
std::vector<std::unique_ptr<class ZoomMeeting>> meetings;
std::atomic<bool> exiting(false);

void my_handler(int s) {
    std::cout << "Caught signal " << s << ", cleaning up...\n";
    std::lock_guard<std::mutex> lock(meetings_mutex);
    meetings.clear();
    std::cout << "Meetings cleared, exiting..." << std::endl;
    exiting = true;
    std::exit(0);
}

class ZoomMeeting {
private:
    MeetingConfig config_;
    IAuthService* m_pAuthService = nullptr;
    IMeetingService* m_pMeetingService = nullptr;
    IMeetingParticipantsController* m_pParticipantsController = nullptr;
    IMeetingRecordingController* m_pRecordingController = nullptr;
    ZoomSDKAudioRawData* audio_source = nullptr;
    IZoomSDKAudioRawDataHelper* audioHelper = nullptr;
    GMainLoop* loop_ = nullptr;

    void onIsHost() {
        std::cout << "Became host for meeting: " << config_.meeting_number << std::endl;
    }

    void onIsCoHost() {
        std::cout << "Became co-host for meeting: " << config_.meeting_number << std::endl;
    }

    void OnAuthenticationComplete() {
        std::cout << "Authentication completed for meeting: " << config_.meeting_number << std::endl;
        JoinMeeting();
    }

public:
    ZoomMeeting(const MeetingConfig& config) : config_(config) {
        audio_source = new ZoomSDKAudioRawData((config_.meeting_number + "_raw_audio_sdk.pcm").c_str());
    }

    ~ZoomMeeting() {
        CleanSDK();
        if (audio_source) {
            delete audio_source;
            audio_source = nullptr;
        }
    }

    void AuthMeetingSDK() {
        InitParam initParam;
        initParam.strWebDomain = "https://zoom.us"; // Default domain
        SDKError err = ZOOMSDK::InitSDK(initParam); // Added InitParam
        if (err != SDKERR_SUCCESS) {
            std::cerr << "Failed to initialize SDK for meeting " << config_.meeting_number << ": " << static_cast<int>(err) << std::endl;
            return;
        }

        m_pAuthService = ZOOMSDK::GetAuthService(); // Fully qualified
        if (!m_pAuthService) {
            std::cerr << "Failed to get auth service for meeting " << config_.meeting_number << std::endl;
            return;
        }

        m_pAuthService->SetEvent(new AuthServiceEventListener([this]() { OnAuthenticationComplete(); }));

        AuthContext authContext;
        authContext.jwt_token = config_.token.c_str();
        err = m_pAuthService->SDKAuth(authContext);
        if (err != SDKERR_SUCCESS) {
            std::cerr << "SDKAuth failed for meeting " << config_.meeting_number << ": " << static_cast<int>(err) << std::endl;
            return;
        }

        std::cout << "SDKAuth initiated successfully for meeting: " << config_.meeting_number << std::endl;

        std::thread([this]() {
            std::cout << "Thread started for meeting: " << config_.meeting_number << std::endl;
            StartMeeting();
            std::cout << "Thread ended for meeting: " << config_.meeting_number << std::endl;
        }).detach();
    }

    void JoinMeeting() {
        std::cout << "Attempting to join meeting: " << config_.meeting_number << std::endl;

        m_pMeetingService = ZOOMSDK::GetMeetingService(); // Fully qualified
        if (!m_pMeetingService) {
            std::cerr << "Failed to get meeting service for meeting " << config_.meeting_number << std::endl;
            return;
        }

        JoinParam joinParam;
        JoinParam4WithoutLogin& withoutLoginParam = joinParam.param.withoutloginuserJoin;
        joinParam.userType = SDK_UT_WITHOUT_LOGIN;
        withoutLoginParam.meetingNumber = std::stoull(config_.meeting_number);
        withoutLoginParam.psw = config_.password.c_str();

        SDKError err = m_pMeetingService->Join(joinParam);
        if (err != SDKERR_SUCCESS) {
            std::cerr << "Failed to join meeting " << config_.meeting_number << ": " << static_cast<int>(err) << std::endl;
            return;
        }

        m_pParticipantsController = m_pMeetingService->GetMeetingParticipantsController();
        if (!m_pParticipantsController) {
            std::cerr << "Failed to get participants controller for meeting " << config_.meeting_number << std::endl;
            return;
        }

        m_pParticipantsController->SetEvent(new MeetingParticipantsCtrlEventListener(
            [this]() { onIsHost(); },
            [this]() { onIsCoHost(); }
        ));

        m_pRecordingController = m_pMeetingService->GetMeetingRecordingController();
        if (!m_pRecordingController) {
            std::cerr << "Failed to get recording controller for meeting " << config_.meeting_number << std::endl;
            return;
        }

        m_pRecordingController->SetEvent(new MeetingRecordingCtrlEventListener(
            [this]() { CheckAndStartRawRecording(config_.getVideoRawData, config_.getAudioRawData); }
        ));

        std::cout << "Joined meeting: " << config_.meeting_number << std::endl;
    }

    void StartMeeting() {
        std::cout << "Started meeting: " << config_.meeting_number << std::endl;
        loop_ = g_main_loop_new(NULL, FALSE);
        std::cout << "Entering GMainLoop for meeting: " << config_.meeting_number << std::endl;
        g_main_loop_run(loop_);
        std::cout << "Exited GMainLoop for meeting: " << config_.meeting_number << std::endl;
        g_main_loop_unref(loop_);
    }

    void CheckAndStartRawRecording(bool isVideo, bool isAudio) {
        if (!m_pRecordingController) {
            std::cerr << "Recording controller not initialized for meeting " << config_.meeting_number << std::endl;
            return;
        }

        SDKError err = SDKERR_UNINITIALIZE;
        if (isAudio) {
            err = m_pRecordingController->StartRawRecording();
            if (err == SDKERR_SUCCESS) {
                std::cout << "Raw recording started for meeting: " << config_.meeting_number << std::endl;
                audioHelper = ZOOMSDK::GetAudioRawdataHelper();
                if (!audioHelper) {
                    std::cerr << "Failed to get audio helper for meeting " << config_.meeting_number << std::endl;
                    return;
                }

                audio_source->setAudioCallback([this](const char* data, unsigned int length) {
                    std::cout << "Received audio raw data of length " << length << " for meeting " << config_.meeting_number << std::endl;
                });

                err = audioHelper->subscribe(audio_source);
                if (err != SDKERR_SUCCESS) {
                    std::cerr << "Failed to subscribe to audio raw data for meeting " << config_.meeting_number << ": " << static_cast<int>(err) << std::endl;
                }
            } else {
                std::cerr << "Failed to start raw recording for meeting " << config_.meeting_number << ": " << static_cast<int>(err) << std::endl;
            }
        }
    }

    void CleanSDK() {
        if (audioHelper && audio_source) {
            audioHelper->unSubscribe(); // No argument
            audioHelper = nullptr;
        }

        if (m_pRecordingController) {
            m_pRecordingController->SetEvent(nullptr);
            m_pRecordingController = nullptr;
        }

        if (m_pParticipantsController) {
            m_pParticipantsController->SetEvent(nullptr);
            m_pParticipantsController = nullptr;
        }

        if (m_pMeetingService) {
            m_pMeetingService = nullptr;
        }

        if (m_pAuthService) {
            m_pAuthService->SetEvent(nullptr);
            m_pAuthService = nullptr;
        }

        if (loop_) {
            g_main_loop_quit(loop_);
        }

        SDKError err = ZOOMSDK::CleanupSDK(); // Fully qualified
        if (err != SDKERR_SUCCESS) {
            std::cerr << "Failed to clean up SDK for meeting " << config_.meeting_number << ": " << static_cast<int>(err) << std::endl;
        } else {
            std::cout << "SDK cleaned up for meeting: " << config_.meeting_number << std::endl;
        }
    }
};

std::vector<MeetingConfig> LoadConfig(const std::string& filename) {
    std::vector<MeetingConfig> configs;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " << filename << std::endl;
        return configs;
    }

    std::string line;
    MeetingConfig currentConfig;
    int index = 1;

    while (std::getline(file, line)) {
        if (line.find("meeting_number_" + std::to_string(index) + ":") == 0) {
            if (!currentConfig.meeting_number.empty()) {
                configs.push_back(currentConfig);
                currentConfig = MeetingConfig();
            }
            currentConfig.meeting_number = line.substr(line.find(":") + 1);
        } else if (line.find("token_" + std::to_string(index) + ":") == 0) {
            currentConfig.token = line.substr(line.find(":") + 1);
        } else if (line.find("meeting_password_" + std::to_string(index) + ":") == 0) {
            currentConfig.password = line.substr(line.find(":") + 1);
        } else if (line.find("recording_token_" + std::to_string(index) + ":") == 0) {
            currentConfig.recording_token = line.substr(line.find(":") + 1);
        } else if (line.find("GetVideoRawData_" + std::to_string(index) + ":") == 0) {
            currentConfig.getVideoRawData = (line.substr(line.find(":") + 1) == "true");
        } else if (line.find("GetAudioRawData_" + std::to_string(index) + ":") == 0) {
            currentConfig.getAudioRawData = (line.substr(line.find(":") + 1) == "true");
        } else if (line.find("SendVideoRawData_" + std::to_string(index) + ":") == 0) {
            currentConfig.sendVideoRawData = (line.substr(line.find(":") + 1) == "true");
        } else if (line.find("SendAudioRawData_" + std::to_string(index) + ":") == 0) {
            currentConfig.sendAudioRawData = (line.substr(line.find(":") + 1) == "true");
            index++;
        }
    }

    if (!currentConfig.meeting_number.empty()) {
        configs.push_back(currentConfig);
    }

    file.close();
    return configs;
}

int main(int argc, char* argv[]) {
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);

    std::vector<MeetingConfig> configs = LoadConfig("config.txt");
    if (configs.empty()) {
        std::cerr << "No valid configurations found in config.txt" << std::endl;
        return 1;
    }

    {
        std::lock_guard<std::mutex> lock(meetings_mutex);
        for (const auto& config : configs) {
            meetings.push_back(std::make_unique<ZoomMeeting>(config));
        }
    }

    std::cout << "Starting " << configs.size() << " meetings..." << std::endl;

    {
        std::lock_guard<std::mutex> lock(meetings_mutex);
        for (auto& meeting : meetings) {
            meeting->AuthMeetingSDK();
        }
    }

    std::cout << "Entering main loop..." << std::endl;
    while (!exiting) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << "Exiting main loop..." << std::endl;
    return 0;
}