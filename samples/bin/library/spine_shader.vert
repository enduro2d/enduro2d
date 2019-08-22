uniform mat4 u_matrix_vp;

attribute vec3 a_vertex;
attribute vec2 a_st0;
attribute vec4 a_color0;

varying vec2 v_st0;
varying vec4 v_color0;

vec4 vertex_to_homo(vec3 pos) {
    return vec4(pos, 1.0) * u_matrix_vp;
}

void main() {
    v_st0 = a_st0;
    v_color0 = a_color0;
    gl_Position = vertex_to_homo(a_vertex);
}
