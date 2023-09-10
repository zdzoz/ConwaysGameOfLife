$input a_position, a_color0
$output v_color0

uniform mat4 uModelView;
//uniform vec2 uPosition;
//uniform vec4 uColor;

#include "common.sh"

void main() {
    gl_Position = mul(uModelView, vec4(a_position, 0.0, 1.0));
    v_color0 = a_color0.abgr;
}