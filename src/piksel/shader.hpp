#ifndef SHADER_HPP
#define SHADER_HPP

#ifdef __EMSCRIPTEN__
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#endif
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include <map>

namespace piksel {

class Shader {
private:
    GLuint program;
    std::map<std::string, GLint> uniforms;

public:
    Shader() : program(0) {}
    void create(const std::string &vertexShaderSource, const std::string &fragmentShaderSource);
    void load(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename);
    void createUniform(const std::string &uniformName);
    void setUniform(const std::string &uniformName, const int &i);
    void setUniform(const std::string &uniformName, const float &f);
    void setUniform(const std::string &uniformName, const glm::vec2 &v);
    void setUniform(const std::string &uniformName, const glm::vec3 &v);
    void setUniform(const std::string &uniformName, const glm::vec4 &v);
    void setUniform(const std::string &uniformName, const glm::mat4 &m);
    void use();
};

} // namespace nv

#endif /* SHADER_HPP */
