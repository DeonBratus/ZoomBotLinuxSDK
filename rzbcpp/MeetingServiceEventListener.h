#include "meeting_service_interface.h"
#include "zoom_sdk.h"

USING_ZOOM_SDK_NAMESPACE	

class MeetingServiceEventListener : public ZOOM_SDK_NAMESPACE::IMeetingServiceEvent
{
    void (*onMeetingEnds_)();
    void (*onMeetingStarts_)();
    void (*onInMeeting_)();
public:
    MeetingServiceEventListener(void (*onMeetingStarts)(), void (*onMeetingEnds)(), void (*onInMeeting)());

    /// \brief Meeting status changed callback.
    virtual void onMeetingStatusChanged(MeetingStatus status, int iResult = 0) override;

    /// \brief Meeting statistics warning notification callback.
    virtual void onMeetingStatisticsWarningNotification(StatisticsWarningType type) override;

    /// \brief Meeting parameter notification callback.
    virtual void onMeetingParameterNotification(const MeetingParameter* meeting_param) override;

    /// \brief Callback event when a meeting is suspended.
    virtual void onSuspendParticipantsActivities() override;

    /// \brief Callback event for the AI Companion active status changed. 
    virtual void onAICompanionActiveChangeNotice(bool bActive) override;

    /// \brief Callback event for the meeting topic changed. 
    virtual void onMeetingTopicChanged(const zchar_t* sTopic) override;

    /// \brief Callback event that the meeting users have reached the meeting capacity.
    virtual void onMeetingFullToWatchLiveStream(const zchar_t* sLiveStreamUrl) override;
};