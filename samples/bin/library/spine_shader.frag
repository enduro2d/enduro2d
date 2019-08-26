uniform sampler2D u_texture;

varying vec2 v_st0;
varying vec4 v_color0;

void main() {
    gl_FragColor = texture2D(u_texture, v_st0) * v_color0;
}
