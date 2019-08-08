uniform sampler2D u_texture;
uniform float u_char_width;
uniform float u_char_edge;
uniform float u_outline_width;
uniform float u_outline_edge;
uniform vec4 u_outline_color;
uniform float u_shadow_width;
uniform float u_shadow_edge;
uniform vec2 u_shadow_offset;
uniform vec4 u_shadow_color;

varying vec2 v_st0;
varying vec4 v_color;

void main() {
    float distance = 1.0 - texture2D(u_texture, v_st0).a;
    float alpha = 1.0 - smoothstep(u_char_width, u_char_width + u_char_edge, distance);

    float outline_alpha = 1.0 - smoothstep(u_outline_width, u_outline_width + u_outline_edge, distance);
    float overall_alpha = alpha + (1.0 - alpha) * outline_alpha;
    vec4 overall_color = mix(u_outline_color, v_color, alpha / overall_alpha);

    float distance_shadow = 1.0 - texture2D(u_texture, v_st0 + u_shadow_offset).a;
    float shadow_alpha = 1.0 - smoothstep(u_shadow_width, u_shadow_width + u_shadow_edge, distance_shadow);
    vec4  shadow = vec4(u_shadow_color.rgb, shadow_alpha);

    gl_FragColor = mix(shadow, overall_color, overall_alpha);
}
