# ![kokosu_32x32](https://user-images.githubusercontent.com/38132413/155867298-41ef3874-8ffd-4175-a42a-106a9a2161cb.png) kokosu

<img align="left" src="https://user-images.githubusercontent.com/38132413/155867408-ea69b566-85f9-4566-92e7-43f7a2b5f79f.png">

osu! key overlay  

<br><br><br><br><br><br>

## Windows

    g++ -DIMGUI_USE_STB_SPRINTF *.cpp imgui/*.cpp -Iinclude -Iimgui -lglfw3 -lopengl32 -Wl,-subsystem,windows

## Linux

    g++ -DIMGUI_USE_STB_SPRINTF *.cpp imgui/*.cpp -Iinclude -Iimgui -lglfw -lpthread -ldl -lGL -lX11

## Controls

|  Keys       | Description    |
|-------------|----------------|
| Z (Default) | Left Click     |
| X (Default) | Right Click    |
| Alt + /     | Reset (Hotkey) |
| LMB         | Reset          |
| RMB         | Settings       |

|   Settings    |
|---------------|
| Remap Keys    |
| Always On Top |
