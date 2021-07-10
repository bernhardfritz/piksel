// Main fragment shader source
// Included in internals.hpp. Relies on existance of std::string glsl_version.
const std::string FRAGMENT_SHADER_SOURCE = glsl_version + R"(
#ifdef GL_ES
precision lowp float;
#endif
#define THICKNESS 0.0 // 0.015 // -1 to +1

in vec2 v_uv;
flat in vec4 v_fillColor;
flat in vec4 v_strokeColor;
flat in vec4 v_tintColor;
flat in float v_strokeWeight;
flat in float v_textureIndex;
uniform sampler2D u_textures[8];
uniform vec2 u_resolution;
uniform float pxRange;
uniform vec2 msdfSize;
out vec4 fragColor;

vec4 getSampleFromArray(sampler2D textures[8], int i, vec2 uv) {
    if (i == 0) {
        return texture(textures[0], uv);
    } else if (i == 1) {
        return texture(textures[1], uv);
    } else if (i == 2) {
        return texture(textures[2], uv);
    } else if (i == 3) {
        return texture(textures[3], uv);
    } else if (i == 4) {
        return texture(textures[4], uv);
    } else if (i == 5) {
        return texture(textures[5], uv);
    } else if (i == 6) {
        return texture(textures[6], uv);
    } else {
        return texture(textures[7], uv);
    }
}

float median(float r, float g, float b) {
    return max(min(r, g), min(max(r, g), b));
}

float linearstep(float a, float b, float x) {
    return clamp((x - a) / (b - a), 0.0, 1.0);
}

void main() {
    vec4 color;

    int textureIndex = int(v_textureIndex + 0.5);
    if (v_textureIndex < 0.0) {
        color = v_fillColor;
    } else if (textureIndex == 0) {
        vec2 msdfUnit = pxRange / msdfSize;
        vec3 smpl = texture(u_textures[0], v_uv).rgb;
        float m = median(smpl.r, smpl.g, smpl.b);
        float pxSize = min(0.5/pxRange*(fwidth(v_uv.x)*msdfSize.x+fwidth(v_uv.y)*msdfSize.y), 0.25);
        float sd = 2.0*m-1.0;
        float inside = linearstep(-v_strokeWeight-pxSize, -v_strokeWeight+pxSize, sd);
        float outsideBorder = v_strokeWeight > 0.0 ? linearstep(+v_strokeWeight-pxSize, +v_strokeWeight+pxSize, sd) : 1.0;
        vec4 fg = mix(v_strokeColor, v_fillColor, outsideBorder);
        color = mix(vec4(fg.rgb, 0.0), fg, inside);
    } else {
        color = getSampleFromArray(u_textures, textureIndex, v_uv) * v_tintColor;
    }

    fragColor = color;
}

)";