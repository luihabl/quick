#pragma once

#include <imgui.h>
#include <string>
#include <memory>

namespace quick
{
    class Application
    {

    public:

        struct Config
        {
            int w = 1920; // Default width
            int h = 1080; // Default height

            std::string name;

            ImVec4 clear_color;

            bool use_vsync = true;
            bool use_framecap = false;
            float fps_target = 60.0f; 

            // Not available for SDL2 backend
            bool render_on_resize = false;
        };

        bool setup(const Config& config);
        void run();
        void quit();
        void render();
        
        Application();
        ~Application();

    protected:        
        //Used to setup. Executed once before main loop.
        virtual void start([[maybe_unused]] const Config& config) {};       

        //Used to draw ImGui stuff and other updates. 
        virtual void update() {};

        //Used for free OpenGL calls. Do not render ImGui here!
        virtual void draw() {};

        //Used to dispose and finish stuff. Called after leaving the main loop.
        virtual void end() {};

    private:
        bool use_framecap = false;
        float fps_target = 60.0f;
    
        ImVec4 clear_color;
        bool m_quit = false;
        bool should_quit();
        void terminate();

        struct Impl;  
        std::unique_ptr<Impl> impl;
    };
}