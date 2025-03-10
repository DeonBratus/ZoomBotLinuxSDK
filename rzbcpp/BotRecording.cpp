#include "BotRecording.h"
#include <iostream>

extern BotRecording* BotRecording::self;

BotRecording::BotRecording() {
    self = this;
    audio_source = new ZoomSDKAudioRawData();
}

void BotRecording::CheckAndStartRawRecording(bool isAudio) {
    if (!isAudio) return;

    recored_controller = meeting_service_->GetMeetingRecordingController();
    SDKError err2 = recored_controller->CanStartRawRecording();

    if (err2 == SDKERR_SUCCESS) {
        SDKError err1 = recored_controller->StartRawRecording();
        if (err1 != SDKERR_SUCCESS) {
            std::cerr << "Failed to start raw recording: " << err1 << std::endl;
        } else {
            audioHelper = GetAudioRawdataHelper();
            if (audioHelper) {
                SDKError err = audioHelper->subscribe(audio_source);
                if (err != SDKERR_SUCCESS) {
                    std::cerr << "Audio subscription error: " << err << std::endl;
                }
            }
        }
    } else {
        std::cerr << "Recording permission denied" << std::endl;
    }
}

// Инициализация и подключение
void BotRecording::recojoin() {
    createServices();
    
    meeting_service_->SetEvent(new MeetingServiceEventListener(
        [](){}, 
        [](){}, 
        &BotRecording::onInMeetingStatic
    ));

    participants_controller = meeting_service_->GetMeetingParticipantsController();
    participants_controller->SetEvent(new MeetingParticipantsCtrlEventListener(
        &BotRecording::onIsHost, 
        &BotRecording::onIsCoHost
    ));

    recored_controller = meeting_service_->GetMeetingRecordingController();
    recored_controller->SetEvent(new MeetingRecordingCtrlEventListener(
        &BotRecording::onIsGivenRecordingPermission
    ));

    ZoomBot::JoinToMeeting();
}

// Обработчики событий
void BotRecording::onInMeeting() {
    if (meeting_service_->GetMeetingStatus() == MEETING_STATUS_INMEETING) {
        CheckAndStartRawRecording(true);
    }
}

void BotRecording::onInMeetingStatic() {
    if (self) self->onInMeeting();
}

void BotRecording::onIsHost() {
    if (self) self->CheckAndStartRawRecording(true);
}

void BotRecording::onIsCoHost() {
    if (self) self->CheckAndStartRawRecording(true);
}

void BotRecording::onIsGivenRecordingPermission() {
    if (self) self->CheckAndStartRawRecording(true);
}

// Инициализация SDK и запуск
void BotRecording::recorun() {
    this->InitZoomSDK();
    this->JoinWithAuthToMeeting([](){
        if (BotRecording::self) BotRecording::self->recojoin();
    });
    this->initAppSettings();
}