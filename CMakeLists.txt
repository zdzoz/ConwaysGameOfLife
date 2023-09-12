cmake_minimum_required(VERSION 3.26)
project(conways C CXX)

if (APPLE)
    enable_language(OBJC OBJCXX)
endif ()

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(fetchcontent)
set(imgui_dir ${CMAKE_BINARY_DIR}/_deps/imgui-src)
fetchcontent_declare(
        imgui
        GIT_REPOSITORY https://github.com/ocornut/imgui.git
        GIT_TAG v1.89.9-docking
        SOURCE_DIR ${imgui_dir}/imgui
)
fetchcontent_declare(
        glfw3
        GIT_REPOSITORY https://github.com/glfw/glfw.git
        GIT_TAG 3.3.8
        FIND_PACKAGE_ARGS
)
fetchcontent_declare(
        glm
        GIT_REPOSITORY https://github.com/g-truc/glm.git
        GIT_TAG 0.9.9.8
        FIND_PACKAGE_ARGS
)
fetchcontent_declare(
        fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG 10.1.1
        FIND_PACKAGE_ARGS
)
fetchcontent_declare(
        entt
        GIT_REPOSITORY https://github.com/skypjack/entt.git
        GIT_TAG v3.12.2
        FIND_PACKAGE_ARGS
)

# BIGG Stack
fetchcontent_makeavailable(imgui glfw3 glm fmt entt)

if (APPLE)
    set(imgui_apple_srcs ${imgui_SOURCE_DIR}/backends/imgui_impl_metal.*)
endif ()

file(GLOB imgui_srcs CONFIGURE_DEPENDS
        ${imgui_SOURCE_DIR}/*.cpp
        ${imgui_SOURCE_DIR}/*.h
        ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.*
        ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.*
        ${imgui_apple_srcs}
)
add_library(imgui ${imgui_srcs})
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})
target_link_libraries(imgui glfw)

add_subdirectory(vendor)
add_subdirectory(src)
