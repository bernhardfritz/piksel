#include "baseapp.hpp"
#include "internals.hpp"
#include <glm/gtc/matrix_transform.hpp>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_FAILURE_USERMSG
#include <stb/stb_image.h>
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>
#include <cpp-base64/base64.h>
#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#include <emscripten/html5.h>
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <chrono>
#endif /* __EMSCRIPTEN__*/
#include <utility>

// TODO: fps is currently hardcoded.
// Unfortunately vsync is not available on all platforms,
// therefore a fixed value seems to be the best solution for now.
// Find a way to make target fps configureable, otherwise render as
// fast as you can and provide a new function e.g. update(double dt)
// that allows to render consistently.
#define FRAMES_PER_SECOND 60.0
#define SECONDS_PER_FRAME 1.0 / FRAMES_PER_SECOND

#define FXAA 0

namespace piksel {

Font minecraft_regular;

void errorCallback(int error, const char* description) {
    fprintf(stderr, "Error: %s\n", description);
}

// PUBLIC

BaseApp::~BaseApp() {}
void BaseApp::draw(Graphics& g) {}
void BaseApp::keyPressed(int key) {}
void BaseApp::keyReleased(int key) {}
int BaseApp::millis() {
    return glfwGetTime() * 1000;
}
void BaseApp::mouseMoved(int x, int y) {}
void BaseApp::mousePressed(int button) {}
void BaseApp::mouseReleased(int button) {}
void BaseApp::mouseWheel(int delta) {}
void BaseApp::setup() {}
void BaseApp::start() {
    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        fputs("Failed to initialize GLFW", stderr);
#ifdef __EMSCRIPTEN__
        emscripten_force_exit(EXIT_FAILURE);
#else
        exit(EXIT_FAILURE);
#endif /* __EMSCRIPTEN__*/
    }

    GLFWmonitor* primaryMonitor = nullptr;
#ifndef __EMSCRIPTEN__
    if (fullscreen) {
        primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
        if (width == -1) {
            width = mode->width;
        }
        if (height == -1) {
            height = mode->height;
        }
    }
#endif /* __EMSCRIPTEN__*/

    // glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_SAMPLES, 0); // disable msaa
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
#ifdef __EMSCRIPTEN__
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    devicePixelRatio = emscripten_get_device_pixel_ratio();
#else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif /* __EMSCRIPTEN__ */

#ifdef __EMSCRIPTEN__
    window = glfwCreateWindow(width * devicePixelRatio, height * devicePixelRatio, title.c_str(), primaryMonitor, NULL);
#else
    window = glfwCreateWindow(width, height, title.c_str(), primaryMonitor, NULL);
#endif /* __EMSCRIPTEN__*/

    if (!window) {
        fputs("Failed to create GLFW window", stderr);
        glfwTerminate();
#ifdef __EMSCRIPTEN__
        emscripten_force_exit(EXIT_FAILURE);
#else
        exit(EXIT_FAILURE);
#endif /* __EMSCRIPTEN__*/
    }

#ifdef __EMSCRIPTEN__
    EM_ASM(
        var canvas = document.getElementById('canvas');
        canvas.style.width = (canvas.width / window.devicePixelRatio) + 'px';
        canvas.style.height = (canvas.height / window.devicePixelRatio) + 'px';
    );
#endif /* __EMSCRIPTEN__*/

    glfwMakeContextCurrent(window);
#ifndef __EMSCRIPTEN__
    gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
#endif /* __EMSCRIPTEN__*/
    // TODO: vsync would be preferable, however it is not available on all platforms
    // glfwSwapInterval(0); // disable vsync
    glfwSwapInterval(1); // TODO: use vsync if available, otherwise use hardcoded fps.
    glfwSetWindowUserPointer(window, this);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    int framebufferWidth, framebufferHeight;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    updateFramebufferSize(framebufferWidth, framebufferHeight);

    auto _keyCallback = [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        ((BaseApp*) glfwGetWindowUserPointer(window))->keyCallback(key, scancode, action, mods);
    };
    glfwSetKeyCallback(window, _keyCallback);
    auto _cursorPosCallback = [](GLFWwindow* window, double xpos, double ypos) {
        ((BaseApp*) glfwGetWindowUserPointer(window))->cursorPosCallback(xpos, ypos);
    };
    glfwSetCursorPosCallback(window, _cursorPosCallback);
    auto _mouseButtonCallback = [](GLFWwindow* window, int button, int action, int mods) {
        ((BaseApp*) glfwGetWindowUserPointer(window))->mouseButtonCallback(button, action, mods);
    };
    glfwSetMouseButtonCallback(window, _mouseButtonCallback); 
    auto _scrollCallback = [](GLFWwindow* window, double xoffset, double yoffset) {
        ((BaseApp*) glfwGetWindowUserPointer(window))->scrollCallback(xoffset, yoffset);
    };
    glfwSetScrollCallback(window, _scrollCallback);
#ifdef __EMSCRIPTEN__
    auto _fullscreenchangeCallback = [](int eventType, const EmscriptenFullscreenChangeEvent *fullscreenChangeEvent, void *userData) {
        ((BaseApp*) userData)->fullscreenchangeCallback(fullscreenChangeEvent->isFullscreen);
        return 1;
    };
    emscripten_set_fullscreenchange_callback("#document", this, true, _fullscreenchangeCallback);
#endif /* __EMSCRIPTEN__*/
    auto _framebufferSizeCallback = [](GLFWwindow* window, int framebufferWidth, int framebufferHeight) {
        ((BaseApp*) glfwGetWindowUserPointer(window))->framebufferSizeCallback(framebufferWidth, framebufferHeight);
    };
    glfwSetFramebufferSizeCallback(window, _framebufferSizeCallback);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    struct {
        float x, y;
    } vertices[4] = {
        { 0.0f, 0.0f },
        { 0.0f, 1.0f },
        { 1.0f, 0.0f },
        { 1.0f, 1.0f }
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    GLuint a_position = 0;
    glEnableVertexAttribArray(a_position);
    glVertexAttribPointer(a_position, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);
    glVertexAttribDivisor(a_position, 0);

    glGenBuffers(1, &sbo);
    glBindBuffer(GL_ARRAY_BUFFER, sbo);
    glBufferData(GL_ARRAY_BUFFER, sboSize, nullptr, GL_DYNAMIC_DRAW);

    GLuint a_fillColor = 1;
    glEnableVertexAttribArray(a_fillColor);
    glVertexAttribPointer(a_fillColor, 4, GL_FLOAT, GL_FALSE, sizeof(ShaderRelevantState), (void*) offsetof(ShaderRelevantState, fillColor));
    glVertexAttribDivisor(a_fillColor, 1);
    
    GLuint a_strokeColor = 2;
    glEnableVertexAttribArray(a_strokeColor);
    glVertexAttribPointer(a_strokeColor, 4, GL_FLOAT, GL_FALSE, sizeof(ShaderRelevantState), (void*) offsetof(ShaderRelevantState, strokeColor));
    glVertexAttribDivisor(a_strokeColor, 1);

    GLuint a_tintColor = 3;
    glEnableVertexAttribArray(a_tintColor);
    glVertexAttribPointer(a_tintColor, 4, GL_FLOAT, GL_FALSE, sizeof(ShaderRelevantState), (void*) offsetof(ShaderRelevantState, tintColor));
    glVertexAttribDivisor(a_tintColor, 1);

    GLuint a_strokeWeight = 4;
    glEnableVertexAttribArray(a_strokeWeight);
    glVertexAttribPointer(a_strokeWeight, 1, GL_FLOAT, GL_FALSE, sizeof(ShaderRelevantState), (void*) offsetof(ShaderRelevantState, strokeWeight));
    glVertexAttribDivisor(a_strokeWeight, 1);

    GLuint a_textureIndex = 5;
    glEnableVertexAttribArray(a_textureIndex);
    glVertexAttribPointer(a_textureIndex, 1, GL_FLOAT, GL_FALSE, sizeof(ShaderRelevantState), (void*) offsetof(ShaderRelevantState, textureIndex));
    glVertexAttribDivisor(a_textureIndex, 1);

    GLuint a_modelMatrix = 6;
    glEnableVertexAttribArray(a_modelMatrix + 0);
    glVertexAttribPointer(a_modelMatrix + 0, 4, GL_FLOAT, GL_FALSE, sizeof(ShaderRelevantState), (void*) (offsetof(ShaderRelevantState, modelMatrix) + 0 * sizeof(glm::vec4)));
    glVertexAttribDivisor(a_modelMatrix + 0, 1);
    glEnableVertexAttribArray(a_modelMatrix + 1);
    glVertexAttribPointer(a_modelMatrix + 1, 4, GL_FLOAT, GL_FALSE, sizeof(ShaderRelevantState), (void*) (offsetof(ShaderRelevantState, modelMatrix) + 1 * sizeof(glm::vec4)));
    glVertexAttribDivisor(a_modelMatrix + 1, 1);
    glEnableVertexAttribArray(a_modelMatrix + 2);
    glVertexAttribPointer(a_modelMatrix + 2, 4, GL_FLOAT, GL_FALSE, sizeof(ShaderRelevantState), (void*) (offsetof(ShaderRelevantState, modelMatrix) + 2 * sizeof(glm::vec4)));
    glVertexAttribDivisor(a_modelMatrix + 2, 1);
    glEnableVertexAttribArray(a_modelMatrix + 3);
    glVertexAttribPointer(a_modelMatrix + 3, 4, GL_FLOAT, GL_FALSE, sizeof(ShaderRelevantState), (void*) (offsetof(ShaderRelevantState, modelMatrix) + 3 * sizeof(glm::vec4)));
    glVertexAttribDivisor(a_modelMatrix + 3, 1);

    GLuint a_textureMatrix = 10;
    glEnableVertexAttribArray(a_textureMatrix + 0);
    glVertexAttribPointer(a_textureMatrix + 0, 4, GL_FLOAT, GL_FALSE, sizeof(ShaderRelevantState), (void*) (offsetof(ShaderRelevantState, textureMatrix) + 0 * sizeof(glm::vec4)));
    glVertexAttribDivisor(a_textureMatrix + 0, 1);
    glEnableVertexAttribArray(a_textureMatrix + 1);
    glVertexAttribPointer(a_textureMatrix + 1, 4, GL_FLOAT, GL_FALSE, sizeof(ShaderRelevantState), (void*) (offsetof(ShaderRelevantState, textureMatrix) + 1 * sizeof(glm::vec4)));
    glVertexAttribDivisor(a_textureMatrix + 1, 1);
    glEnableVertexAttribArray(a_textureMatrix + 2);
    glVertexAttribPointer(a_textureMatrix + 2, 4, GL_FLOAT, GL_FALSE, sizeof(ShaderRelevantState), (void*) (offsetof(ShaderRelevantState, textureMatrix) + 2 * sizeof(glm::vec4)));
    glVertexAttribDivisor(a_textureMatrix + 2, 1);
    glEnableVertexAttribArray(a_textureMatrix + 3);
    glVertexAttribPointer(a_textureMatrix + 3, 4, GL_FLOAT, GL_FALSE, sizeof(ShaderRelevantState), (void*) (offsetof(ShaderRelevantState, textureMatrix) + 3 * sizeof(glm::vec4)));
    glVertexAttribDivisor(a_textureMatrix + 3, 1);

    glBindVertexArray(0);

    glDeleteBuffers(1, &vbo);

    shader.create(VERTEX_SHADER_SOURCE, FRAGMENT_SHADER_SOURCE);
    shader.createUniform("u_projectionMatrix");
    shader.createUniform("u_textures[0]");
    shader.createUniform("u_textures[1]");
    shader.createUniform("u_textures[2]");
    shader.createUniform("u_textures[3]");
    shader.createUniform("u_textures[4]");
    shader.createUniform("u_textures[5]");
    shader.createUniform("u_textures[6]");
    shader.createUniform("u_textures[7]");
    shader.createUniform("u_resolution");
    shader.createUniform("pxRange");
    shader.createUniform("msdfSize");
    shader.use();
    shader.setUniform("u_textures[0]", 0);
    shader.setUniform("u_textures[1]", 1);
    shader.setUniform("u_textures[2]", 2);
    shader.setUniform("u_textures[3]", 3);
    shader.setUniform("u_textures[4]", 4);
    shader.setUniform("u_textures[5]", 5);
    shader.setUniform("u_textures[6]", 6);
    shader.setUniform("u_textures[7]", 7);
    float pxRange = SHAPE_PXRANGE;
    shader.setUniform("pxRange", pxRange);
    glm::vec2 msdfSize(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    shader.setUniform("msdfSize", msdfSize);

    glGenVertexArrays(1, &postfx_vao);
    glBindVertexArray(postfx_vao);

    GLuint postfx_vbo;
    glGenBuffers(1, &postfx_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, postfx_vbo);
    struct {
        float x, y;
    } postfx_vertices[4] = {
        { -1.0f, -1.0f },
        { -1.0f, 1.0f },
        { 1.0f, -1.0f },
        { 1.0f, 1.0f }
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(postfx_vertices), postfx_vertices, GL_STATIC_DRAW);

    GLuint postfx_a_position = 0;
    glEnableVertexAttribArray(postfx_a_position);
    glVertexAttribPointer(postfx_a_position, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*) 0);

    glBindVertexArray(0);

    glDeleteBuffers(1, &postfx_vbo);

    glGenFramebuffers(1, &postfx_framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, postfx_framebuffer);

    glGenTextures(1, &postfx_texture);
    glBindTexture(GL_TEXTURE_2D, postfx_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, framebufferWidth, framebufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postfx_texture, 0);
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        fputs("Failed to create framebuffer", stderr);
        glfwTerminate();
#ifdef __EMSCRIPTEN__
        emscripten_force_exit(EXIT_FAILURE);
#else
        exit(EXIT_FAILURE);
#endif /* __EMSCRIPTEN__*/
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    postfx_shader.create(POSTFX_VERTEX_SHADER_SOURCE, POSTFX_FRAGMENT_SHADER_SOURCE);
    postfx_shader.createUniform("u_texture");
    postfx_shader.createUniform("u_resolution");
    postfx_shader.use();
    postfx_shader.setUniform("u_texture", 0);

    stateStack.push(State());

    Shape rect, ellipse, triangle;
    describeShape("{ -1, -1; -1, +1; +1, +1; +1, -1; # }", rect);
    describeShape("{ 1, 0; (1, 0.55191502449; 0.55191502449, 1); 0, 1; (-0.55191502449, 1; -1, 0.55191502449); -1, 0; (-1, -0.55191502449; -0.55191502449, -1); 0, -1; (0.55191502449, -1; 1, -0.55191502449); # }", ellipse);
    describeShape("{ -1, +1; -1, -1; +1, +1; # }", triangle);
    shapes.push_back(rect);
    shapes.push_back(ellipse);
    shapes.push_back(triangle);

    std::string minecraft_regular_decoded = base64_decode(minecraft_regular_encoded);
    minecraft_regular._load(reinterpret_cast<const unsigned char *>(minecraft_regular_decoded.c_str()));

    setup();

    Graphics g(width, height, framebufferWidth, framebufferHeight, stateStack, shaderRelevantStateVector, shapes);

#ifdef __EMSCRIPTEN__
    auto _mainLoop = [](void* arg) {
        std::pair<BaseApp*, Graphics>* p = (std::pair<BaseApp*, Graphics>*) arg;
        p->first->mainLoop(p->second);
    };
    std::pair<BaseApp*, Graphics> p = { this, g };
    // TODO: hardcoding the fps here is suboptimal because this means that
    // setTimeout will be used instead of requestAnimationFrame.
    // requestAnimationFrame would be better since it is heavily optimized
    // requestAnimationFrame uses vsync if available
    // TODO: check if vsync is available otherwise hardcode fps
    // emscripten_set_main_loop_arg(_mainLoop, (void*) &p, FRAMES_PER_SECOND, 1); // simulate infinite loop prevents stack to be unwound
    emscripten_set_main_loop_arg(_mainLoop, (void*) &p, 0, 1); // simulate infinite loop prevents stack to be unwound
#else
    double start;
    while (!glfwWindowShouldClose(window)) {
        start = glfwGetTime();
        mainLoop(g);
        std::this_thread::sleep_for(std::chrono::duration<double>(start + SECONDS_PER_FRAME - glfwGetTime())); // TODO: would not be necessary with vsync
    } 
#endif /* __EMSCRIPTEN__*/
}

// PRIVATE

void BaseApp::mainLoop(Graphics& g) {
    // // Measure speed
    // nbFrames++;
    // double currentTime = glfwGetTime();
    // if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
    //     // printf and reset timer
    //     printf("%f ms/frame\n", 1000.0/double(nbFrames));
    //     nbFrames = 0;
    //     lastTime = currentTime;
    // }

#if FXAA
    glBindFramebuffer(GL_FRAMEBUFFER, postfx_framebuffer);
#endif /* FXAA */
    // TODO: check if needed
    // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // glClear(GL_COLOR_BUFFER_BIT);

    shader.use();
    shader.setUniform("u_projectionMatrix", projectionMatrix);
    shader.setUniform("u_resolution", glm::vec2(width, height));

    for (int i = 0; i < 8; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, i);
    }

    draw(g);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, sbo);
    if (shaderRelevantStateVector.size() * sizeof(ShaderRelevantState) > sboSize) {
        while (shaderRelevantStateVector.size() * sizeof(ShaderRelevantState) > sboSize) {
            sboSize *= 2;
        }
        glBufferData(GL_ARRAY_BUFFER, sboSize, nullptr, GL_DYNAMIC_DRAW);
    }
    glBufferSubData(GL_ARRAY_BUFFER, 0, shaderRelevantStateVector.size() * sizeof(ShaderRelevantState), shaderRelevantStateVector.data());
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, shaderRelevantStateVector.size());
    glBindVertexArray(0);

#if FXAA
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // TODO: check if needed
    // glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    // glClear(GL_COLOR_BUFFER_BIT);

    postfx_shader.use();
    postfx_shader.setUniform("u_resolution", glm::vec2(width, height));

    for (int i = 0; i < 8; i++) {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, i);
    }

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, postfx_texture);

    glBindVertexArray(postfx_vao);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
#endif /* FXAA */

    glfwSwapBuffers(window);

    // pop all but the first state from the stateStack
    while(stateStack.size() > 1) {
        stateStack.pop();
    }
    // reset the modelMatrix of the first state (otherwise transformations would be applied cumulatively)
    State& state = stateStack.top();
    state = State(); // reset state
    shaderRelevantStateVector.clear();
    // doing the above steps before glfwPollEvents is essential to
    // allow changing style inside of key and mouse event handlers

    glfwPollEvents();
}

void BaseApp::updateFramebufferSize(int framebufferWidth, int framebufferHeight) {
#if defined(__APPLE__) && !defined(__EMSCRIPTEN__)
    // This is a bit of a hack, but GLFW does not seem to provide a way of querying the monior of the current context,
    // so this is the only way I could come up with of dealing with a mix between a native retina display and an external non-retina one.
    static int oldFramebufferWidth = width;
    static int oldFramebufferHeight = height;
    static bool retinaDisplay = false;
    if (framebufferWidth == 2*oldFramebufferWidth and framebufferHeight == 2*oldFramebufferHeight) {
        // Framebuffer doubled in size. Lets assume that this is because the window got dragged between a non-retina external monitor and the native mac display.
        retinaDisplay = true;
    } else if (oldFramebufferWidth == 2*framebufferWidth and oldFramebufferHeight == 2*framebufferHeight) {
        // Framebuffer halved in size. Lets assume the window got dragged back.
        retinaDisplay = false;
    }
#endif /* __APPLE__ */

    this->framebufferWidth = framebufferWidth;
    this->framebufferHeight = framebufferHeight;
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    float framebufferRatio = framebufferWidth / (float) framebufferHeight;
    float ratio = width / (float) height;
    if (framebufferRatio >= ratio) { 
        projectionMatrix = glm::ortho(0.0f, (framebufferRatio / ratio) * width, (float) height, 0.0f);
#ifndef __EMSCRIPTEN__
        devicePixelRatio = framebufferHeight / (float) height;
#endif /* __EMSCRIPTEN__ */
    } else {
        projectionMatrix = glm::ortho(0.0f, (float) width, (ratio / framebufferRatio) * height, 0.0f);
#ifndef __EMSCRIPTEN__
        devicePixelRatio = framebufferWidth / (float) width;
#endif /* __EMSCRIPTEN__ */
    }

#if defined(__APPLE__) && !defined(__EMSCRIPTEN__)
    // Part II of the retina-display hack.
    devicePixelRatio *= retinaDisplay ? 0.5 : 1.0;
    oldFramebufferWidth = framebufferWidth;
    oldFramebufferHeight = framebufferHeight;
#endif /* __APPLE__ */

#if FXAA
    if (postfx_texture) {
        glBindTexture(GL_TEXTURE_2D, postfx_texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, framebufferWidth, framebufferHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    }
#endif /* FXAA */
}

void BaseApp::framebufferSizeCallback(int framebufferWidth, int framebufferHeight) {
    updateFramebufferSize(framebufferWidth, framebufferHeight);
}

void BaseApp::keyCallback(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        keyPressed(key);
    } else if (action == GLFW_RELEASE) {
        keyReleased(key);
    }
}

void BaseApp::cursorPosCallback(double xpos, double ypos) {
    mouseMoved(xpos / devicePixelRatio, ypos / devicePixelRatio);
}

void BaseApp::mouseButtonCallback(int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        mousePressed(button);
    } else if (action == GLFW_RELEASE) {
        mouseReleased(button);
    }
}

void BaseApp::scrollCallback(double xoffset, double yoffset) {
    if (yoffset < 0.0 || 0.0 < yoffset) {
        mouseWheel(yoffset);
    }
}


void BaseApp::fullscreenchangeCallback(bool isFullscreen) {
#ifdef __EMSCRIPTEN__
    if (!isFullscreen) {
        EM_ASM(
            setTimeout(function() {
                var canvas = document.getElementById('canvas');
                canvas.style.width = (canvas.width / window.devicePixelRatio) + 'px';
                canvas.style.height = (canvas.height / window.devicePixelRatio) + 'px';
            }, 0); // dirty hack :)
        );
    }
#endif /* __EMSCRIPTEN__ */
}

} // namespace nv
