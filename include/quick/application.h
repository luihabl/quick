#pragma once

#include <imgui.h>
#include <string>

namespace quick
{
    class Application
    {

    public:

        struct Config
        {
            int w = 1920;
            int h = 1080;

            std::string name;

            ImVec4 clear_color;

            bool use_vsync = true;
            bool use_framecap = false;
            float fps_target = 60.0f; 
        };




        bool setup(const Config& config);
        void run();
        void quit();
        void terminate();

    protected:        
        //Used to setup. Executed once before main loop.
        virtual void start([[maybe_unused]] const Config& config) {};       

        //Used to draw ImGui stuff and other updates. 
        virtual void update() {};

        //Used for free OpenGL calls. Do not render ImGui here!
        virtual void draw() {};

    private:
        bool use_framecap = false;
        float fps_target = 60.0f;
    
        ImVec4 clear_color;
        bool m_quit = false;
        bool should_quit();

    };
}