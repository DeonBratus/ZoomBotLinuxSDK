#include "MeetingAudioCtrlEventListener.h"
#include <iostream>

MeetingAudioCtrlEventListener::MeetingAudioCtrlEventListener(void(*audioactive_handler)(IList<unsigned int>* active_users)) 
    : audioactive_handler_(audioactive_handler) {}

void MeetingAudioCtrlEventListener::onUserAudioStatusChange(IList<IUserAudioStatus*>* user_list, const zchar_t* status) {
    std::cout << "status_changed" << std::endl;
}

void MeetingAudioCtrlEventListener::onUserActiveAudioChange(IList<unsigned int>* plstActiveAudio) {
    if(audioactive_handler_) {
        audioactive_handler_(plstActiveAudio);
    }
}