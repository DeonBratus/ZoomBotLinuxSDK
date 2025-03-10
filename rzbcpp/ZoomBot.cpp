#include "ZoomBot.h"
#include <AuthServiceEventListener.h>
#include <MeetingServiceEventListener.h>

extern ZoomBot* ZoomBot::s_instance;

ZoomBot::ZoomBot(const std::string& token, const std::string& meetingNum, const std::string& pwd, const std::string botName)
    : token_(token), meeting_num_(meetingNum), m_pwd_(pwd), bot_name(botName) {s_instance = this;}
ZoomBot::ZoomBot() { s_instance = this;}

void ZoomBot::run() {
    SDKError err(SDKERR_SUCCESS);
    InitZoomSDK();
    JoinWithAuthToMeeting(ZoomBot::onAuthSuccess);
    initAppSettings();
}

void ZoomBot::InitZoomSDK() {
    InitParam initParam;
    initParam.strWebDomain = "https://zoom.us";
    initParam.strSupportUrl = "https://zoom.us";
    initParam.emLanguageID = LANGUAGE_English;
    initParam.enableLogByDefault = false;
    initParam.enableGenerateDump = false;

    if (InitSDK(initParam) == SDKERR_SUCCESS) {
        std::cout << "Zoom SDK initialized successfully!" << std::endl;
    }
}


void ZoomBot::createServices(){
    CreateMeetingService(&meeting_service_);
    CreateSettingService(&setting_service_);

}


void ZoomBot::JoinToMeeting() {
    std::cerr << "Joining Meeting" << std::endl;
 
    JoinParam joinParam;
    joinParam.userType = SDK_UT_WITHOUT_LOGIN;
    JoinParam4WithoutLogin& withoutloginParam = joinParam.param.withoutloginuserJoin;

    try {
        withoutloginParam.meetingNumber = std::stoull(meeting_num_);
    } catch (const std::exception& e) {
        std::cerr << "Invalid meeting number: " << e.what() << std::endl;
        return;
    }

    withoutloginParam.userName = bot_name.c_str();
    withoutloginParam.psw = m_pwd_.c_str();
    withoutloginParam.isVideoOff = true;
    withoutloginParam.isAudioOff = false;

    if (meeting_service_) {
        SDKError err = meeting_service_->Join(joinParam);
        if (err == SDKERR_SUCCESS) {
            std::cout << "join_meeting: success" << std::endl;
        } else {
            std::cout << "join_meeting: error " << err << std::endl;
        }
    } else {
        std::cout << "join_meeting: meeting_service is null" << std::endl;
    }
}

void ZoomBot::AuthCallback() {
    std::cout << "AuthCallback" << std::endl;
    createServices();
    JoinToMeeting();
}

void ZoomBot::JoinWithAuthToMeeting(void(*actionCallback)()) {
    SDKError err = CreateAuthService(&auth_service_);
    if (err != SDKERR_SUCCESS || !auth_service_) {
        std::cerr << "Failed to create auth service: " << err << std::endl;
        return;
    }

    AuthContext param;
    param.jwt_token = token_.c_str();

    // Передаём статическую функцию-обработчик, которая соответствует сигнатуре callback
    
    auto* auth_join_event = new AuthServiceEventListener(actionCallback);
    err = auth_service_->SetEvent(auth_join_event);
    
    if (err != SDKERR_SUCCESS) {
        std::cerr << "Failed to set auth event listener: " << err << std::endl;
    } else {
        std::cout << "AuthServiceEventListener added." << std::endl;
    }

    err = auth_service_->SDKAuth(param);
    if (err != SDKERR_SUCCESS) {
        std::cerr << "Zoom SDK authentication failed: " << err << std::endl;
    } else {
        std::cout << "Zoom SDK authentication successful!" << std::endl;
    }
}

void ZoomBot::CleanZoomSDK() {
    if (auth_service_) {
        DestroyAuthService(auth_service_);
        auth_service_ = nullptr;
    }
    if (setting_service_) {
        DestroySettingService(setting_service_);
        setting_service_ = nullptr;
    }
    if (meeting_service_) {
        DestroyMeetingService(meeting_service_);
        meeting_service_ = nullptr;
    }

    if (CleanUPSDK() != SDKERR_SUCCESS) {
        std::cerr << "CleanSDK meetingSdk: error" << std::endl;
    } else {
        std::cerr << "CleanSDK meetingSdk: success" << std::endl;
    }
}

void ZoomBot::LeaveFromMeeting() {
    if (!meeting_service_) {
        std::cout << "leave_meeting: meeting_service is null" << std::endl;
        return;
    }

    MeetingStatus status = meeting_service_->GetMeetingStatus();
    if (status == MEETING_STATUS_IDLE || 
        status == MEETING_STATUS_ENDED ||
        status == MEETING_STATUS_FAILED) {
        std::cout << "LeaveMeeting() not in meeting" << std::endl;
        return;
    }

    if (meeting_service_->Leave(LEAVE_MEETING) == SDKERR_SUCCESS) {
        std::cout << "LeaveMeeting() success" << std::endl;
    } else {
        std::cout << "LeaveMeeting() error" << std::endl;
    }
}

void ZoomBot::signal_handler(int s) {
    std::cout << "\nCaught signal " << s << std::endl;
    s_instance->LeaveFromMeeting();
    s_instance->CleanZoomSDK();
    std::exit(0);
}

void ZoomBot::initAppSettings() {
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = signal_handler;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
}

// Статическая функция-обработчик для успешной аутентификации
void ZoomBot::onAuthSuccess() {
    if (s_instance) {
        s_instance->AuthCallback();
    }
}

// Функция для запуска бота с индивидуальными параметрами в отдельном процессе
void launch_bot(const std::string &bot_name, 
                const std::string &token, 
                const std::string &meeting_num, 
                const std::string &pwd) {
    pid_t pid = fork();
    if (pid == 0) {
        // Дочерний процесс для бота
        ZoomBot bot(token, meeting_num, pwd, bot_name);
        bot.run();
        
        // Запускаем главный цикл GLib для обработки событий
        GMainLoop* loop = g_main_loop_new(NULL, FALSE);
        g_main_loop_run(loop);
        g_main_loop_unref(loop);
        exit(0);
    } else if (pid < 0) {
        std::cerr << "Failed to fork for " << bot_name << std::endl;
        exit(1);
    }
}

void parseZoomLink(const std::string& link, std::string& meetingID, std::string& password) {
    std::regex re("wc/([0-9]+)/.*[?&]pwd=([^&]+)");
    std::smatch match;
    
    if (std::regex_search(link, match, re) && match.size() == 3) {
        meetingID = match[1].str();
        password = match[2].str();
    } else {
        meetingID.clear();
        password.clear();
    }
}

