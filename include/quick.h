#pragma once

#include <glad/glad.h>
#include <imgui.h>

#ifdef QUICK_PLATFORM_GLFW

#include <GLFW/glfw3.h>

    #ifdef _WIN32
    #undef APIENTRY
    #define GLFW_EXPOSE_NATIVE_WIN32
    #include <GLFW/glfw3native.h>   // for glfwGetWin32Window()
    #endif
    #ifdef __APPLE__
    #define GLFW_EXPOSE_NATIVE_COCOA
    #include <GLFW/glfw3native.h>   // for glfwGetCocoaWindow()
    #endif

#endif // QUICK_PLATFORM_GLFW

#ifdef QUICK_PLATFORM_SDL2

    #include <SDL.h>

#endif //QUICK_PLATFORM_SDL2





#include "quick/application.h"