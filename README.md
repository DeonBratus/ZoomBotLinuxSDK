# RecoZoomBot

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
export LD_LIBRARY_PATH=/home/debral/DevSpace/RecoZoomBot/rzbcpp/lib:$LD_LIBRARY_PATH
```

## Install ZoomSDK libs
just run utils/installers/install_zoom_sdk.sh