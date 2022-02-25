# kokosu

![kokosu_128x128](https://user-images.githubusercontent.com/38132413/155426092-839e7818-80dc-4cad-8b76-aba1191c564c.png)

## Linux build

    g++ -DIMGUI_USE_STB_SPRINTF *.cpp imgui/*.cpp -Iinclude -Iimgui -lglfw -lpthread -ldl -lGL -lX11

## Windows build

    g++ -DIMGUI_USE_STB_SPRINTF *.cpp imgui/*.cpp -Iinclude -Iimgui -lglfw3 -lopengl32 -Wl,-subsystem,windows

## Keys

|  Keys   | Description |
|---------|-------------|
| S       | Left Click  |
| D       | Right Click |
| Alt + / | Reset       |
