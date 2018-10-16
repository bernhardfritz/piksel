#ifndef IMAGE_HPP
#define IMAGE_HPP

#ifdef __EMSCRIPTEN__
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#endif
#include <string>

namespace piksel {

class Image {
public:
    GLuint _texture = 0;
    int width = 1, height = 1, _index = -1;

    int load(std::string filename);
};

} // namespace nv

#endif /* IMAGE_HPP */
