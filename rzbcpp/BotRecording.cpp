#include "BotRecording.h"
#include <iostream>

extern BotRecording* BotRecording::r_instance;

BotRecording::BotRecording() {
    r_instance = this;
    audio_source = new ZoomSDKAudioRawData();
}

void BotRecording::CheckAndStartRawRecording(bool isAudio) {
    if (!isAudio) return;

    m_pRecordController = meeting_service_->GetMeetingRecordingController();
    SDKError err2 = m_pRecordController->CanStartRawRecording();

    if (err2 == SDKERR_SUCCESS) {
        SDKError err1 = m_pRecordController->StartRawRecording();
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

    m_pParticipantsController = meeting_service_->GetMeetingParticipantsController();
    m_pParticipantsController->SetEvent(new MeetingParticipantsCtrlEventListener(
        &BotRecording::onIsHost, 
        &BotRecording::onIsCoHost
    ));

    m_pRecordController = meeting_service_->GetMeetingRecordingController();
    m_pRecordController->SetEvent(new MeetingRecordingCtrlEventListener(
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
    if (r_instance) r_instance->onInMeeting();
}

void BotRecording::onIsHost() {
    if (r_instance) r_instance->CheckAndStartRawRecording(true);
}

void BotRecording::onIsCoHost() {
    if (r_instance) r_instance->CheckAndStartRawRecording(true);
}

void BotRecording::onIsGivenRecordingPermission() {
    if (r_instance) r_instance->CheckAndStartRawRecording(true);
}

// Инициализация SDK и запуск
void BotRecording::recorun() {
    this->InitZoomSDK();
    this->JoinWithAuthToMeeting([](){
        if (BotRecording::r_instance) BotRecording::r_instance->recojoin();
    });
    this->initAppSettings();
}