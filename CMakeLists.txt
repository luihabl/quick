cmake_minimum_required(VERSION 3.14)

project(quick)

include(FetchContent)

# C++ version ===========================================================================

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# External libs =========================================================================

#Platform backend options
option(QUICK_PLATFORM_GLFW "Use GLFW platform backend" ON)
option(QUICK_PLATFORM_SDL2 "Use SDL2 platform backend" OFF)

set(QUICK_LIBS "")
set(QUICK_SOURCE "")
set(QUICK_INCLUDES "")
set(QUICK_DEFINITIONS "")

set(FETCHCONTENT_QUIET FALSE)


#Dear ImGui
FetchContent_Declare(
    imgui
    GIT_REPOSITORY    https://github.com/ocornut/imgui.git
    GIT_TAG           origin/docking
)
FetchContent_MakeAvailable(imgui)
set(QUICK_INCLUDES ${QUICK_INCLUDES} 
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends    
)
set(QUICK_SOURCE ${QUICK_SOURCE}
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
)

if(QUICK_PLATFORM_GLFW)
    set(QUICK_SOURCE ${QUICK_SOURCE}
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
    )
elseif(QUICK_PLATFORM_SDL2)
    set(QUICK_SOURCE ${QUICK_SOURCE}
        ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl2.cpp
    )
endif()


#ImPlot
FetchContent_Declare(
    implot
    GIT_REPOSITORY    https://github.com/epezent/implot.git
    GIT_TAG           origin/master
)
FetchContent_MakeAvailable(implot)
set(QUICK_INCLUDES ${QUICK_INCLUDES} 
    ${implot_SOURCE_DIR}
)
set(QUICK_SOURCE ${QUICK_SOURCE}
    ${implot_SOURCE_DIR}/implot.cpp
    ${implot_SOURCE_DIR}/implot_demo.cpp
    ${implot_SOURCE_DIR}/implot_items.cpp
)


#Glad
set(QUICK_SOURCE ${QUICK_SOURCE}
    external/glad/src/glad.c    
)
set(QUICK_INCLUDES ${QUICK_INCLUDES} 
    external/glad/include
)


#GLFW
if(QUICK_PLATFORM_GLFW)

    FetchContent_Declare(
        glfw
        GIT_REPOSITORY    https://github.com/glfw/glfw.git
        GIT_TAG           3.3.8
    )
    set(GLFW_BUILD_EXAMPLES OFF CACHE INTERNAL "Build the GLFW example programs")
    set(GLFW_BUILD_TESTS OFF CACHE INTERNAL "Build the GLFW test programs")
    set(GLFW_BUILD_DOCS OFF CACHE INTERNAL "Build the GLFW documentation")
    set(GLFW_INSTALL OFF CACHE INTERNAL "Generate installation target")
    FetchContent_MakeAvailable(glfw)
    
    set(QUICK_LIBS ${QUICK_LIBS} glfw)

    set(QUICK_SOURCE ${QUICK_SOURCE}
        src/application_glfw.cpp
    )

    set(QUICK_DEFINITIONS ${QUICK_DEFINITIONS} QUICK_PLATFORM_GLFW)

#SDL2
elseif(QUICK_PLATFORM_SDL2)

    FetchContent_Declare(
				sdl2
				GIT_REPOSITORY https://github.com/libsdl-org/SDL
				GIT_TAG release-2.24.0 # grab latest stable release
				GIT_PROGRESS TRUE
			)
    set(SDL_TEST_ENABLED_BY_DEFAULT OFF CACHE BOOL "Build the SDL2_test library by default" FORCE)
    set(SDL_TEST OFF CACHE BOOL "Build the SDL2_test library" FORCE)
    FetchContent_MakeAvailable(sdl2)

    set(QUICK_LIBS ${QUICK_LIBS} SDL2main SDL2-static)

    set(QUICK_SOURCE ${QUICK_SOURCE}
        src/application_sdl2.cpp
    )

    set(QUICK_DEFINITIONS ${QUICK_DEFINITIONS} QUICK_PLATFORM_SDL2)


endif()


# Source code ===========================================================================

#Main code
add_library(quick)

target_link_libraries(quick 
    PUBLIC 
    ${QUICK_LIBS}
)

target_include_directories(quick
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    PRIVATE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
)

#External code
target_include_directories(quick
    PUBLIC
    ${QUICK_INCLUDES}
)

target_sources(quick PRIVATE
    ${QUICK_SOURCE}
)

target_compile_definitions(quick PUBLIC 
    ${QUICK_DEFINITIONS}
)