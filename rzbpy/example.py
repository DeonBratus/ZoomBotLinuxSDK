from zbpy_pkg import ZoomBot

def main():
    bot = ZoomBot()
    
    bot.configure(
        meeting_token="eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzZGtLZXkiOiJvRGNsaWpESlNvQ3Uwdko1MThkOUlBIiwiYXBwS2V5Ijoib0RjbGlqREpTb0N1MHZKNTE4ZDlJQSIsIm1uIjo3MzYxNDM1ODQ5OCwicm9sZSI6MCwiaWF0IjoxNzQxNTMzMDk3LCJleHAiOjE3NDE1NDAyOTcsInRva2VuRXhwIjoxNzQxNTQwMjk3fQ.jeAyYqNc39DC1lp7Y0iZmm3wjC0rP0i_CkLnNO7j0bk",
        meeting_id="79789006489",
        meeting_pwd="flaKPtYu4EO5lxwK0lkHD5xs00jyaL.1",
        bot_name="PythonBot"
    )
    
    bot.run()


if __name__ == "__main__":
    main()