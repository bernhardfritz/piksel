#include "shader.hpp"
#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <sstream>

namespace piksel {

std::string readFile(const std::string& filename) {
    std::ifstream ifs(filename.c_str());
    std::stringstream ss;
    ss << ifs.rdbuf();
    return ss.str();
}

GLuint compile(GLenum shaderType, const std::string &shaderSource) {
    GLuint shader = glCreateShader(shaderType);
    const GLchar* c_str = shaderSource.c_str();
    glShaderSource(shader, 1, &c_str, NULL);
    glCompileShader(shader);

    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE) {
        GLint infoLogLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar infoLog[infoLogLength];
        glGetShaderInfoLog(shader, infoLogLength, &infoLogLength, infoLog);
        
        glDeleteShader(shader);

        fprintf(stderr, "Shader compilation failed: %s\n", infoLog);

        return 0;
    }

    return shader;
}

GLuint link(GLuint vertexShader, GLuint fragmentShader) {
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    GLint linkStatus;
    glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
    if(linkStatus == GL_FALSE) {
        GLint infoLogLength = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

        GLchar infoLog[infoLogLength];
        glGetProgramInfoLog(program, infoLogLength, &infoLogLength, infoLog);

        glDeleteProgram(program);

        fprintf(stderr, "Program linking failed: %s\n", infoLog);

        return 0;
    }

    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);

    return program;
}

void Shader::create(const std::string &vertexShaderSource, const std::string &fragmentShaderSource) {
    GLuint vertexShader;
    if ((vertexShader = compile(GL_VERTEX_SHADER, vertexShaderSource)) > 0) {
        GLuint fragmentShader;
        if ((fragmentShader = compile(GL_FRAGMENT_SHADER, fragmentShaderSource)) > 0) {
            program = link(vertexShader, fragmentShader);
        }
    }
}

void Shader::load(const std::string &vertexShaderFilename, const std::string &fragmentShaderFilename) {
    create(readFile(vertexShaderFilename), readFile(fragmentShaderFilename));
}

void Shader::createUniform(const std::string &uniformName) {
    uniforms[uniformName] = glGetUniformLocation(program, uniformName.c_str());
}

void Shader::setUniform(const std::string &uniformName, const int &i) {
    glUniform1iv(uniforms[uniformName], 1, &i);
}

void Shader::setUniform(const std::string &uniformName, const float &f) {
    glUniform1fv(uniforms[uniformName], 1, &f);
}

void Shader::setUniform(const std::string &uniformName, const glm::vec2 &v) {
    glUniform2fv(uniforms[uniformName], 1, glm::value_ptr(v));
}

void Shader::setUniform(const std::string &uniformName, const glm::vec3 &v) {
    glUniform3fv(uniforms[uniformName], 1, glm::value_ptr(v));
}

void Shader::setUniform(const std::string &uniformName, const glm::vec4 &v) {
    glUniform4fv(uniforms[uniformName], 1, glm::value_ptr(v));
}

void Shader::setUniform(const std::string &uniformName, const glm::mat4 &m) {
    glUniformMatrix4fv(uniforms[uniformName], 1, GL_FALSE, glm::value_ptr(m));
}

void Shader::use() {
    if (program) {
        glUseProgram(program);
    }
}

} // namespace nv
