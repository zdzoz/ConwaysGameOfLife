
set(verts vs_texture.sc)
set(frags fs_texture.sc)

set(shader_out ${CMAKE_BINARY_DIR}/include/generated)
bgfx_compile_shader_to_header(
        TYPE VERTEX
        SHADERS ${verts}
#        VARYING_DEF varying.def.sc
        OUTPUT_DIR ${shader_out}/shaders
)

bgfx_compile_shader_to_header(
        TYPE FRAGMENT
        SHADERS ${frags}
#        VARYING_DEF varying.def.sc
        OUTPUT_DIR ${shader_out}/shaders
)

add_library(shader_lib INTERFACE ${verts} ${frags})

# generate all.h file
add_custom_target(create_all
        COMMAND python3 ${CMAKE_SOURCE_DIR}/tools/aggregate_shaders.py ${shader_out}/shaders shaders
        DEPENDS shader_lib ${verts} ${frags}
        VERBATIM
)

target_include_directories(life PRIVATE ${shader_out})
add_dependencies(life shader_lib create_all)
