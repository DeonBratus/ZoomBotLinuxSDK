#include <BotRecording.h>

#include <glib.h>

int main() {
    BotRecording bot;
    bot.meeting_num_ = "77534130011";
    bot.m_pwd_ = "iyoT7lFLyoFEOsu0RBS3o6tYTH0rf1.1";
    bot.token_ = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzZGtLZXkiOiJvRGNsaWpESlNvQ3Uwdko1MThkOUlBIiwiYXBwS2V5Ijoib0RjbGlqREpTb0N1MHZKNTE4ZDlJQSIsIm1uIjo3MzYxNDM1ODQ5OCwicm9sZSI6MCwiaWF0IjoxNzQxNjk4MjgyLCJleHAiOjE3NDE3MDU0ODIsInRva2VuRXhwIjoxNzQxNzA1NDgyfQ.aCPeVg1jno9dLXIl7gVPP9XMELageGbvISCjRGky3pY";
    bot.bot_name = "RecoBot";
    
    bot.recorun();

    GMainLoop* loop = g_main_loop_new(nullptr, FALSE);
    g_main_loop_run(loop);
    g_main_loop_unref(loop);
    
    return 0;
}