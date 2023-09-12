
if (APPLE)
    set(apple_defs APPLE UNIX GLFW_EXPOSE_NATIVE_COCOA)
    set(apple_src_files window/metal_window.h window/metal_window.mm)
elseif (WIN32)
    set(windows_defs WIN32 GLFW_EXPOSE_NATIVE_WIN32)
endif()

add_executable(life
        main.cpp
        components.h
        window/window.h window/window.cpp
        game.h game.cpp
        shader.h shader.cpp
        texture.h texture.cpp
        ${apple_src_files}
)

add_subdirectory(shaders)

target_link_libraries(life bgfx glfw glm::glm imgui fmt::fmt EnTT::EnTT)
target_compile_definitions(life PRIVATE ${apple_defs} ${windows_defs})

target_include_directories(life PRIVATE ${imgui_dir} window ${bgfx_SOURCE_DIR}/bgfx/include ${bgfx_SOURCE_DIR}/bx/include ${bgfx_SOURCE_DIR}/bimg/include)

target_precompile_headers(life PRIVATE pch.h)