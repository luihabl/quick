#ifdef QUICK_PLATFORM_SDL2

#include "quick/application.h"

#include <imgui.h>
#include <imgui_impl_sdl.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <SDL.h>

#include <stdio.h>

#include <chrono>
#include <thread>

#define STR2(X) #X
#define STR(X) STR2(X)
#define GL_VERSION_MAJOR 3
#define GL_VERISON_MINOR 3

namespace 
{
    SDL_Window* window = nullptr;
    SDL_GLContext gl_context;
}

namespace quick
{

    bool Application::setup(const Config& config)
    {
        if(window)
            return false;

        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
        {
            printf("Error: %s\n", SDL_GetError());
            return -1;
        }

        const char* glsl_version = "#version " STR(GL_VERSION_MAJOR) STR(GL_VERISON_MINOR) "0";

        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, GL_VERSION_MAJOR);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, GL_VERISON_MINOR);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac

        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        window = SDL_CreateWindow(config.name.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, config.w, config.h, window_flags);

        gl_context = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, gl_context);
        SDL_GL_SetSwapInterval(config.use_vsync ? 1 : 0); // Enable vsync

        if(!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress))
        {
            printf("Failed to load GL functions");
            return false;
        }
        
        // ImGui Setup
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
        ImGui_ImplOpenGL3_Init(glsl_version);

        use_framecap = config.use_framecap;
        fps_target = config.fps_target;
        clear_color = config.clear_color;

        start(config);

        return true;
    }


    void Application::run()
    {
        using dsec = std::chrono::duration<float>;
        std::chrono::system_clock::duration frame_dt = std::chrono::round<std::chrono::system_clock::duration>(dsec{1./fps_target});
        auto begin_frame = std::chrono::system_clock::now();
        auto end_frame = begin_frame + frame_dt;

        ImGuiIO& io = ImGui::GetIO(); (void)io;

        while(!should_quit())
        {

            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                ImGui_ImplSDL2_ProcessEvent(&event);
                if (event.type == SDL_QUIT)
                    m_quit = true;
                if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                    m_quit = true;
            }
            
            draw();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplSDL2_NewFrame();
            ImGui::NewFrame();

            update();
            
            // Rendering
            ImGui::Render();
            glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Update and Render additional Platform Windows
            // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
            //  For this specific demo app we could also call SDL_GL_MakeCurrent(window, gl_context) directly)
            if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
            {
                SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
                SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
                ImGui::UpdatePlatformWindows();
                ImGui::RenderPlatformWindowsDefault();
                SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
            }

            SDL_GL_SwapWindow(window);

            if (use_framecap)
            {
                std::this_thread::sleep_until(end_frame);
                begin_frame = end_frame;
                end_frame = begin_frame + frame_dt;
            }
        }
    }

    bool Application::should_quit()
    {
        return m_quit;
    }


    void Application::quit()
    {
        m_quit = true;
    }

    void Application::terminate()
    {
        // Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();

        SDL_GL_DeleteContext(gl_context);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }



}

#endif