// Main fragment vertex source
// Included in internals.hpp. Relies on existance of std::string glsl_version.
const std::string VERTEX_SHADER_SOURCE = glsl_version + R"(

#ifdef GL_ES
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