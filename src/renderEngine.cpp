#include "../include/renderEngine.h"

#include <algorithm>
#include <ostream>
#include <string>
#include <vector>

#if defined(_WIN64)
#include <Windows.h>
#endif
#include <GL/gl.h>
#include <SDL.h>
#include <SDL_events.h>
#include <SDL_opengl.h>
#include <SDL_video.h>

#include <cstddef>
#include <cstdio>
#include <iostream>

#include "../depend/imgui/backends/imgui_impl_opengl3.h"
#include "../depend/imgui/backends/imgui_impl_sdl2.h"
#include "../depend/imgui/imgui.h"
#include "../include/core.h"

renderEngine::renderEngine() { }
renderEngine::~renderEngine() { }
SDL_Window* window;
SDL_GLContext gl_context;
ImGuiIO io;
ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

// Start engine
void renderEngine::Initialise(const char* title, int w, int h)
{
    // SDL Attributes
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    // Create window with flags
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL);
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED, w, h, window_flags);

    // Initialise renderer
    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);

    // Setup ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    io = ImGui::GetIO();
    (void)io;
    // ImGui::GetIO().IniFilename = NULL;
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Setup Platform/Renderer backends
    ImGui_ImplOpenGL3_Init();
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);

    // Setup Done
    isRunning = true;
}

// Tick renderengine
void renderEngine::Update()
{
    // Tick
    tick++;

    // Handle events
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            isRunning = false;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
            isRunning = false;
    }

    //  Start the ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    // Common variables
    static ImGuiTableFlags commonFlags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse;
    static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable;

    // Menu Bar
    ImGui::BeginMainMenuBar();
    ImGui::Text("NIP-Engine");
    ImGui::Separator();
    ImGui::Text(D_VERSION);
    ImGui::Separator();
    // ImGui::Separator();
    ImGui::EndMainMenuBar();

    // Step 1
    ImGui::Begin("Input", NULL,
        commonFlags);

    ImGui::InputText("Input csv file", path, 60);
    if (ImGui::Button("Load file")) {
        std::vector<dataStructure> data;
        data = csvLoader::LoadCSV(path);
        csvLoader::RepairRawData(&data);
        fData = csvLoader::SumRawData(&data);
        csvLoader::SortFinalData(&fData);
    }

    ImGui::End();

    if (popup_id == "Error_UnevenTemps") {
        ImGui::OpenPopup("Error_UnevenTemps");

        if (ImGui::BeginPopupModal("Error_UnevenTemps", NULL, commonFlags)) {
            ImGui::Text("Potential Error: Uneven temperature intervals detected!");
            if (ImGui::Button("Ok!")) {
                popup_id = " ";
            }
            ImGui::EndPopup();
        }
    }
}

// Render
void renderEngine::Render()
{
    // Imgui Render
    ImGui::Render();

    // Clear and render
    glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w,
        clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    SDL_GL_SwapWindow(window);
}

// Clean
void renderEngine::Clean()
{
    // Shutdown imgui graphic implementation
    ImGui_ImplOpenGL3_Shutdown();
    // Clean Imgui
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    // Clean SDL
    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
    std::cout << "Engine Cleaned!" << std::endl;
}
