#ifndef MEETING_AUDIO_CTRL_EVENT_LISTENER_H
#define MEETING_AUDIO_CTRL_EVENT_LISTENER_H

#include <meeting_audio_interface.h>

USING_ZOOM_SDK_NAMESPACE

class MeetingAudioCtrlEventListener : public IMeetingAudioCtrlEvent {
public:
    void(*audioactive_handler_)(IList<unsigned int>* active_users);

    explicit MeetingAudioCtrlEventListener(void(*audioactive_handler)(IList<unsigned int>* active_users));
    
    void onUserAudioStatusChange(IList<IUserAudioStatus*>* user_list, const zchar_t* status) override;
    void onUserActiveAudioChange(IList<unsigned int>* plstActiveAudio) override;
    void onHostRequestStartAudio(IRequestStartAudioHandler* handler) override {}
    void onJoin3rdPartyTelephonyAudio(const zchar_t* info) override {}
    void onMuteOnEntryStatusChange(bool status) override {}
};

#endif // MEETING_AUDIO_CTRL_EVENT_LISTENER_H