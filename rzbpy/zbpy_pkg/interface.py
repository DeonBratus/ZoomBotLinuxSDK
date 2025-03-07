from . import zoombotpy
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
