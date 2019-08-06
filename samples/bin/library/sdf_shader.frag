uniform sampler2D u_texture;
uniform lowp float u_smoothing;
uniform lowp float u_outline_distance;// Between 0 and 0.5, 0 = thick outline, 0.5 = no outline
uniform lowp float u_shadow_smoothing;// Between 0 and 0.5
uniform lowp vec2 u_shadow_offset;    // Between 0 and spread / texture_size
uniform lowp vec4 u_shadow_color;
uniform lowp vec4 u_outline_color;

varying vec2 v_st0;
varying vec4 v_color;

void main() {
    lowp vec2  st = vec2(v_st0.s, 1.0 - v_st0.t);
    lowp float distance = texture2D(u_texture, st).a;
    lowp float outline_factor = smoothstep(0.5 - u_smoothing, 0.5 + u_smoothing, distance);
    lowp vec4  color = mix(u_outline_color, v_color, outline_factor);
    lowp float alpha = smoothstep(u_outline_distance - u_smoothing, u_outline_distance + u_smoothing, distance);
    lowp vec4  text = vec4(color.rgb, color.a * alpha);
    lowp float shadow_distance = texture2D(u_texture, st - u_shadow_offset).a;
    lowp float shadow_alpha = smoothstep(0.5 - u_shadow_smoothing, 0.5 + u_shadow_smoothing, shadow_distance);
    lowp vec4  shadow = vec4(u_shadow_color.rgb, u_shadow_color.a * shadow_alpha);

    gl_FragColor = mix(shadow, text, text.a);
}
