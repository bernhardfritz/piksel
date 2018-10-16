#ifndef STATE_HPP
#define STATE_HPP

#include "constants.hpp"
#include "font.hpp"
#include <glm/mat4x4.hpp>
#include <glm/vec4.hpp>

namespace piksel {

struct ShaderRelevantState {
    glm::vec4 fillColor, strokeColor, tintColor;
    float strokeWeight;
    float textureIndex;
    glm::mat4 modelMatrix, textureMatrix;

    ShaderRelevantState()
        : fillColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
        , strokeColor(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
        , tintColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f))
        , strokeWeight(1.0f)
        , textureIndex(-1)
        , modelMatrix(glm::mat4(1.0f))
        , textureMatrix(glm::mat4(1.0f)) {
    }
};

struct ShaderIrrelevantState {
    bool fill, stroke, tint;
    int rectMode, ellipseMode, imageMode;
    Font* textFont;
    float textSize;

    ShaderIrrelevantState()
        : fill(true)
        , stroke(true)
        , tint(false)
        , rectMode(CORNER)
        , ellipseMode(CENTER)
        , imageMode(CORNER)
        , textFont(nullptr)
        , textSize(12.0f) {
    }
};

struct State {
    ShaderRelevantState shaderRelevantState;
    ShaderIrrelevantState shaderIrrelevantState;
};

} // namespace nv

#endif /* STATE_HPP */
