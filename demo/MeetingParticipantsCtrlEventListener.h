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

    virtual void onLocalRecordingStatusChanged(unsigned int user_id, ZOOMSDK: