#include "BotRecording.h"
#include "MeetingAudioCtrlEventListener.h"
#include <meeting_service_components/meeting_participants_ctrl_interface.h>
#include <meeting_service_components/meeting_recording_interface.h>
#include <zoom_rawdata_api.h>

#include <iostream>
#include <fstream>
#include <ctime>
#include <thread>
#include <functional>
#include <sys/stat.h>
#include <unistd.h>
#include <glib.h>

#include <ZoomSDKAudioRawData.h>
#include <MeetingServiceEventListener.h>
#include <MeetingParticipantsCtrlEventListener.h>
#include <MeetingRecordingCtrlEventListener.h>
#include <MeetingReminderEventListener.h>

using namespace ZOOMSDK;

BotRecording* BotRecording::self = nullptr;

// Конструктор/Деструктор
BotRecording::BotRecording()  {
    self = this;
    self->enableReqRecordPermission = false;
    self->isStopRecording = false;
    self->isStartRecording = false;
}


// Методы управления командами
void BotRecording::startCommandListener() {
    std::unordered_map<std::string, std::function<void()>> commands = {
        {"join", [this]() { JoinWithAuthToMeeting(ZoomBot::onAuthSuccess); }},
        {"leave", [this]() { LeaveFromMeeting(); }},
        {"recoallow", [this](){requestRecordingPermissionCrutch();}},
        {"recorun", [this]() { self->CheckAndStartRawRecordingCrutch(); }},
        {"recostop", [this]() { self->recostopCrutch(); }},
        {"exit", [this]() { std::exit(0); }},
    };

    std::thread([commands]() {
        std::string command;
        while (true) {
            std::cout << "Enter command: ";
            std::cin >> command;
            auto it = commands.find(command);
            if (it != commands.end()) {
                it->second();
            } else {
                std::cout << "Unknown command!" << std::endl;
            }
        }
    }).detach();
}

// Работа с файловой системой
bool BotRecording::directoryExists(const std::string& path) {
    return access(path.c_str(), F_OK) == 0;
}

bool BotRecording::createDirectory(const std::string& path) {
    if (!directoryExists(path)) {
#ifdef _WIN32
        return mkdir(path.c_str()) == 0;
#else
        return mkdir(path.c_str(), 0777) == 0;
#endif
    }
    return true;
}

// Форматирование времени
std::string BotRecording::formatTime(std::time_t t) {
    std::tm tm_buf;
    localtime_r(&t, &tm_buf);
    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm_buf);
    return buffer;
}

std::string BotRecording::getCurrentTime() {
    return formatTime(time(nullptr));
}

// Система логирования
void BotRecording::logAction(const std::string& action, unsigned int userID, const std::string& username) {
    logDir = "meeting_" + self->meeting_num_ + "/";

    if (!createDirectory(logDir)) {
        std::cerr << "[ERROR] Failed to create directory: " << logDir << std::endl;
        return;
    }

    const std::string logPath = logDir + "action_logs.txt";
    if (std::ofstream file{logPath, std::ios::app}) {
        file << action << "," << userID << "," << username << "," << getCurrentTime() << "\n";
    } else {
        std::cerr << "[ERROR] Failed to open log file: " << logPath << std::endl;
    }
}

void BotRecording::logUserInfo(unsigned int userID, const std::string& username, const std::string& role) {
    logDir = "meeting_" + self->meeting_num_ + "/";

    if (!createDirectory(logDir)) {
        std::cerr << "[ERROR] Failed to create directory: " << logDir << std::endl;
        return;
    }

    const std::string logPath = logDir + "user_info.txt";
    if (std::ofstream file{logPath, std::ios::app}) {
        file << userID << "," << username << "," << getCurrentTime() << "," << role << "\n";
    } else {
        std::cerr << "[ERROR] Failed to open user info file: " << logPath << std::endl;
    }
}

// Обработчики событий пользователей
void BotRecording::UserJoinCheck(unsigned int userid) {
    self->uids_list = self->participants_controller->GetParticipantsList();
    if (IUserInfo* user = self->participants_controller->GetUserByUserID(userid)) {
        self->current_user_dict[userid] = user->GetUserName();
        self->global_user_dict[userid] = user->GetUserName();

        std::cout << "[JOIN] " << user->GetUserName() << " (ID: " << userid << ")\n";
        self->logAction("JOIN", userid, user->GetUserName());
        self->logUserInfo(userid, user->GetUserName(), "participant");

        std::cout << "Current participants:\n";
        for (const auto& [id, name] : self->current_user_dict) {
            std::cout << " - " << name << "\n";
        }
    }
}

void BotRecording::UserLeftCheck(unsigned int userid) {
    if (self->current_user_dict.count(userid)) {
        std::cout << "[LEFT] " << self->current_user_dict[userid] << " (ID: " << userid << ")\n";
        self->logAction("LEFT", userid, self->current_user_dict[userid]);
        self->current_user_dict.erase(userid);
    }
}

void BotRecording::CheckConnectedUsersFirst() {
    uids_list = participants_controller->GetParticipantsList();
    for (int i = 0; i < uids_list->GetCount(); ++i) {
        if (IUserInfo* user = participants_controller->GetUserByUserID(uids_list->GetItem(i))) {
            current_user_dict[user->GetUserID()] = user->GetUserName();
            std::cout << "[EXISTING] " << user->GetUserName() << " (ID: " << user->GetUserID() << ")\n";
            logAction("WAS", user->GetUserID(), user->GetUserName());
            logUserInfo(user->GetUserID(), user->GetUserName(), "participant");
        }
    }
}

// Управление записью
void BotRecording::CheckAndStartRawRecording(bool isAudio, bool isVideo) {
    if (!isAudio) return;
    std::string startRecordigPath = logDir + "/recrdLog.txt";
    std::ofstream file(startRecordigPath, std::ios::app);
    const std::string timestamp = self->getCurrentTime();
    file << "START" << "," << timestamp << "\n";

    std::string audioPath = logDir + "/audio.pcm";
    ZoomSDKAudioRawData* audio_source = new ZoomSDKAudioRawData(audioPath);
    
    recored_controller = meeting_service_->GetMeetingRecordingController();
    if (recored_controller->CanStartRawRecording() != SDKERR_SUCCESS) {
        std::cerr << "[ERROR] No recording permissions\n";
        return;
    }

    if (recored_controller->StartRawRecording() != SDKERR_SUCCESS) {
        std::cerr << "[ERROR] Failed to start recording\n";
        return;
    }

    if (audioHelper = GetAudioRawdataHelper()) {
        if (audioHelper->subscribe(audio_source) != SDKERR_SUCCESS) {
            std::cerr << "[ERROR] Audio subscription failed\n";
        }
    } else {
        std::cerr << "[ERROR] Audio helper not available\n";
    }
}

void BotRecording::CheckAndStartRawRecordingCrutch(){
    self->isStartRecording = true;
}

void BotRecording::recostop() {
    if (recored_controller) {
        SDKError err = recored_controller->StopRawRecording();
        if (err == SDKERR_SUCCESS) {
            std::cout << "Recording stopped successfully." << std::endl;
        } else {
            std::cerr << "[ERROR] Failed to stop recording: " << err << std::endl;
        }

        if (audioHelper) {
            SDKError unsubErr = audioHelper->unSubscribe();
            if (unsubErr != SDKERR_SUCCESS) {
                std::cerr << "[ERROR] Failed to unsubscribe audio: " << unsubErr << std::endl;
            } else {
                std::cout << "Audio unsubscribed successfully." << std::endl;
            }
        }
    }
}

void BotRecording::recostopCrutch(){
    self->isStopRecording = true;
}

void BotRecording::audioActiveHandler(IList<unsigned int>* audio_users) {

    const std::string timestamp = self->getCurrentTime();
    for (const auto& [id, name] : self->current_user_dict) {
        std::string filename = self->logDir + "/user_audio_" + std::to_string(id) + ".csv";
        std::ofstream file(filename, std::ios::app);
        if (file) {
            for (int i = 0; i < audio_users->GetCount(); i++) {
                if (audio_users->GetItem(i) == id) {
                    file << "A" << "," << timestamp << "\n";
                    break;
                }
            }
        }
    }
    
    if (self->enableReqRecordPermission){
        self->requestRecordingPermission();
    }

    if (self->isStopRecording){
        self->recostop();
    }

    if (self->isStartRecording){
        self->CheckAndStartRawRecording(true,true);
    }
    self->isStopRecording = false;
    self->enableReqRecordPermission = false;
    self->isStartRecording = false;

}


void BotRecording::requestRecordingPermission() {
    if (self->recored_controller) {
        SDKError err = self->recored_controller->RequestLocalRecordingPrivilege();
        if (err != SDKERR_SUCCESS) {
            std::cerr << "[ERROR] Failed to request recording permission: " << err << std::endl;
        }
    }
}

void BotRecording::requestRecordingPermissionCrutch() {
    self->enableReqRecordPermission = true;
}

// Системные обработчики
void BotRecording::onIsHost() {
    std::cout << "[STATUS] Host permissions granted\n";
    self->CheckAndStartRawRecording(true, true);
}

void BotRecording::onIsCoHost() {
    std::cout << "[STATUS] Co-host permissions granted\n";
    self->CheckAndStartRawRecording(true, true);
}

void BotRecording::onIsGivenRecordingPermission() {
    std::cout << "[STATUS] Recording permissions granted\n";
    self->CheckAndStartRawRecording(true, true);
}

void BotRecording::onMeetingJoined() {}
void BotRecording::onMeetingEndsQuitApp() {}

void BotRecording::onInMeeting() {
    if (self->meeting_service_->GetMeetingStatus() == MEETING_STATUS_INMEETING) {
        self->requestRecordingPermission();
    }
    self->CheckAndStartRawRecording(true, true);
    self->CheckConnectedUsersFirst();
}

// Настройка сервисов
void BotRecording::recojoin() {
    createServices();

    meeting_service_->SetEvent(new MeetingServiceEventListener(
        BotRecording::onMeetingJoined,
        BotRecording::onMeetingEndsQuitApp,
        BotRecording::onInMeeting
    ));

    participants_controller = meeting_service_->GetMeetingParticipantsController();
    participants_controller->SetEvent(new MeetingParticipantsCtrlEventListener(
        BotRecording::onIsHost,
        BotRecording::onIsCoHost,
        BotRecording::UserJoinCheck,
        BotRecording::UserLeftCheck
    ));

    recored_controller = meeting_service_->GetMeetingRecordingController();
    recored_controller->SetEvent(new MeetingRecordingCtrlEventListener(
        BotRecording::onIsGivenRecordingPermission
    ));

    if (auto* reminder = meeting_service_->GetMeetingReminderController()) {
        reminder->SetEvent(new MeetingReminderEventListener());
    }

    audioEventListener = new MeetingAudioCtrlEventListener(audioActiveHandler);
    audio_controller = meeting_service_->GetMeetingAudioController();
    audio_controller->SetEvent(audioEventListener);

    ZoomBot::JoinToMeeting();
}

void BotRecording::JoinForPython(){
    self->JoinWithAuthToMeeting(self->OnAuthSuccess);
}

// Запуск приложения
void BotRecording::recorun() {
    InitZoomSDK();
    JoinWithAuthToMeeting(OnAuthSuccess);
    initAppSettings();
    startCommandListener();
}

void BotRecording::OnAuthSuccess() { 
    if (self) self->recoCallback(); 
}

void BotRecording::recoCallback() { 
    recojoin(); 
}