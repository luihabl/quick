#pragma once

#include <imgui.h>

namespace quick
{
    class Application
    {

    public:
        bool setup();
        void run();
        void quit();
        void terminate();

    protected:        
        //Used to setup. Executed once before main loop.
        virtual void start() {};       

        //Used to draw ImGui stuff and other updates. 
        virtual void update() {};

        //Used for free OpenGL calls. Do not render ImGui here!
        virtual void draw() {};

    private:
        ImVec4 clear_color;
        bool m_quit = false;
        bool should_quit();

    };
}