#include "stb_sprintf.h"

#include "input.hpp"

bool lmb_down = false;
double last_mouse_clicked_x = 0.0f;
double last_mouse_clicked_y = 0.0f;

char left_s[16] = {0};
char right_s[16] = {0};
unsigned int left_i = 0;
unsigned int right_i = 0;
bool left_down = false;
bool right_down = false;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if(action == GLFW_PRESS)
        {
            glfwGetCursorPos(window, &last_mouse_clicked_x, &last_mouse_clicked_y);
            lmb_down = true;
        }
        if(action == GLFW_RELEASE)
            lmb_down = false;
    }
}

#ifdef _WIN32
#include <windows.h>

#include <stdio.h>

HHOOK hHook;

LRESULT CALLBACK HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT cKey = *((KBDLLHOOKSTRUCT *)lParam);

    if (wParam == WM_KEYUP)
    {
        if (cKey.vkCode == 83)
            left_down = false;
        if (cKey.vkCode == 68)
            right_down = false;
        return CallNextHookEx(hHook, nCode, wParam, lParam);
    }

    if (65 <= cKey.vkCode && cKey.vkCode <= 90)
    {
        if (cKey.vkCode == 83 && !left_down)
        {
            left_down = true;
            stbsp_snprintf(left_s, sizeof(left_s), "%d", ++left_i);
        }
        if (cKey.vkCode == 68 && !right_down)
        {
            right_down = true;
            stbsp_snprintf(right_s, sizeof(right_s), "%d", ++right_i);
        }
    }

    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

void init_input()
{
    SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0);
}

#else

#include <X11/Xlib.h>

#include <thread>

void set_keyboard_hook()
{
    Display *display = XOpenDisplay(0);
    XAutoRepeatOff(display);
    Window root = DefaultRootWindow(display);
    Window current_focus_window;
    int revert;

    XGetInputFocus(display, &current_focus_window, &revert);
    XSelectInput(display, current_focus_window, KeyPressMask | KeyReleaseMask | FocusChangeMask);
    XIM xim = XOpenIM(display, 0, 0, 0);
    XIC xic = XCreateIC(xim, XNInputStyle, XIMPreeditNothing | XIMStatusNothing, NULL);
    XEvent event;

    while (true)
    {
        XNextEvent(display, &event);
        switch (event.type)
        {
        case FocusOut:
        {
            if (current_focus_window != root)
                XSelectInput(display, current_focus_window, 0);
            XGetInputFocus(display, &current_focus_window, &revert);
            if (current_focus_window == PointerRoot)
                current_focus_window = root;
            XSelectInput(display, current_focus_window, KeyPressMask | KeyReleaseMask | FocusChangeMask);
        } break;
        case KeyPress:
        {
            if (event.xkey.keycode == 39 && !left_down)
            {
                left_down = true;
                stbsp_snprintf(left_s, sizeof(left_s), "%d", ++left_i);
            }
            if (event.xkey.keycode == 40 && !right_down)
            {
                right_down = true;
                stbsp_snprintf(right_s, sizeof(right_s), "%d", ++right_i);
            }
        } break;
        case KeyRelease:
        {
            if (event.xkey.keycode == 39)
                left_down = false;
            if (event.xkey.keycode == 40)
                right_down = false;
        } break;
        }
    }
}

void init_input()
{
    static std::thread t(set_keyboard_hook);
}

#endif // _WIN32
