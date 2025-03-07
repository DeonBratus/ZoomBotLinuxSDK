
#ifndef ZOOMBOT_H
#define ZOOMBOT_H

#include <iostream>
#include <zoom_sdk.h>
#include <zoom_sdk_def.h>
#include <auth_service_interface.h>
#include <meeting_service_interface.h>
#include <setting_service_interface.h>
#include <signal.h>
#include <glib.h>
#include <vector>
#include <cstdlib>
#include <string>
#include <unistd.h>          // для fork()
#include <sys/wait.h>        // для wait()
#include <regex>


USING_ZOOM_SDK_NAMESPACE

class ZoomBot {
public:
    // Статическая переменная для хранения текущего экземпляра
    static ZoomBot* s_instance;

    ZoomBot(const std::string& token, const std::string& meetingNum, const std::string& pwd, const std::string botName);
    ZoomBot();

    void run();
    void JoinWithAuthToMeeting(void(*actionCallback)());
    void LeaveFromMeeting();
    void JoinToMeeting();
    void createServices();

    void InitZoomSDK();
    void initAppSettings();

    std::string bot_name; 
    std::string token_;
    std::string meeting_num_;
    std::string m_pwd_;

    IMeetingService* meeting_service_{nullptr};
    IAuthService* auth_service_{nullptr};
    ISettingService* setting_service_{nullptr};
    

private:
    void AuthCallback();
    void CleanZoomSDK();


public:
    // Статическая функция-обработчик для успешной аутентификации
    static void onAuthSuccess();
    static void signal_handler(int s);

};

// Инициализация статической переменной
extern GMainLoop* loop;

extern std::vector<ZoomBot*> g_bots;

// Функция для запуска бота с индивидуальными параметрами в отдельном процессе
void launch_bot(const std::string &bot_name, 
                const std::string &token, 
                const std::string &meeting_num, 
                const std::string &pwd);

void parseZoomLink(const std::string& link, std::string& meetingID, std::string& password);

#endif 