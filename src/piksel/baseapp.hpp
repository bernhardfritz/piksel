#ifndef BASEAPP_HPP
#define BASEAPP_HPP

#include "graphics.hpp"
#include "shader.hpp"
#include "state.hpp"
#include <stack>
#include <string>
#include <vector>

namespace piksel {

class BaseApp {
public:
    BaseApp(int width, int height, std::string title)
        : width(width)
        , height(height)
        , title(title)
        , sboSize(1024 * sizeof(State))
        , postfx_texture(0)
        , devicePixelRatio(1.0f) {
    }
    BaseApp(int width, int height) : BaseApp(width, height, "piksel") {
    }
    BaseApp() : BaseApp(100, 100) {
    }
    virtual ~BaseApp();
    virtual void draw(Graphics& g);
    virtual void keyPressed(int key);
    virtual void keyReleased(int key);
    int millis();
    virtual void mouseMoved(int x, int y);
    virtual void mousePressed(int button);
    virtual void mouseReleased(int button);
    virtual void mouseWheel(int delta);
    virtual void setup();
    void start();
protected:
    int width;
    int height;
private:
    std::string title;
    GLFWwindow* window;
    int framebufferWidth, framebufferHeight;
    glm::mat4 projectionMatrix;
    GLuint vao, sbo;
    GLsizeiptr sboSize;
    Shader shader;
    GLuint postfx_framebuffer;
    GLuint postfx_texture;
    Shader postfx_shader;
    GLuint postfx_vao;
    std::stack<State> stateStack;
    std::vector<ShaderRelevantState> shaderRelevantStateVector;
    std::vector<Shape> shapes;
    float devicePixelRatio;

    void mainLoop(Graphics& g);
    void updateFramebufferSize(int framebufferWidth, int framebufferHeight);
    void framebufferSizeCallback(int framebufferWidth, int framebufferHeight);
    void keyCallback(int key, int scancode, int action, int mods);
    void cursorPosCallback(double xpos, double ypos);
    void mouseButtonCallback(int button, int action, int mods);
    void scrollCallback(double xoffset, double yoffset);
    void fullscreenchangeCallback(bool isFullscreen);
};

} // namespace nv

#endif /* BASEAPP_HPP */
