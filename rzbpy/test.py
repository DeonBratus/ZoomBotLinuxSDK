from recozoombot import ZoomRecoBot, generate_signature, parse_zoom_link
import gi.repository.GLib

API_KEY = ""
API_SECRET = ""

meeting_url = "https://us05web.zoom.us/j/84539958981?pwd=tD8n3AR5Jyi4OOr5W5NdzPzXdF128e.1"

def run(name: str = None, zoom_url: str = None):

    loop = gi.repository.GLib.MainLoop()

    result = parse_zoom_link(zoom_url)
    token = generate_signature(API_KEY, API_SECRET, result["id"], 0)

    recording_bot = ZoomRecoBot()

    recording_bot.bot_name = name
    recording_bot.meeting_token = token
    recording_bot.meeting_id = result["id"]
    recording_bot.meeting_pwd = result["password"]
    recording_bot.run_with_cli()

    loop.run()


if __name__ == "__main__":

    run("recobot", zoom_url=meeting_url)
