# quick
Quick and simple C++ GUIs using Dear ImGui and ImPlot ✨.

## Dependencies

- [GLFW](https://github.com/glfw/glfw) or [SDL2](https://github.com/libsdl-org/SDL) for windowing and inputs
- OpenGL 3.3 for rendering
- [Dear ImGui](https://github.com/ocornut/imgui) for GUI
- [ImPlot](https://github.com/epezent/implot) for plotting

## Building

The library is built using CMake (>3.14). All dependencies are downloaded automatically using [CMake FetchContent](https://cmake.org/cmake/help/latest/module/FetchContent.html) so no need of downloading and building the dependencies manually. 

To create a new project using the library simply do

```cmake 
add_subdirectory(path/to/quick)

(...)

target_link_libraries(<target_name> quick)
```

## Usage

```cpp

#include <quick.h>

class MyApp : public quick::Application
{
    void start(const Config& config)
    {
        // Setup app (example)
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

        ImGui::StyleColorsClassic();
    }
    
    void update()
    {
        // Draw ImGui elements (example)
        ImGui::ShowDemoWindow();
    }

};

int main()
{
    auto app = MyApp();
    
    auto config = quick::Application::Config();
    config.name = "MyApp title";
    config.w = 1920;
    config.h = 1080;

    if(!app.setup(config))
    {
        return 1;
    }
    app.run();
    app.terminate();  
        
    return 0;
}


```
