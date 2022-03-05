#include <string.h>

#include "stb_sprintf.h"

#include "input.hpp"

bool lmb_down = false;
double last_mouse_clicked_x = 0.0f;
double last_mouse_clicked_y = 0.0f;

char left_s[16] = {'0', '\0'};
char right_s[16] = {'0', '\0'};
unsigned int left_i = 0;
unsigned int right_i = 0;
bool left_down = false;
bool right_down = false;
bool drag = false;

double begin = 0.0;
int keypresses_count = 0;
char bpm_s[16] = {'0', '.', '0', '0', '\0'};

char left_click_key[2]  = {'Z', '\0'};
char right_click_key[2] = {'X', '\0'};

void reset()
{
    left_i = 0;
    right_i = 0;
    keypresses_count = 0;
    memcpy(left_s, "0", 2);
    memcpy(right_s, "0", 2);
    memcpy(bpm_s, "0.00", 5);
}

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
        {
            lmb_down = false;
            drag = false;
        }
    }
}

void on_key_pressed(Key key)
{
    if (key == Key::LEFT && !left_down)
    {
        left_down = true;
        stbsp_snprintf(left_s, sizeof(left_s), "%d", ++left_i);
    }
    if (key == Key::RIGHT && !right_down)
    {
        right_down = true;
        stbsp_snprintf(right_s, sizeof(right_s), "%d", ++right_i);
    }
    if (!keypresses_count)
        begin = glfwGetTime();
    else
        stbsp_snprintf(bpm_s, sizeof(bpm_s), "%.2f", 15 * keypresses_count / (glfwGetTime() - begin));
    keypresses_count++;
}

void on_key_released(Key key)
{
    if (key == Key::LEFT)
        left_down = false;
    if (key == Key::RIGHT)
        right_down = false;
}

#ifdef _WIN32
#include <windows.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <thread>

HHOOK hHook;

void set_always_on_top(GLFWwindow* window)
{
    HWND hwnd = glfwGetWin32Window(window);
    SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void unset_always_on_top(GLFWwindow* window)
{
    HWND hwnd = glfwGetWin32Window(window);
    SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

static void register_hotkey()
{
    if (RegisterHotKey(NULL, 100, MOD_ALT | MOD_NOREPEAT, 0xBF))
    {
        MSG msg = {0};
        while (GetMessage(&msg, NULL, 0, 0) != 0)
            if (msg.message == WM_HOTKEY)
                reset();
    }
}

LRESULT CALLBACK HookCallback(int nCode, WPARAM wParam, LPARAM lParam)
{
    KBDLLHOOKSTRUCT cKey = *((KBDLLHOOKSTRUCT *)lParam);

    if ('A' <= cKey.vkCode && cKey.vkCode <= 'Z')
    {
        if (wParam == WM_KEYUP)
        {
            if (cKey.vkCode == left_click_key[0])  on_key_released(Key::LEFT);
            if (cKey.vkCode == right_click_key[0]) on_key_released(Key::RIGHT);
            return CallNextHookEx(hHook, nCode, wParam, lParam);
        }

        if (cKey.vkCode == left_click_key[0]  && !left_down)  on_key_pressed(Key::LEFT);
        if (cKey.vkCode == right_click_key[0] && !right_down) on_key_pressed(Key::RIGHT);
    }

    return CallNextHookEx(hHook, nCode, wParam, lParam);
}

void init_input()
{
    static std::thread hotkey(register_hotkey);
    SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, NULL, 0);
}

#else

#include <X11/XKBlib.h>
#include <X11/Xlib.h>

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

#include <thread>

#define _NET_WM_STATE_TOGGLE 2

static void toggle_always_on_top(GLFWwindow *window)
{
    Display *display = XOpenDisplay(0);
    Window w = glfwGetX11Window(window);
    XClientMessageEvent xclient;
    memset(&xclient, 0, sizeof (xclient));
    xclient.type = ClientMessage;
    xclient.window = w;
    xclient.message_type = XInternAtom(display, "_NET_WM_STATE", 1);
    xclient.format = 32;
    xclient.data.l[0] = _NET_WM_STATE_TOGGLE;
    xclient.data.l[1] = XInternAtom(display, "_NET_WM_STATE_ABOVE", 1);
    xclient.data.l[2] = 0;
    xclient.data.l[3] = 0;
    xclient.data.l[4] = 0;
    XSendEvent(display, DefaultRootWindow(display), False, SubstructureRedirectMask | SubstructureNotifyMask, (XEvent *)&xclient);
    XFlush(display);
}

void set_always_on_top(GLFWwindow* window)
{
    toggle_always_on_top(window);
}

void unset_always_on_top(GLFWwindow* window)
{
    toggle_always_on_top(window);
}

static void register_hotkey()
{
    Display *dpy = XOpenDisplay(0);
    Window root = DefaultRootWindow(dpy);
    XEvent ev;
    XGrabKey(dpy, 61, Mod1Mask, root, 0, GrabModeAsync, GrabModeAsync);
    XSelectInput(dpy, root, KeyPressMask);
    while (true)
    {
        XNextEvent(dpy, &ev);
        if (ev.type == KeyPress)
            reset();
    }
}

static void set_keyboard_hook()
{
    Display *display = XOpenDisplay(0);
    Window root = DefaultRootWindow(display);
    XkbSetDetectableAutoRepeat(display, True, NULL);
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
            if (!left_down  && event.xkey.keycode == XKeysymToKeycode(display, left_click_key[0]))  on_key_pressed(Key::LEFT);
            if (!right_down && event.xkey.keycode == XKeysymToKeycode(display, right_click_key[0])) on_key_pressed(Key::RIGHT);
        } break;
        case KeyRelease:
        {
            if (event.xkey.keycode == XKeysymToKeycode(display, left_click_key[0]))  on_key_released(Key::LEFT);
            if (event.xkey.keycode == XKeysymToKeycode(display, right_click_key[0])) on_key_released(Key::RIGHT);
        } break;
        }
    }
}

void init_input()
{
    static std::thread hotkey(register_hotkey);
    static std::thread kb_hook(set_keyboard_hook);
}

#endif // _WIN32
