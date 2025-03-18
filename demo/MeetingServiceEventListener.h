#include "meeting_service_interface.h"
#include "zoom_sdk.h"
#include <functional>

USING_ZOOM_SDK_NAMESPACE

class MeetingServiceEventListener : public IMeetingServiceEvent {
    std::function<void()> onMeetingStarts_;
    std::function<void()> onMeetingEnds_;
    std::function<void()> onInMeeting_;
public:
    MeetingServiceEventListener(std::function<void()> onMeetingStarts, std::function<void()> onMeetingEnds, std::function<void()> onInMeeting)
        : onMeetingStarts_(onMeetingStarts), onMeetingEnds_(onMeetingEnds), onInMeeting_(onInMeeting) {}

    virtual void onMeetingStatusChanged(MeetingStatus status, int iResult = 0) {
        if (status == MEETING_STATUS_INMEETING && onInMeeting_) onInMeeting_();
        if (status == MEETING_STATUS_ENDED && onMeetingEnds_) onMeetingEnds_();
        if (status == MEETING_STATUS_CONNECTING && onMeetingStarts_) onMeetingStarts_();
    }
    virtual void onMeetingStatisticsWarningNotification(StatisticsWarningType type) {}
    virtual void onMeetingParameterNotification(const MeetingParameter* meeting_param) {}
    virtual void onSuspendParticipantsActivities() {}
    virtual void onAICompanionActiveChangeNotice(bool bActive) {}
    virtual void onMeetingTopicChanged(const zchar_t* sTopic) {}
};