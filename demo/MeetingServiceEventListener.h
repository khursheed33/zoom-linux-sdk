// MeetingServiceEventListener.h
#pragma once
#include <functional>
#include "meeting_service_interface.h"

class MeetingServiceEventListener : public ZOOMSDK::IMeetingServiceEvent {
private:
    std::function<void()> onMeetingStarts_;
    std::function<void()> onMeetingEnds_;
    std::function<void()> onInMeeting_;

public:
    MeetingServiceEventListener(std::function<void()> onMeetingStarts, 
                                std::function<void()> onMeetingEnds, 
                                std::function<void()> onInMeeting);

    virtual void onMeetingStatusChanged(ZOOMSDK::MeetingStatus status, int iResult = 0); // Declaration only
    virtual void onMeetingStatisticsWarningNotification(ZOOMSDK::StatisticsWarningType type) {}
    virtual void onMeetingParameterNotification(const MeetingParameter* meeting_param) {}
    virtual void onSuspendParticipantsActivities() {}
    virtual void onAICompanionActiveChangeNotice(bool bActive) {}
    virtual void onMeetingTopicChanged(const zchar_t* sTopic) {}
};