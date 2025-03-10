#include <ZoomBot.h>
#include <rawdata_audio_helper_interface.h>
#include <meeting_service_components/meeting_recording_interface.h>
#include <zoom_rawdata_api.h>
#include <ZoomSDKAudioRawData.h>
#include <ZoomSDKVideoSource.h>
#include <ZoomSDKRenderer.h>
#include <MeetingRecordingCtrlEventListener.h>
#include <MeetingServiceEventListener.h>
#include <MeetingParticipantsCtrlEventListener.h>
#include <MeetingReminderEventListener.h>

#include <iostream>
#include <fstream>
#include <ctime>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include <iomanip>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>

USING_ZOOM_SDK_NAMESPACE

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
// КЛАСС ДЛЯ ОБРАБОТКИ АУДИО СОБЫТИЙ
//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
class MeetingAudioCtrlEventListener : public IMeetingAudioCtrlEvent {
public:
    void(*audioactive_handler_)(IList<unsigned int>* active_users);

    MeetingAudioCtrlEventListener(void(*audioactive_handler)(IList<unsigned int>* active_users)) {
        audioactive_handler_ = audioactive_handler;

    };
    
    void onUserAudioStatusChange(IList<IUserAudioStatus*>*, const zchar_t*) override {
        std::cout << "status_changed" << std::endl;
    }

    void onUserActiveAudioChange(IList<unsigned int>* plstActiveAudio) override {
        if(audioactive_handler_) {
            audioactive_handler_(plstActiveAudio);
        }
    }
    void onHostRequestStartAudio(IRequestStartAudioHandler*) override {}
    void onJoin3rdPartyTelephonyAudio(const zchar_t*) override {}
    void onMuteOnEntryStatusChange(bool) override {}
};

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
// ОСНОВНОЙ КЛАСС БОТА
//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
class BotRecording : public ZoomBot::ZoomBot {
public:
    //►►► Конструктор и инициализация ◄◄◄
    BotRecording() {
        self = this;
    }

    //►►► Управление файлами и директориями ◄◄◄
    //──────────────────────────────────────────────────────────────────────────
    bool directoryExists(const std::string& path) {
        return access(path.c_str(), F_OK) == 0;
    }

    bool createDirectory(const std::string& path) {
        if (!directoryExists(path)) {
            #ifdef _WIN32
            return mkdir(path.c_str()) == 0;
            #else
            return mkdir(path.c_str(), 0777) == 0;
            #endif
        }
        return true;
    }

    //►►► Работа со временем ◄◄◄
    //──────────────────────────────────────────────────────────────────────────
    std::string formatTime(std::time_t t) {
        std::tm tm_buf;
        localtime_r(&t, &tm_buf);
        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm_buf);
        return buffer;
    }

    std::string getCurrentTime() {
        return formatTime(time(nullptr));
    }

    //►►► Система логирования ◄◄◄
    //──────────────────────────────────────────────────────────────────────────
    void logAction(const std::string& action, unsigned int userID, const std::string& username) {
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

    void logUserInfo(unsigned int userID, const std::string& username, const std::string& role) {
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

    //►►► Обработчики событий пользователей ◄◄◄
    //──────────────────────────────────────────────────────────────────────────
    static void UserJoinCheck(unsigned int userid) {
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

    static void UserLeftCheck(unsigned int userid) {
        if (self->current_user_dict.count(userid)) {
            std::cout << "[LEFT] " << self->current_user_dict[userid] << " (ID: " << userid << ")\n";
            self->logAction("LEFT", userid, self->current_user_dict[userid]);
            self->current_user_dict.erase(userid);
        }
    }

    void CheckConnectedUsersFirst() {
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

    //►►► Управление записью ◄◄◄
    //──────────────────────────────────────────────────────────────────────────
    void CheckAndStartRawRecording(bool isAudio, bool isVideo) {
        if (!isAudio) return;
        std::string audioPath = logDir + "/" + "audio.pcm";
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


    static void audioActiveHandler(IList<unsigned int>* audio_users){
        std::cout << self->getCurrentTime();

        for (const auto& [id, name] : self->current_user_dict) {

            for (int i = 0; i < audio_users->GetCount(); i++){
                auto speaking_user = audio_users->GetItem(i);
                if (speaking_user != id){
                    std::cout << ","<< name << ",PASS" << "";

                } 
                
                if (speaking_user == id){
                    std::cout << name << ",ACTIVE" << "";
                }
            }
        }
        std::cout << endl;

    }
    //►►► Системные обработчики ◄◄◄
    //──────────────────────────────────────────────────────────────────────────
    static void onIsHost() {
        std::cout << "[STATUS] Host permissions granted\n";
        self->CheckAndStartRawRecording(true, true);
    }

    static void onIsCoHost() {
        std::cout << "[STATUS] Co-host permissions granted\n";
        self->CheckAndStartRawRecording(true, true);
    }

    static void onIsGivenRecordingPermission() {
        std::cout << "[STATUS] Recording permissions granted\n";
        self->CheckAndStartRawRecording(true, true);
    }

    static void onMeetingJoined(){};
    static void onMeetingEndsQuitApp(){};

    static void onInMeeting() {
        if (self->meeting_service_->GetMeetingStatus() == MEETING_STATUS_INMEETING) {
            std::cout << "[STATUS] Meeting in progress\n";
        }
        self->CheckAndStartRawRecording(true, true);
        self->CheckConnectedUsersFirst();
    }

    //►►► Настройка сервисов ◄◄◄
    //──────────────────────────────────────────────────────────────────────────
    void recojoin() {
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
        audio_controller->SetEvent(BotRecording::audioEventListener);

        ZoomBot::JoinToMeeting();
    }

    //►►► Запуск приложения ◄◄◄
    //──────────────────────────────────────────────────────────────────────────
    void recorun() {
        InitZoomSDK();
        JoinWithAuthToMeeting(OnAuthSuccess);
        initAppSettings();
    }

    static void OnAuthSuccess() { if (self) self->recoCallback(); }
    void recoCallback() { recojoin(); }

private:
    //►►► Члены класса ◄◄◄
    static BotRecording* self;
    std::string logDir;

    // Компоненты Zoom SDK
    IMeetingRecordingController* recored_controller = nullptr;
    IMeetingAudioController* audio_controller = nullptr;
    IMeetingParticipantsController* participants_controller = nullptr;
    IZoomSDKAudioRawDataHelper* audioHelper = nullptr;
    MeetingAudioCtrlEventListener* audioEventListener = nullptr;

    // Управление пользователями
    std::unordered_map<unsigned int, std::string> current_user_dict;
    std::unordered_map<unsigned int, std::string> global_user_dict;
    ZOOMSDK::IList<unsigned int>* uids_list = nullptr;
};

BotRecording* BotRecording::self = nullptr;

//▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄▄
// ТОЧКА ВХОДА
//▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀
int main() {
    BotRecording bot;
    bot.meeting_num_ = "74167515410";
    bot.m_pwd_ = "PhjvZDSIq7U9xlOiz8o7bIaJa4RxMz.1";
    bot.token_ = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzZGtLZXkiOiJvRGNsaWpESlNvQ3Uwdko1MThkOUlBIiwiYXBwS2V5Ijoib0RjbGlqREpTb0N1MHZKNTE4ZDlJQSIsIm1uIjo3MzYxNDM1ODQ5OCwicm9sZSI6MCwiaWF0IjoxNzQxNTU4ODI0LCJleHAiOjE3NDE1NjYwMjQsInRva2VuRXhwIjoxNzQxNTY2MDI0fQ.zSn0Zw2qqhckwi0gMcj53x4QewzhgprRGI2QA7cEBnc";
    bot.bot_name = "RecoBot";
    
    bot.recorun();

    GMainLoop* loop = g_main_loop_new(nullptr, FALSE);
    g_main_loop_run(loop);
    g_main_loop_unref(loop);
    
    return EXIT_SUCCESS;
}