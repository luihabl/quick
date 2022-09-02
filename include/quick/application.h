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

        ImVec4 clear_color;

        //Used to draw ImGui stuff and other updates. 
        virtual void update() {};

        //Used for free OpenGL calls. Do not render ImGui here!
        virtual void draw() {};

    private:
        bool m_quit = false;
        bool should_quit();

    };
}