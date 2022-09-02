#include "quick/application.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include <chrono>
#include <thread>

#define STR2(X) #X
#define STR(X) STR2(X)
#define GL_VERSION_MAJOR 3
#define GL_VERISON_MINOR 3

#define TARGET_FPS 60.0f

namespace 
{
    GLFWwindow* window = nullptr;
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}


namespace quick
{

    bool Application::setup()
    {
        if(window)
            return false;

        glfwSetErrorCallback(glfw_error_callback);

        if (!glfwInit())
            return false;

        const char* glsl_version = "#version " STR(GL_VERSION_MAJOR) STR(GL_VERISON_MINOR) "0";
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERISON_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  
    
        window = glfwCreateWindow(1280, 720, "Dear ImGui GLFW+OpenGL3 example", NULL, NULL);
        if (!window)
            return false;

        glfwMakeContextCurrent(window);
        glfwSwapInterval(1); // Enable vsync

        if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        {
            printf("Failed to load GL functions");
            return false;
        }
        
        // ImGui Setup
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        // Remove this
        clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        start();

        return true;
    }


    void Application::run()
    {
        using dsec = std::chrono::duration<double>;
        std::chrono::system_clock::duration frame_dt = std::chrono::round<std::chrono::system_clock::duration>(dsec{1./TARGET_FPS});
        auto begin_frame = std::chrono::system_clock::now();
        auto end_frame = begin_frame + frame_dt;

        while(!should_quit())
        {
            glfwWaitEventsTimeout(1.0f / (6 * TARGET_FPS)); // Wait for 1/6th of the frame dt for events
            // glfwPollEvents();
            
            draw();

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            update();
            
            ImGui::Render();
            int display_w, display_h;
            glfwGetFramebufferSize(window, &display_w, &display_h);
            glViewport(0, 0, display_w, display_h);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            glClear(GL_COLOR_BUFFER_BIT);
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);

            std::this_thread::sleep_until(end_frame);
            begin_frame = end_frame;
            end_frame = begin_frame + frame_dt;

        }
    }

    bool Application::should_quit()
    {
        return glfwWindowShouldClose(window) || m_quit;
    }


    void Application::quit()
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        m_quit = true;
    }

    void Application::terminate()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        window = nullptr;

        glfwTerminate();
    }



}