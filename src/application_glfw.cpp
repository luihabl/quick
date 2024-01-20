#ifdef QUICK_PLATFORM_GLFW

#include "quick/application.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <implot.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdio.h>

#include <chrono>
#include <thread>

#define STR2(X) #X
#define STR(X) STR2(X)
#define GL_VERSION_MAJOR 3
#define GL_VERISON_MINOR 3

namespace 
{
    GLFWwindow* window = nullptr;
}

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "Glfw Error %d: %s\n", error, description);
}

static void glfw_resize_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    auto* app = glfwGetWindowUserPointer(window);
    if(app) 
    {
        ((quick::Application*) app)->render();
    }
}

namespace quick
{
    bool Application::setup(const Config& config)
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
    
        window = glfwCreateWindow(config.w, config.h, config.name.c_str(), NULL, NULL);
        if (!window)
            return false;

        glfwMakeContextCurrent(window);
        glfwSwapInterval(config.use_vsync ? 1 : 0); // Enable vsync

        if(!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        {
            printf("Failed to load GL functions");
            return false;
        }

        if(config.render_on_resize) 
        {
            glfwSetWindowUserPointer(window, this);
            glfwSetFramebufferSizeCallback(window, glfw_resize_callback);
        }

        // ImGui Setup
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImPlot::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui::StyleColorsDark();

        // Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init(glsl_version);

        use_framecap = config.use_framecap;
        fps_target = config.fps_target;
        clear_color = config.clear_color;

        start(config);

        return true;
    }

    void Application::render()
    {
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
    }


    void Application::run()
    {
        using dsec = std::chrono::duration<float>;
        std::chrono::system_clock::duration frame_dt = std::chrono::round<std::chrono::system_clock::duration>(dsec{1./fps_target});
        auto begin_frame = std::chrono::system_clock::now();
        auto end_frame = begin_frame + frame_dt;

        while(!should_quit())
        {
        #ifdef __APPLE__
            // Wait for 1/6th of the frame dt for events otherwise the window stutters
            glfwWaitEventsTimeout(1.0f / (6 * fps_target)); 
        #else
            glfwPollEvents();
        #endif
            
            render();

            if (use_framecap)
            {
                std::this_thread::sleep_until(end_frame);
                begin_frame = end_frame;
                end_frame = begin_frame + frame_dt;
            }
        }

        terminate();
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
        end();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImPlot::DestroyContext();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        window = nullptr;

        glfwTerminate();
    }



}

#endif