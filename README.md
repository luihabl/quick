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
