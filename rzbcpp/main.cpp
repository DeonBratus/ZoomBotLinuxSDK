#include <BotRecording.h>

#include <glib.h>

int main() {
    BotRecording bot;
    bot.meeting_num_ = "4853804281";
    bot.m_pwd_ = "6n7EIheVMiBibt10tXb9aMYSjACHfe.1";
    bot.token_ = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzZGtLZXkiOiJvRGNsaWpESlNvQ3Uwdko1MThkOUlBIiwiYXBwS2V5Ijoib0RjbGlqREpTb0N1MHZKNTE4ZDlJQSIsIm1uIjo0ODUzODA0MjgxLCJyb2xlIjowLCJpYXQiOjE3NDE3MTc3MjAsImV4cCI6MTc0MTcyNDkyMCwidG9rZW5FeHAiOjE3NDE3MjQ5MjB9.0BZSj3qvnOkuo0k8YLaI7ZJea7ml4ltzkk0t9rHcO_4";
    bot.bot_name = "RecoBot";
    
    bot.recorun();

    GMainLoop* loop = g_main_loop_new(nullptr, FALSE);
    g_main_loop_run(loop);
    g_main_loop_unref(loop);
    
    return 0;
}