#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include "drawmode.hpp"
#include "font.hpp"
#include "image.hpp"
#include "shape.hpp"
#include "state.hpp"
#include <glm/glm.hpp>
#include <stack>
#include <string>
#include <vector>

namespace piksel {

class Graphics {
public:
    Graphics(int& width, int& height, int& framebufferWidth, int& framebufferHeight, std::stack<State>& stateStack, std::vector<ShaderRelevantState>& shaderRelevantStateVector, std::vector<Shape>& shapes)
        : width(width)
        , height(height)
        , framebufferWidth(framebufferWidth)
        , framebufferHeight(framebufferHeight)
        , stateStack(stateStack)
        , shaderRelevantStateVector(shaderRelevantStateVector)
        , shapes(shapes) {
    }

    void applyMatrix(float a, float b, float c, float d, float e, float f);
    void background(glm::vec4 color);
    void ellipse(float x, float y, float w, float h);
    void ellipseMode(DrawMode ellipseMode);
    void fill(glm::vec4 color);
    void image(Image& img, float dx, float dy, float dWidth, float dHeight, float sx, float sy, float sWidth, float sHeight);
    void image(Image& img, float x, float y, float w, float h);
    void imageMode(DrawMode imageMode);
    void line(float x1, float y1, float x2, float y2);
    int millis();
    void noFill();
    void noStroke();
    void noTint();
    void point(float x, float y);
    void pop();
    void push();
    void rect(float x, float y, float w, float h);
    void rectMode(DrawMode rectMode);
    void resetFont();
    void resetMatrix();
    void rotate(float rad);
    void scale(float x, float y);
    void stroke(glm::vec4 color);
    void strokeWeight(float weight);
    void text(std::string str, float x, float y);
    void textFont(Font& font);
    void textSize(float size);
    void tint(glm::vec4 color);
    void translate(float x, float y);
    void triangle(float x1, float y1, float x2, float y2, float x3, float y3);
private:
    int &width, &height;
    int &framebufferWidth, &framebufferHeight;
    std::stack<State>& stateStack;
    std::vector<ShaderRelevantState>& shaderRelevantStateVector;
    std::vector<Shape>& shapes;

    void drawShape(const Shape& shape, DrawMode drawMode, float a, float b, float c, float d);
    State& peek();
    void _rect(float a, float b, float c, float d);
};

} // namespace nv

#endif /* GRAPHICS_HPP */
