
if (APPLE)
    set(apple_defs APPLE UNIX GLFW_EXPOSE_NATIVE_COCOA)
    set(apple_src_files window/metal_window.h window/metal_window.mm)
elseif (WIN32)
    set(windows_defs WIN32 GLFW_EXPOSE_NATIVE_WIN32)
    set(windows_includes ${CMAKE_SOURCE_DIR}/include/msvc)
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

target_link_libraries(life bgfx bx bimg glfw glm::glm imgui fmt::fmt EnTT::EnTT)
target_compile_definitions(life PRIVATE ${apple_defs} ${windows_defs})

target_include_directories(life PRIVATE
        ${windows_includes}
        ${imgui_dir}
        window
)

target_precompile_headers(life PRIVATE pch.h)