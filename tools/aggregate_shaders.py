#!/usr/bin/env python3

import sys
import os

if len(sys.argv) != 3 and len(sys.argv) != 2:
    sys.stderr.write(f"[aggregate_shaders.py] ERROR: Incorrect number of arguments, got {len(sys.argv)}")
    exit(1)

file_name = 'all.h'
dir = sys.argv[1]
shader_dir = ''
if len(sys.argv) == 3:
    shader_dir = f'{sys.argv[2]}/'

files = os.listdir(dir)
if 'all.h' in files:
    files.remove('all.h')

mac = []
win = []
etc = []

for file in files:
    if 'mtl' in file:
        mac.append(file)
    elif 'dx9' in file or 'dx11' in file or 'dx12' in file:
        win.append(file)
    else:
        etc.append(file)

unique_shaders = list(dict.fromkeys(map(lambda fi: fi.split('.sc.')[0], files)))

with open(f'{dir}/{file_name}', "w") as f:
    for file in etc:
        f.write(f'#include <{shader_dir}{file}>\n')
    if win:
        f.write('#if defined(_WIN32)\n')
        for file in win:
            f.write(f'#include <{shader_dir}{file}>\n')
        f.write('#endif //  defined(_WIN32)')
    if mac:
        f.write('#if __APPLE__\n')
        for file in mac:
            f.write(f'#include <{shader_dir}{file}>\n')
        f.write('#endif // __APPLE__')
    f.write('\n\n#include <bgfx/embedded_shader.h>\n')
    f.write('const bgfx::EmbeddedShader embedded_shaders[] = {\n')
    for file in unique_shaders:
        f.write(f'\tBGFX_EMBEDDED_SHADER({file}),\n')
    f.write('\tBGFX_EMBEDDED_SHADER_END()\n};')
