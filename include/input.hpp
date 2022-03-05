#pragma once

#include <GLFW/glfw3.h>

typedef enum
{
    LEFT = 0,
    RIGHT,
} Key;

void reset();
void init_input();
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void on_key_pressed(Key key);
void on_key_released(Key key);
void set_always_on_top(GLFWwindow* window);
void unset_always_on_top(GLFWwindow* window);
