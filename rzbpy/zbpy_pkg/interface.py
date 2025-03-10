import zoombotpy
import gi.repository.GLib

class ZoomBot:
    def __init__(self):
        self.bot = zoombotpy.ZoomRecoBot()
        self.loop = gi.repository.GLib.MainLoop()

    def configure(self, meeting_token, meeting_id, meeting_pwd, bot_name):
        self.bot.meeting_token = meeting_token
        self.bot.meeting_id = meeting_id
        self.bot.meeting_pwd = meeting_pwd
        self.bot.bot_name = bot_name

    def run(self):
        self.bot.run()
        self.loop.run()

def main():
    bot = ZoomBot()
    
    bot.configure(
        meeting_token="eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzZGtLZXkiOiJvRGNsaWpESlNvQ3Uwdko1MThkOUlBIiwiYXBwS2V5Ijoib0RjbGlqREpTb0N1MHZKNTE4ZDlJQSIsIm1uIjo3MzYxNDM1ODQ5OCwicm9sZSI6MCwiaWF0IjoxNzQxNTY0Nzc5LCJleHAiOjE3NDE1NzE5NzksInRva2VuRXhwIjoxNzQxNTcxOTc5fQ.MG_TMxbJ_7MYDTzb6mxAN80_8x337bNkBgpSntLTn1A",
        meeting_id="71422996542",
        meeting_pwd="cq5KKIYqCkxTjGaG7tUeVjIEVIdVp8.1",
        bot_name="PythonBot"
    )
    
    bot.run()


if __name__ == "__main__":
    main()
