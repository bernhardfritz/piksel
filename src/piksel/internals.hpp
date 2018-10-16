#ifndef INTERNALS_HPP
#define INTERNALS_HPP

#include <string>

namespace piksel {

extern const bool USE_EMSCRIPTEN;

const std::string VERTEX_SHADER_SOURCE = std::string(USE_EMSCRIPTEN ? "#version 300 es\n" : "#version 330 core\n") +
R"(#ifdef GL_ES
precision lowp float;
#endif
layout (location = 0) in vec2 a_position;
layout (location = 1) in vec4 a_fillColor;
layout (location = 2) in vec4 a_strokeColor;
layout (location = 3) in vec4 a_tintColor;
layout (location = 4) in float a_strokeWeight;
layout (location = 5) in float a_textureIndex;
layout (location = 6) in mat4 a_modelMatrix;
layout (location = 10) in mat4 a_textureMatrix;
uniform mat4 u_projectionMatrix;
out vec2 v_uv;
flat out vec4 v_fillColor;
flat out vec4 v_strokeColor;
flat out vec4 v_tintColor;
flat out float v_strokeWeight;
flat out float v_textureIndex;

void main() {
    v_uv = vec2(a_textureMatrix * vec4(a_position.x, 1.0 - a_position.y, 0.0, 1.0));
    v_fillColor = a_fillColor;
    v_strokeColor = a_strokeColor;
    v_tintColor = a_tintColor;
    v_strokeWeight = a_strokeWeight;
    v_textureIndex = a_textureIndex;
    mat4 mvp = u_projectionMatrix * a_modelMatrix;
    gl_Position = mvp * vec4(a_position, 0.0, 1.0);
}
)";

const std::string FRAGMENT_SHADER_SOURCE = std::string(USE_EMSCRIPTEN ? "#version 300 es\n" : "#version 330 core\n") +
R"(#ifdef GL_ES
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
    if (textureIndex == -1) {
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

const std::string POSTFX_VERTEX_SHADER_SOURCE = std::string(USE_EMSCRIPTEN ? "#version 300 es\n" : "#version 330 core\n") +
R"(#ifdef GL_ES
precision mediump float;
#endif
layout (location = 0) in vec2 a_position;
uniform vec2 u_resolution;
out vec2 v_uv;
out vec2 v_rgbNW;
out vec2 v_rgbNE;
out vec2 v_rgbSW;
out vec2 v_rgbSE;
out vec2 v_rgbM;

void texcoords(vec2 fragCoord, vec2 resolution,
			out vec2 v_rgbNW, out vec2 v_rgbNE,
			out vec2 v_rgbSW, out vec2 v_rgbSE,
			out vec2 v_rgbM) {
	vec2 inverseVP = 1.0 / resolution.xy;
	v_rgbNW = (fragCoord + vec2(-1.0, -1.0)) * inverseVP;
	v_rgbNE = (fragCoord + vec2(1.0, -1.0)) * inverseVP;
	v_rgbSW = (fragCoord + vec2(-1.0, 1.0)) * inverseVP;
	v_rgbSE = (fragCoord + vec2(1.0, 1.0)) * inverseVP;
	v_rgbM = vec2(fragCoord * inverseVP);
}

void main() {
    v_uv = (a_position + 1.0) * 0.5;
    vec2 fragCoord = v_uv * u_resolution;
    texcoords(fragCoord, u_resolution, v_rgbNW, v_rgbNE, v_rgbSW, v_rgbSE, v_rgbM);
    gl_Position = vec4(a_position, 0.0, 1.0);
}
)";

const std::string POSTFX_FRAGMENT_SHADER_SOURCE = std::string(USE_EMSCRIPTEN ? "#version 300 es\n" : "#version 330 core\n") +
R"(#ifdef GL_ES
precision mediump float;
#endif
#ifndef FXAA_REDUCE_MIN
    #define FXAA_REDUCE_MIN   (1.0/ 128.0)
#endif
#ifndef FXAA_REDUCE_MUL
    #define FXAA_REDUCE_MUL   (1.0 / 8.0)
#endif
#ifndef FXAA_SPAN_MAX
    #define FXAA_SPAN_MAX     8.0
#endif
in vec2 v_uv;
in vec2 v_rgbNW;
in vec2 v_rgbNE;
in vec2 v_rgbSW;
in vec2 v_rgbSE;
in vec2 v_rgbM;
uniform sampler2D u_texture;
uniform vec2 u_resolution;
out vec4 fragColor;

vec4 fxaa(sampler2D tex, vec2 fragCoord, vec2 resolution,
            vec2 v_rgbNW, vec2 v_rgbNE, 
            vec2 v_rgbSW, vec2 v_rgbSE, 
            vec2 v_rgbM) {
    vec4 color;
    mediump vec2 inverseVP = vec2(1.0 / resolution.x, 1.0 / resolution.y);
    vec3 rgbNW = texture(tex, v_rgbNW).xyz;
    vec3 rgbNE = texture(tex, v_rgbNE).xyz;
    vec3 rgbSW = texture(tex, v_rgbSW).xyz;
    vec3 rgbSE = texture(tex, v_rgbSE).xyz;
    vec4 texColor = texture(tex, v_rgbM);
    vec3 rgbM  = texColor.xyz;
    vec3 luma = vec3(0.299, 0.587, 0.114);
    float lumaNW = dot(rgbNW, luma);
    float lumaNE = dot(rgbNE, luma);
    float lumaSW = dot(rgbSW, luma);
    float lumaSE = dot(rgbSE, luma);
    float lumaM  = dot(rgbM,  luma);
    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
    
    mediump vec2 dir;
    dir.x = -((lumaNW + lumaNE) - (lumaSW + lumaSE));
    dir.y =  ((lumaNW + lumaSW) - (lumaNE + lumaSE));
    
    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) *
                          (0.25 * FXAA_REDUCE_MUL), FXAA_REDUCE_MIN);
    
    float rcpDirMin = 1.0 / (min(abs(dir.x), abs(dir.y)) + dirReduce);
    dir = min(vec2(FXAA_SPAN_MAX, FXAA_SPAN_MAX),
              max(vec2(-FXAA_SPAN_MAX, -FXAA_SPAN_MAX),
              dir * rcpDirMin)) * inverseVP;
    
    vec3 rgbA = 0.5 * (
        texture(tex, fragCoord * inverseVP + dir * (1.0 / 3.0 - 0.5)).xyz +
        texture(tex, fragCoord * inverseVP + dir * (2.0 / 3.0 - 0.5)).xyz);
    vec3 rgbB = rgbA * 0.5 + 0.25 * (
        texture(tex, fragCoord * inverseVP + dir * -0.5).xyz +
        texture(tex, fragCoord * inverseVP + dir * 0.5).xyz);

    float lumaB = dot(rgbB, luma);
    if ((lumaB < lumaMin) || (lumaB > lumaMax))
        color = vec4(rgbA, texColor.a);
    else
        color = vec4(rgbB, texColor.a);
    return color;
}

void main() {
    mediump vec2 fragCoord = v_uv * u_resolution;
    fragColor = fxaa(u_texture, fragCoord, u_resolution, v_rgbNW, v_rgbNE, v_rgbSW, v_rgbSE, v_rgbM);
}
)";

} // namespace nv

#endif /* INTERNALS_HPP */
