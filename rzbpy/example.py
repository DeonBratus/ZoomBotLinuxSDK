from zbpy_pkg import ZoomBot

def main():
    bot = ZoomBot()
    
    bot.configure(
        meeting_token="your_token",
        meeting_id="meeting_id",
        meeting_pwd="password",
        bot_name="PythonBot"
    )
    
    bot.run()


if __name__ == "__main__":
    main()