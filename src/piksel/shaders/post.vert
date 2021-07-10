// Postfix vertex shader source
// Included in internals.hpp. Relies on existance of std::string glsl_version.
const std::string POSTFX_VERTEX_SHADER_SOURCE = glsl_version + R"(

#ifdef GL_ES
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