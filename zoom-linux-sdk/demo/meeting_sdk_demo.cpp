#include <limits.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <glib.h>
#include <sstream>
#include <thread>
#include <sys/syscall.h>
#include <fstream>
#include <iosfwd>
#include <iostream>
#include <map>
#include <algorithm>
#include <vector>
#include <chrono>

#include "zoom_sdk.h"
#include "auth_service_interface.h"
#include "meeting_service_interface.h"
#include "meeting_service_components/meeting_audio_interface.h"
#include "meeting_service_components/meeting_participants_ctrl_interface.h"
#include "meeting_service_components/meeting_video_interface.h"
#include "setting_service_interface.h"

// Event listeners and helpers
#include "MeetingReminderEventListener.h"
#include "MeetingServiceEventListener.h"
#include "AuthServiceEventListener.h"
#include "NetworkConnectionHandler.h"
#include "MeetingParticipantsCtrlEventListener.h"
#include "MeetingRecordingCtrlEventListener.h"

// Raw data handling
#include "ZoomSDKRenderer.h"
#include "rawdata/rawdata_renderer_interface.h"
#include "rawdata/zoom_rawdata_api.h"
#include "ZoomSDKAudioRawData.h"
#include "meeting_service_components/meeting_recording_interface.h"
#include "ZoomSDKVideoSource.h"
#include "ZoomSDKVirtualAudioMicEvent.h"

// JWT generation
#include "GenerateSDKJwtToken.h"

#include <mutex>

USING_ZOOM_SDK_NAMESPACE

// Global variables
std::string DEFAULT_AUDIO_SOURCE = "audio.wav";
std::string DEFAULT_VIDEO_SOURCE = "video.mp4";
GMainLoop* loop;

// Meeting credentials
std::string meeting_number, meeting_password, recording_token, sdk_key, sdk_secret;

// SDK services
IAuthService* m_pAuthService;
IMeetingService* m_pMeetingService;
ISettingService* m_pSettingService;
INetworkConnectionHelper* network_connection_helper;

// Raw data helpers
ZoomSDKRenderer* videoSource = new ZoomSDKRenderer();
IZoomSDKRenderer* videoHelper;
IMeetingRecordingController* m_pRecordController;
IMeetingParticipantsController* m_pParticipantsController;
ZoomSDKAudioRawData* audio_source = new ZoomSDKAudioRawData();
IZoomSDKAudioRawDataHelper* audioHelper;

// Flags for raw data handling
bool GetVideoRawData = true;
bool GetAudioRawData = true;
bool SendVideoRawData = false;
bool SendAudioRawData = false;

// Helper methods
uint32_t getUserID() {
    m_pParticipantsController = m_pMeetingService->GetMeetingParticipantsController();
    int returnvalue = m_pParticipantsController->GetParticipantsList()->GetItem(0);
    std::cout << "UserID is : " << returnvalue << std::endl;
    return returnvalue;
}

IUserInfo* getMyself() {
    m_pParticipantsController = m_pMeetingService->GetMeetingParticipantsController();
    return m_pParticipantsController->GetMySelfUser();
}

IUserInfo* getUserObj() {
    m_pParticipantsController = m_pMeetingService->GetMeetingParticipantsController();
    int userID = m_pParticipantsController->GetParticipantsList()->GetItem(0);
    IUserInfo* returnvalue = m_pParticipantsController->GetUserByUserID(userID);
    std::cout << "UserID is : " << returnvalue << std::endl;
    return returnvalue;
}

// Raw data recording and sending logic
void CheckAndStartRawRecording(bool isVideo, bool isAudio) {
    if (isVideo || isAudio) {
        m_pRecordController = m_pMeetingService->GetMeetingRecordingController();
        SDKError err2 = m_pRecordController->CanStartRawRecording();
        if (err2 == SDKERR_SUCCESS) {
            SDKError err1 = m_pRecordController->StartRawRecording();
            if (err1 != SDKERR_SUCCESS) {
                std::cout << "Error occurred starting raw recording" << std::endl;
            } else {
                if (isVideo) {
                    SDKError err = createRenderer(&videoHelper, videoSource);
                    if (err != SDKERR_SUCCESS) {
                        std::cout << "Error occurred" << std::endl;
                    } else {
                        std::cout << "attemptToStartRawRecording : subscribing" << std::endl;
                        videoHelper->setRawDataResolution(ZoomSDKResolution_720P);
                        videoHelper->subscribe(getUserID(), RAW_DATA_TYPE_VIDEO);
                    }
                }
                if (isAudio) {
                    audioHelper = GetAudioRawdataHelper();
                    if (audioHelper) {
                        SDKError err = audioHelper->subscribe(audio_source);
                        if (err != SDKERR_SUCCESS) {
                            std::cout << "Error occurred subscribing to audio : " << err << std::endl;
                        }
                    } else {
                        std::cout << "Error getting audioHelper" << std::endl;
                    }
                }
            }
        } else {
            std::cout << "Cannot start raw recording: no permissions yet" << std::endl;
        }
    }
}

void CheckAndStartRawSending(bool isVideo, bool isAudio) {
    if (isVideo) {
        ZoomSDKVideoSource* virtual_camera_video_source = new ZoomSDKVideoSource(DEFAULT_VIDEO_SOURCE);
        IZoomSDKVideoSourceHelper* p_videoSourceHelper = GetRawdataVideoSourceHelper();
        if (p_videoSourceHelper) {
            SDKError err = p_videoSourceHelper->setExternalVideoSource(virtual_camera_video_source);
            if (err != SDKERR_SUCCESS) {
                printf("Failed to set external video source, error code: %d\n", err);
            } else {
                printf("Success setting external video source\n");
                IMeetingVideoController* meetingController = m_pMeetingService->GetMeetingVideoController();
                meetingController->UnmuteVideo();
            }
        } else {
            printf("Failed to get video source helper\n");
        }
    }
    if (isAudio) {
        ZoomSDKVirtualAudioMicEvent* audio_source = new ZoomSDKVirtualAudioMicEvent(DEFAULT_AUDIO_SOURCE);
        IZoomSDKAudioRawDataHelper* audioHelper = GetAudioRawdataHelper();
        if (audioHelper) {
            SDKError err = audioHelper->setExternalAudioSource(audio_source);
        }
    }
}

// Callbacks
void onIsHost() {
    printf("Is host now...\n");
    CheckAndStartRawRecording(GetVideoRawData, GetAudioRawData);
}

void onIsCoHost() {
    printf("Is co-host now...\n");
    CheckAndStartRawRecording(GetVideoRawData, GetAudioRawData);
}

void onIsGivenRecordingPermission() {
    printf("Is given recording permissions now...\n");
    CheckAndStartRawRecording(GetVideoRawData, GetAudioRawData);
}

void onInMeeting() {
    printf("onInMeeting Invoked\n");
    if (m_pMeetingService->GetMeetingStatus() == MEETING_STATUS_INMEETING) {
        printf("In Meeting Now...\n");
        IList<unsigned int>* participants = m_pMeetingService->GetMeetingParticipantsController()->GetParticipantsList();
        printf("Participants count: %d\n", participants->GetCount());
    }
    CheckAndStartRawRecording(GetVideoRawData, GetAudioRawData);
    CheckAndStartRawSending(SendVideoRawData, SendAudioRawData);
}

void onMeetingEndsQuitApp() {
//     CleanSDK();
//     std::exit(0);
}

void onMeetingJoined() {
    printf("Joining Meeting...\n");
}

// Read settings from environment variables
void ReadEnvSettings() {
    const char* env_meeting_number = std::getenv("MEETING_ID");
    const char* env_meeting_password = std::getenv("MEETING_PASSWORD");
    const char* env_sdk_key = std::getenv("ZOOM_SDK_KEY");
    const char* env_sdk_secret = std::getenv("ZOOM_SDK_SECRET");
    const char* env_recording_token = std::getenv("RECORDING_TOKEN");

    meeting_number = env_meeting_number ? env_meeting_number : "";
    meeting_password = env_meeting_password ? env_meeting_password : "";
    sdk_key = env_sdk_key ? env_sdk_key : "";
    sdk_secret = env_sdk_secret ? env_sdk_secret : "";
    recording_token = env_recording_token ? env_recording_token : "";

    std::cout << "Meeting Number: " << meeting_number << std::endl;
    std::cout << "Meeting Password: " << meeting_password << std::endl;
    std::cout << "SDK Key: " << sdk_key << std::endl;
    std::cout << "SDK Secret: " << sdk_secret << std::endl;
    std::cout << "Recording Token: " << recording_token << std::endl;
}

// Cleanup
void CleanSDK() {
    SDKError err(SDKERR_SUCCESS);
    if (m_pAuthService) {
        DestroyAuthService(m_pAuthService);
        m_pAuthService = NULL;
    }
    if (m_pSettingService) {
        DestroySettingService(m_pSettingService);
        m_pSettingService = NULL;
    }
    if (m_pMeetingService) {
        DestroyMeetingService(m_pMeetingService);
        m_pMeetingService = NULL;
    }
    if (videoHelper) {
        videoHelper->unSubscribe();
    }
    if (audioHelper) {
        audioHelper->unSubscribe();
    }
    err = CleanUPSDK();
    if (err != SDKERR_SUCCESS) {
        std::cerr << "CleanSDK meetingSdk:error " << std::endl;
    } else {
        std::cerr << "CleanSDK meetingSdk:success" << std::endl;
    }
}

// Join meeting
void JoinMeeting() {
    std::cerr << "Joining Meeting" << std::endl;
    SDKError err2(SDKERR_SUCCESS);

    if ((err2 = CreateMeetingService(&m_pMeetingService)) != SDKERR_SUCCESS) {}
    std::cerr << "MeetingService created." << std::endl;

    CreateSettingService(&m_pSettingService);
    std::cerr << "Settingservice created." << std::endl;

    m_pMeetingService->SetEvent(new MeetingServiceEventListener(&onMeetingJoined, &onMeetingEndsQuitApp, &onInMeeting));
    m_pParticipantsController = m_pMeetingService->GetMeetingParticipantsController();
    m_pParticipantsController->SetEvent(new MeetingParticipantsCtrlEventListener(&onIsHost, &onIsCoHost));
    m_pRecordController = m_pMeetingService->GetMeetingRecordingController();
    m_pRecordController->SetEvent(new MeetingRecordingCtrlEventListener(&onIsGivenRecordingPermission));

    IMeetingReminderController* meetingremindercontroller = m_pMeetingService->GetMeetingReminderController();
    MeetingReminderEventListener* meetingremindereventlistener = new MeetingReminderEventListener();
    meetingremindercontroller->SetEvent(meetingremindereventlistener);

    JoinParam joinParam;
    joinParam.userType = SDK_UT_WITHOUT_LOGIN;
    JoinParam4WithoutLogin& withoutloginParam = joinParam.param.withoutloginuserJoin;
    withoutloginParam.meetingNumber = std::stoull(meeting_number);
    withoutloginParam.vanityID = NULL;
    withoutloginParam.userName = "LinuxChun";
    withoutloginParam.psw = meeting_password.c_str();
    withoutloginParam.customer_key = NULL;
    withoutloginParam.webinarToken = NULL;
    withoutloginParam.isVideoOff = false;
    withoutloginParam.isAudioOff = false;
    withoutloginParam.app_privilege_token = recording_token.empty() ? NULL : recording_token.c_str();

    if (GetAudioRawData) {
        IAudioSettingContext* pAudioContext = m_pSettingService->GetAudioSettings();
        if (pAudioContext) {
            pAudioContext->EnableAutoJoinAudio(true);
        }
    }
    if (SendVideoRawData) {
        withoutloginParam.isVideoOff = false;
        IVideoSettingContext* pVideoContext = m_pSettingService->GetVideoSettings();
        if (pVideoContext) {
            pVideoContext->EnableAutoTurnOffVideoWhenJoinMeeting(false);
        }
    }
    if (SendAudioRawData) {
        IAudioSettingContext* pAudioContext = m_pSettingService->GetAudioSettings();
        if (pAudioContext) {
            pAudioContext->EnableAutoJoinAudio(true);
            pAudioContext->EnableAlwaysMuteMicWhenJoinVoip(true);
            pAudioContext->SetSuppressBackgroundNoiseLevel(Suppress_BGNoise_Level_None);
        }
    }

    if (m_pMeetingService) {
        SDKError err = m_pMeetingService->Join(joinParam);
        if (err == SDKERR_SUCCESS) {
            std::cout << "join_meeting:success" << std::endl;
        } else {
            std::cout << "join_meeting:error" << std::endl;
        }
    } else {
        std::cout << "join_meeting m_pMeetingService:Null" << std::endl;
    }
}

// Authentication with generated JWT
void OnAuthenticationComplete() {
    std::cout << "OnAuthenticationComplete" << std::endl;
    JoinMeeting();
}

void AuthMeetingSDK() {
    SDKError err(SDKERR_SUCCESS);
    if ((err = CreateAuthService(&m_pAuthService)) != SDKERR_SUCCESS) {}
    std::cerr << "AuthService created." << std::endl;

    if ((err = m_pAuthService->SetEvent(new AuthServiceEventListener(&OnAuthenticationComplete))) != SDKERR_SUCCESS) {}
    std::cout << "AuthServiceEventListener added." << std::endl;

    // Generate JWT token internally
    std::string jwt_token = generateZoomJWT(sdk_key, sdk_secret, meeting_number, 0); // Role 0 for participant
    std::cout << "Generated JWT Token: " << jwt_token << std::endl;

    AuthContext param;
    param.jwt_token = jwt_token.c_str();
    m_pAuthService->SDKAuth(param);
}

// Initialization
void InitMeetingSDK() {
    SDKError err(SDKERR_SUCCESS);
    InitParam initParam;
    initParam.strWebDomain = "https://zoom.us";
    initParam.strSupportUrl = "https://zoom.us";
    initParam.emLanguageID = LANGUAGE_English;
    initParam.enableLogByDefault = true;
    initParam.enableGenerateDump = true;

    err = InitSDK(initParam);
    if (err != SDKERR_SUCCESS) {
        std::cerr << "Init meetingSdk:error " << std::endl;
    } else {
        std::cerr << "Init meetingSdk:success" << std::endl;
    }
}

// Signal handler
void my_handler(int s) {
    printf("\nCaught signal %d signum:%d\n", s, SIGINT);
    if (m_pMeetingService) {
        m_pMeetingService->Leave(LEAVE_MEETING);
        printf("Leaving session.\n");
    }
    CleanSDK();
    g_main_loop_quit(loop);
    std::exit(0);
}

void initAppSettings() {
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = my_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
}

// Define the timeout callback
gboolean timeout_callback(gpointer data) {
    std::cout << "Timeout callback triggered" << std::endl;
    return TRUE; // Keep the timeout running
}

int main(int argc, char* argv[]) {
    ReadEnvSettings();
    if (sdk_key.empty() || sdk_secret.empty()) {
        std::cerr << "Error: ZOOM_SDK_KEY and ZOOM_SDK_SECRET must be provided as environment variables." << std::endl;
        return 1;
    }
    if (meeting_number.empty() || meeting_password.empty()) {
        std::cerr << "Error: MEETING_ID and MEETING_PASSWORD must be provided as environment variables." << std::endl;
        return 1;
    }

    InitMeetingSDK();
    AuthMeetingSDK();
    initAppSettings();

    loop = g_main_loop_new(NULL, FALSE);
    g_timeout_add(1000, timeout_callback, loop);
    g_main_loop_run(loop);
    return 0;
}