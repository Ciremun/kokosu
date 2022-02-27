# kokosu

<img align="left" src="https://user-images.githubusercontent.com/38132413/155426092-839e7818-80dc-4cad-8b76-aba1191c564c.png">

osu! key overlay  

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
