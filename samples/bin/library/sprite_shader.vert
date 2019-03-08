#version 120

uniform mat4 u_matrix_vp;

attribute vec3 a_vertex;
attribute vec4 a_tint;
attribute vec2 a_st;

varying vec4 v_tint;
varying vec2 v_st;

void main() {
    v_st = a_st;
    v_tint = a_tint;
    gl_Position = vec4(a_vertex, 1.0) * u_matrix_vp;
}
