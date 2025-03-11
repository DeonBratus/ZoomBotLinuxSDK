#include <BotRecording.h>

#include <glib.h>

int main() {
    BotRecording bot;
    bot.meeting_num_ = "";
    bot.m_pwd_ = "";
    bot.token_ = "";
    bot.bot_name = "RecoBot";
    
    bot.recorun();

    GMainLoop* loop = g_main_loop_new(nullptr, FALSE);
    g_main_loop_run(loop);
    g_main_loop_unref(loop);
    
    return 0;
}