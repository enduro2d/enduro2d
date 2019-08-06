uniform sampler2D u_texture;

varying vec2 v_st0;
varying vec4 v_color;

const float smoothing = 1.0 / 16.0;

void main() {
    vec2 st = vec2(v_st0.s, 1.0 - v_st0.t);
    gl_FragColor = texture2D(u_texture, st) * v_color;
    float distance = texture2D(u_texture, st).a;
    float alpha = smoothstep(0.5 - smoothing, 0.5 + smoothing, distance);
    gl_FragColor = vec4(v_color.rgb, v_color.a * alpha);
}
