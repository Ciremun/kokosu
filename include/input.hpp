#pragma once

#include <GLFW/glfw3.h>

typedef enum
{
    LEFT = 0,
    RIGHT,
} Key;

void init_input();
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void on_key_pressed(Key key);
void on_key_released(Key key);
