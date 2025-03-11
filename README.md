# RecoZoomBot

## Install ZoomSDK libs
just run ```utils/installers/install_zoom_sdk.sh```

если вдруг не заработал скрипт, то необходимо определенные файлы из архива перенсти в соответсвующие диры:
папку **qt_libs** и **\*.so** файлы в **rzbcpp/lib**

Потом в **rzbcpp/lib**:
 
```bash
ln -s libmeetingsdk.so libmeetingsdk.so.1
```

По идее на этом этапе должно все запуститься, но что-то всегда может идти не так)

## installing dependencies

install package for building package

```bash
sudo pacman -Sy pybind11 
sudo pacman -Sy glib2
```

install glib(gl package) for global venv

```bash
sudo pacman -Sy python-gobject    
```
install glib(gl package) for local venv

```bash
pip install pygobject
```

set dependencies for python pkg
```bash
export LD_LIBRARY_PATH=/path/to/rzbcpp/lib:$LD_LIBRARY_PATH
```

## Running
### C++
Перед запуском приедется самостоятельно вводить токен, номер конфернеции и пароль от нее, автоматически там делать не хотелось.
В **main.cpp** можно будет найти все нужные поля)
Токен предварительно надо будет сгенерить в **utils/token_generator** командной ```node index.js```. Но перед этим надо вставить *api_key* и *api_secret* в этот файл.

В **rzbcpp** папке:
```bash
cmake -B build
cd build/
make
./zoom_app
```

### Python
Тут все проще, в файле **[rzbpy/test.py](rzbpy/test.py)** вставляем api_key и api_secret от Zoom App, вставляем ссылку на встречу, и потом просто запускаем файл.

PS: В идеале это в переменыне среды, но мне было лень)
```python
API_KEY = ""
API_SECRET = ""

meeting_url = "ZOOM_LINK"
```

## Как рулить?

Для питона все описано в **[rzbpy/test.py](rzbpy/recozoombot/rzb.py)**, там вроде понятно, но для базового запуска, чтобы протестить, можно использовать метод **run_with_cli()**, он все запустит и дальше только смотреть в терминал, коннектнулся бот или нет.

Дальше бот коннектнится к встрече и запросит доступ к записи, при разрешении он ее начнет. Боту также через консоль можно давать команды. Они описаны все в BotRecording.cpp

- join - присоединиться к встрече
- leave - выйти из встречи
- recoallow - запросить доступ
- recorun - запустить запись
- recostop - остановить запись
- exit - выход из бота