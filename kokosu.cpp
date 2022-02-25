#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"

#include "icon.hpp"
#include "input.hpp"

#define WINDOW_WIDTH 150
#define WINDOW_HEIGHT 178

extern char left_s[16];
extern char right_s[16];
extern char bpm_s[16];
extern unsigned int left_i;
extern unsigned int right_i;
extern bool left_down;
extern bool right_down;

extern bool lmb_down;
extern double last_mouse_clicked_x;
extern double last_mouse_clicked_y;

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

int main(int, char**)
{
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

#if defined(IMGUI_IMPL_OPENGL_ES2)
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#else
    const char* glsl_version = "#version 130";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
#endif

    glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "kokosu", NULL, NULL);
    if (window == NULL)
        return 1;
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.IniFilename = NULL;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    ImGuiStyle& style = ImGui::GetStyle();
    style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
    style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.28f, 0.05f, 0.66f, 1.0f);
    style.Colors[ImGuiCol_Button]        = ImVec4(0.28f, 0.05f, 0.66f, 1.0f);
    style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.34f, 0.04f, 0.68f, 1.0f);
    style.Colors[ImGuiCol_ButtonActive]  = ImVec4(0.34f, 0.04f, 0.68f, 1.0f);

    ImFontConfig config;
    config.SizePixels = 13;
    config.OversampleH = config.OversampleV = 1;
    config.PixelSnapH = true;
    ImFont *default_font = io.Fonts->AddFontDefault(&config);

    config.SizePixels = 24;
    ImFont *font = io.Fonts->AddFontDefault(&config);

    int channels;
    int width, height;
    GLFWimage icon[1];
    icon[0].pixels = stbi_load_from_memory(kokosu_128x128_map, 128 * 128, &width, &height, &channels, 0);
    icon[0].height = height;
    icon[0].width = width;
    glfwSetWindowIcon(window, 1, icon);

    init_input();

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();

        if (lmb_down)
        {
            int win_x, win_y;
            double mouse_x, mouse_y;
            glfwGetCursorPos(window, &mouse_x, &mouse_y);
            glfwGetWindowPos(window, &win_x, &win_y);
            glfwSetWindowPos(window, mouse_x + win_x - last_mouse_clicked_x, mouse_y + win_y - last_mouse_clicked_y);
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowSize(ImVec2(WINDOW_WIDTH, WINDOW_HEIGHT), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f), ImGuiCond_Once);

        ImGui::Begin("kokosu", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

        ImGui::PushFont(font);

        ImGui::PushID(0);
        if (left_down) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.34f, 0.04f, 0.68f, 1.0f));
        if (ImGui::Button(left_s, ImVec2(63.0f, 115.0f)))
            reset();
        if (left_down) ImGui::PopStyleColor();
        ImGui::PopID();

        ImGui::SameLine();

        ImGui::PushID(1);
        if (right_down) ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.34f, 0.04f, 0.68f, 1.0f));
        if (ImGui::Button(right_s, ImVec2(63.0f, 115.0f))) reset();
        if (right_down) ImGui::PopStyleColor();
        ImGui::PopID();

        ImGui::SetCursorPosX((ImGui::GetWindowSize().x - ImGui::CalcTextSize(bpm_s).x) * 0.5f);
        ImGui::PushID(2);
        ImGui::Text("%s", bpm_s);
        ImGui::PopID();

        ImGui::PopFont();

        ImGui::End();

        ImGui::Render();
        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
