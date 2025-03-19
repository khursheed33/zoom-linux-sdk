// MeetingParticipantsCtrlEventListener.h
#pragma once
#include "meeting_service_components/meeting_participants_ctrl_interface.h"
#include "meeting_service_components/meeting_audio_interface.h"
#include <functional>
#include <iostream>

class MeetingParticipantsCtrlEventListener : public ZOOMSDK::IMeetingParticipantsCtrlEvent {
private:
    std::function<void()> onIsHost_;
    std::function<void()> onIsCoHost_;

public:
    MeetingParticipantsCtrlEventListener(std::function<void()> hostCb, std::function<void()> coHostCb)
        : onIsHost_(hostCb), onIsCoHost_(coHostCb) {}

    virtual void onUserJoin(ZOOMSDK::IList<unsigned int>* lstUserID, const zchar_t* strUserList = NULL) {
        std::cout << "User joined event" << std::endl;
    }

    virtual void onUserLeft(ZOOMSDK::IList<unsigned int>* lstUserID, const zchar_t* strUserList = NULL) {
        std::cout << "User left event" << std::endl;
    }

    virtual void onHostChangeNotification(unsigned int userId) {
        std::cout << "onHostChangeNotification: " << userId << std::endl;
        if (onIsHost_) onIsHost_();
    }

    virtual void onCoHostChangeNotification(unsigned int userId, bool isCoHost) {
        std::cout << "onCoHostChangeNotification: " << userId << ", isCoHost: " << isCoHost << std::endl;
        if (isCoHost && onIsCoHost_) onIsCoHost_();
    }

    virtual void onLowOrRaiseHandStatusChanged(bool bLow, unsigned int userid) {
        std::cout << "Hand status changed for user " << userid << ": " << (bLow ? "low" : "raised") << std::endl;
    }

    virtual void onUserNamesChanged(ZOOMSDK::IList<unsigned int>* lstUserID) {
        std::cout << "User names changed event" << std::endl;
    }

    virtual void onInvalidReclaimHostkey() {
        std::cout << "Invalid reclaim host key" << std::endl;
    }

    virtual void onAllHandsLowered() {
        std::cout << "All hands lowered" << std::endl;
    }

    virtual void onLocalRecordingStatusChanged(unsigned int user_id, ZOOMSDK::RecordingStatus status) {
        std::cout << "Local recording status changed for user " << user_id << ": " << static_cast<int>(status) << std::endl;
    }

    virtual void onAllowParticipantsRenameNotification(bool bAllow) {
        std::cout << "Allow participants rename: " << bAllow << std::endl;
    }

    virtual void onAllowParticipantsUnmuteSelfNotification(bool bAllow) {
        std::cout << "Allow participants unmute self: " << bAllow << std::endl;
    }

    virtual void onAllowParticipantsStartVideoNotification(bool bAllow) {
        std::cout << "Allow participants start video: " << bAllow << std::endl;
    }

    virtual void onAllowParticipantsShareWhiteBoardNotification(bool bAllow) {
        std::cout << "Allow participants share whiteboard: " << bAllow << std::endl;
    }

    virtual void onRequestLocalRecordingPrivilegeChanged(ZOOMSDK::LocalRecordingRequestPrivilegeStatus status) {
        std::cout << "Request local recording privilege changed: " << static_cast<int>(status) << std::endl;
    }

    virtual void onAllowParticipantsRequestCloudRecording(bool bAllow) {
        std::cout << "Allow participants request cloud recording: " << bAllow << std::endl;
    }

    virtual void onInMeetingUserAvatarPathUpdated(unsigned int userID) {
        std::cout << "User avatar path updated for user: " << userID << std::endl;
    }

    virtual void onParticipantProfilePictureStatusChange(bool bHidden) {
        std::cout << "Participant profile picture status changed: " << bHidden << std::endl;
    }

    virtual void onFocusModeStateChanged(bool bEnabled) {
        std::cout << "Focus mode state changed: " << bEnabled << std::endl;
    }

    virtual void onFocusModeShareTypeChanged(ZOOMSDK::FocusModeShareType type) {
        std::cout << "Focus mode share type changed: " << static_cast<int>(type) << std::endl;
    }
};