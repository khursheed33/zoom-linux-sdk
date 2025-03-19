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
    MeetingParticipantsCtrlEventListener(std::function<void()> hostCb, std::function<void()> coHostCb);
    virtual void onUserJoin(ZOOMSDK::IList<unsigned int>* pUserList);
    virtual void onUserLeft(ZOOMSDK::IList<unsigned int>* pUserList);
    virtual void onHostChangeNotification(unsigned int userId);
    virtual void onCoHostChangeNotification(unsigned int userId, bool isCoHost);
    virtual void onLowOrRaiseHandStatusChanged(bool bLow, unsigned int userid);
    virtual void onUserNameChanged(ZOOMSDK::IList<unsigned int>* pUserList);
};