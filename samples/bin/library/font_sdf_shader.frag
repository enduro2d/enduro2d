uniform sampler2D u_texture;

uniform float u_glyph_dilate;
uniform float u_outline_width;
uniform vec4 u_outline_color;

varying vec2 v_st0;
varying vec4 v_color;

void main() {
    float distance = texture2D(u_texture, v_st0).a;
    float width = fwidth(distance);

    float glyph_center = 0.5 - u_glyph_dilate * 0.5;
    float outline_center = glyph_center - u_outline_width * 0.5;

    float glyph_alpha = smoothstep(glyph_center - width, glyph_center + width, distance);
    float outline_alpha = smoothstep(outline_center - width, outline_center + width, distance);

    vec4 glyph_color = vec4(v_color.rgb * v_color.a, v_color.a);
    vec4 outline_color = vec4(u_outline_color.rgb * u_outline_color.a, u_outline_color.a);

    vec2 layers_mask = vec2(
        step(0.01, v_color.a),
        step(0.01, u_outline_color.a * u_outline_width));

    gl_FragColor = glyph_alpha * glyph_color * layers_mask.x +
        outline_alpha * outline_color * layers_mask.y * (1.0 - glyph_alpha);
}
