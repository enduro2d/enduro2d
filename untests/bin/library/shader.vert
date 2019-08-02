attribute vec3 a_position;
attribute vec2 a_st;

uniform mat4 u_MVP;
varying vec2 v_st;

void main() {
    v_st = a_st;
    gl_Position = vec4(a_position, 1.0) * u_MVP;
}
