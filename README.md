# ![kokosu_32x32](https://user-images.githubusercontent.com/38132413/155867298-41ef3874-8ffd-4175-a42a-106a9a2161cb.png) kokosu

<img align="left" src="https://user-images.githubusercontent.com/38132413/156884152-12f2bf40-ff9d-4c0b-8c1f-de160c054626.png">

osu! key overlay  

<br><br><br><br><br><br><br>

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
