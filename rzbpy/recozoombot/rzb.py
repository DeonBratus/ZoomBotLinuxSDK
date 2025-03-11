from . import zoombotpy
from urllib.parse import urlparse, parse_qs
import time
import jwt


class ZoomRecoBot:
    def __init__(self):
        self.bot = zoombotpy.ZoomRecoBot()

    def init_sdk(self):
        """Инициализировать SDK для бота."""
        self.bot.init()

    def join_meeting(self):
        """Присоединиться к встрече."""
        self.bot.join()

    def init_settings(self):
        """Инициализировать настройки приложения."""
        self.bot.init_setting()

    def leave_meeting(self):
        """Покинуть встречу."""
        self.bot.leave()

    def run_with_cli(self):
        """Запустить бота и присоединиться к встрече."""
        self.bot.run_with_cli()

    def stop_recording(self):
        """Остановить текущую запись."""
        self.bot.stop_recording()

    def start_recording(self):
        """Начать запись."""
        self.bot.start_recording()

    def request_recording_permission(self):
        """Запросить разрешение на запись."""
        self.bot.request_recording_permission()

    @property
    def meeting_id(self):
        """Получить или установить ID встречи."""
        return self.bot.meeting_id

    @meeting_id.setter
    def meeting_id(self, value):
        self.bot.meeting_id = value

    @property
    def meeting_pwd(self):
        """Получить или установить пароль встречи."""
        return self.bot.meeting_pwd

    @meeting_pwd.setter
    def meeting_pwd(self, value):
        self.bot.meeting_pwd = value

    @property
    def meeting_token(self):
        """Получить или установить токен аутентификации JWT."""
        return self.bot.meeting_token

    @meeting_token.setter
    def meeting_token(self, value):
        self.bot.meeting_token = value

    @property
    def bot_name(self):
        """Получить или установить отображаемое имя бота."""
        return self.bot.bot_name

    @bot_name.setter
    def bot_name(self, value):
        self.bot.bot_name = value


def generate_token(key, secret, meeting_number, role):
    """JWT Token Generator"""
    iat = int(time.time()) - 30
    exp = iat + 2 * 60 * 60  # 2 hours
    
    payload = {
        'sdkKey': key,
        'appKey': key,
        'mn': meeting_number,
        'role': role,
        'iat': iat,
        'exp': exp,
        'tokenExp': exp
    }
    
    return jwt.encode(
        payload,
        secret,
        algorithm='HS256',
        headers={'alg': 'HS256', 'typ': 'JWT'}
    )


def parse_zoom_link(url):
    parsed_url = urlparse(url)
    
    path_parts = parsed_url.path.split('/')
    conference_id = None
    try:
        conference_index = path_parts.index('j') + 1
        conference_id = path_parts[conference_index]
    except (ValueError, IndexError):
        pass
    
    query_params = parse_qs(parsed_url.query)
    password = query_params.get('pwd', [None])[0]
    
    return {
        'id': conference_id,
        'password': password
    }


if __name__ == "__main__":
    print(generate_token("", "", 23456, 0))
