uniform mat4 u_matrix_m;
uniform mat4 u_matrix_vp;

attribute vec3 a_vertex;
attribute vec2 a_st0;

varying vec2 v_st0;

void main() {
    v_st0 = a_st0;
    gl_Position = vec4(a_vertex, 1.0) * u_matrix_m * u_matrix_vp;
}
