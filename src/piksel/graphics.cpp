#include "graphics.hpp"
#include "shape.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <stb/stb_truetype.h>

namespace piksel {

// PUBLIC

void Graphics::applyMatrix(float a, float b, float c, float d, float e, float f) {
    State& state = peek();
    glm::mat4 m = glm::mat4(
        a, b, 0, 0,
        c, d, 0, 0,
        e, f, 1, 0,
        0, 0, 0, 1
    );
    state.shaderRelevantState.modelMatrix = m * state.shaderRelevantState.modelMatrix;
}

void Graphics::background(glm::vec4 color) {
    push();
    fill(color);
    noStroke();
    rectMode(DrawMode::CORNER);
    rect(0.0f, 0.0f, framebufferWidth, framebufferHeight);
    pop();
}

void Graphics::ellipse(float a, float b, float c, float d) {
    State& state = peek();
    drawShape(shapes[1], state.shaderIrrelevantState.ellipseMode, a, b, c, d);
}

void Graphics::ellipseMode(DrawMode ellipseMode) {
    State& state = peek();
    state.shaderIrrelevantState.ellipseMode = ellipseMode;
}

void Graphics::fill(glm::vec4 color) {
    State& state = peek();
    state.shaderRelevantState.fillColor = color;
    state.shaderIrrelevantState.fill = true;
}

void Graphics::image(Image& img, float dx, float dy, float dWidth, float dHeight, float sx, float sy, float sWidth, float sHeight) {
    push();
    State& state = peek();
    if (!state.shaderIrrelevantState.tint) {
        state.shaderRelevantState.tintColor = glm::vec4(1.0f);
    }
    state.shaderIrrelevantState.rectMode = state.shaderIrrelevantState.imageMode;
    glActiveTexture(GL_TEXTURE0 + (img._index % 8));
	glBindTexture(GL_TEXTURE_2D, img._texture);
	state.shaderRelevantState.textureMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(sx / img.width, (img.height - sy - sHeight) / img.height, 0.0f));
	state.shaderRelevantState.textureMatrix = glm::scale(state.shaderRelevantState.textureMatrix, glm::vec3(sWidth / img.width, sHeight / img.height, 1.0f));
    // state.shaderRelevantState.textureMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0, 0.0, 0.0f)); // for debug
    state.shaderRelevantState.textureIndex = img._index % 8;
    _rect(dx, dy, dWidth, dHeight);
    pop();
}

void Graphics::image(Image& img, float x, float y, float w, float h) {
    image(img, x, y, w, h, 0.0f, 0.0f, img.width, img.height);
}

void Graphics::imageMode(DrawMode imageMode) {
    State& state = peek();
    state.shaderIrrelevantState.imageMode = imageMode;
}

void Graphics::line(float x1, float y1, float x2, float y2) {
	push();
	State& state = peek();
	state.shaderRelevantState.fillColor = state.shaderRelevantState.strokeColor;
    state.shaderIrrelevantState.fill = state.shaderIrrelevantState.stroke;
    state.shaderIrrelevantState.stroke = false;
    state.shaderRelevantState.textureIndex = -1;
    glm::vec2 a = glm::vec2(x1, y1);
	glm::vec2 b = glm::vec2(x2, y2);
	glm::vec2 r = b - a;
	float angle = atan2f(r.y, r.x);
	translate(x1 + sinf(angle) * state.shaderRelevantState.strokeWeight / 2.0f, y1 - cosf(angle) * state.shaderRelevantState.strokeWeight / 2.0f);
	rotate(angle);
    rectMode(DrawMode::CORNER);
	rect(0.0f, 0.0f, glm::length(r), state.shaderRelevantState.strokeWeight);
    // state.shaderRelevantState.textureIndex = 0;
    // ellipseMode(DrawMode::CENTER);
    // ellipse(0.0f, state.shaderRelevantState.strokeWeight / 2.0f, state.shaderRelevantState.strokeWeight, state.shaderRelevantState.strokeWeight);
    // ellipse(glm::length(r), state.shaderRelevantState.strokeWeight / 2.0f, state.shaderRelevantState.strokeWeight, state.shaderRelevantState.strokeWeight);
	pop();
}

void Graphics::noFill() {
    State& state = peek();
    state.shaderIrrelevantState.fill = false;
}

void Graphics::noStroke() {
    State& state = peek();
    state.shaderIrrelevantState.stroke = false;
}

void Graphics::noTint() {
    State& state = peek();
    state.shaderIrrelevantState.tint = false;
}

void Graphics::point(float x, float y) {
	push();
	State& state = peek();
	state.shaderRelevantState.fillColor = state.shaderRelevantState.strokeColor;
    state.shaderIrrelevantState.fill = state.shaderIrrelevantState.stroke;
    state.shaderIrrelevantState.stroke = false;
    if (state.shaderRelevantState.strokeWeight <= 1.0f) {
        rectMode(DrawMode::CENTER);
        rect(x, y, state.shaderRelevantState.strokeWeight, state.shaderRelevantState.strokeWeight);
    } else {
        ellipseMode(DrawMode::CENTER);
        ellipse(x, y, state.shaderRelevantState.strokeWeight, state.shaderRelevantState.strokeWeight);
    }
	pop();
}

void Graphics::pop() {
    if (stateStack.size() > 1) {
        stateStack.pop();
    }
}

void Graphics::push() {
    State& state = peek();
    stateStack.push(state);
}

void Graphics::rect(float a, float b, float c, float d) {
    State& state = peek();
    drawShape(shapes[0], state.shaderIrrelevantState.rectMode, a, b, c, d);
}

void Graphics::rectMode(DrawMode rectMode) {
    State& state = peek();
    state.shaderIrrelevantState.rectMode = rectMode;
}

void Graphics::resetMatrix() {
    State& state = peek();
    state.shaderRelevantState.modelMatrix = glm::mat4(1.0f);
}

void Graphics::rotate(float rad) {
    State& state = peek();
	state.shaderRelevantState.modelMatrix = glm::rotate(state.shaderRelevantState.modelMatrix, rad, glm::vec3(0.0f, 0.0f, 1.0f));
}

void Graphics::scale(float x, float y) {
    State& state = peek();
    state.shaderRelevantState.modelMatrix = glm::scale(state.shaderRelevantState.modelMatrix, glm::vec3(x, y, 1.0f));
}

void Graphics::stroke(glm::vec4 color) {
    State& state = peek();
    state.shaderRelevantState.strokeColor = color;
    state.shaderIrrelevantState.stroke = true;
}

void Graphics::strokeWeight(float weight) {
    State& state = peek();
    state.shaderRelevantState.strokeWeight = weight;
}

void Graphics::text(std::string str, float x, float y) {
    if (str.empty()) {
        return;
    }
    State& state = peek();
    Font* font = state.shaderIrrelevantState.textFont;
    if (!font || font->letters.empty()) {
        return;
    }
    float textScale = state.shaderIrrelevantState.textSize / SHAPE_HEIGHT;
    glm::vec2 cursor(x, y);
    for (int i = 0; i < str.length(); i++) {
        char ch = str.at(i);
        if (ch == '\n') {
            cursor.x = x;
            cursor.y += ((font->ascent - font->descent + font->lineGap) / float(font->y1 - font->y0)) * textScale * SHAPE_HEIGHT;
        } else {
            if (!(' ' <= ch && ch <= '~')) {
                ch = ' ';
            }
            Letter& letter = font->letters[ch - ' '];
            float letterWidth = letter.x1 - letter.x0;
            float letterHeight = letter.y1 - letter.y0;
            float fontHeight = font->y1 - font->y0;
            float inverseFontHeight = 1.0f / fontHeight;
            float ratioHorizontalSpacing = std::max(0.0f, letterHeight - letterWidth) * inverseFontHeight; // to compensate for letters that are taller than they are wide
            float ratioVerticalSpacing = std::max(0.0f, letterWidth - letterHeight) * inverseFontHeight; // to compensate for letters that are wider than they are tall
            float baseScale = letterHeight * inverseFontHeight + ratioVerticalSpacing; // assuming a textScale of 1.0f, the baseScale represents a factor to render text with a height of SHAPE_HEIGHT
            float leftSideBearing = letter.leftSideBearing * inverseFontHeight;
            float kerning = 0.0f;
            if (i > 0) {
                const char& prev = str.at(i - 1);
                if (prev != '\n') {
                    kerning = stbtt_GetGlyphKernAdvance(font->info, stbtt_FindGlyphIndex(font->info, prev), stbtt_FindGlyphIndex(font->info, ch)) * inverseFontHeight;
                }
            }
            float xoff = ratioHorizontalSpacing / 2.0f - leftSideBearing - kerning;
            float yoff = (letter.y0 - font->y0) * inverseFontHeight - ratioVerticalSpacing / 2.0f;
            float a = cursor.x - xoff * textScale * SHAPE_WIDTH;
            float b = cursor.y - yoff * textScale * SHAPE_HEIGHT;
            float c = a + baseScale * textScale * SHAPE_WIDTH;
            float d = b - baseScale * textScale * SHAPE_HEIGHT;
            drawShape(font->letters[ch - ' '].shape, DrawMode::CORNERS, a, b, c, d);
            float advanceWidth = letter.advanceWidth * inverseFontHeight;
            cursor.x += textScale * SHAPE_WIDTH * advanceWidth;
        }
    }
}

void Graphics::textFont(Font& font) {
    State& state = peek();
    state.shaderIrrelevantState.textFont = &font;
}

void Graphics::textSize(float size) {
    State& state = peek();
    state.shaderIrrelevantState.textSize = size;
}

void Graphics::tint(glm::vec4 color) {
    State& state = peek();
    state.shaderRelevantState.tintColor = color;
    state.shaderIrrelevantState.tint = true;
}

void Graphics::translate(float x, float y) {
    State& state = peek();
    state.shaderRelevantState.modelMatrix = glm::translate(state.shaderRelevantState.modelMatrix, glm::vec3(x, y, 0.0f));
}

void Graphics::triangle(float x1, float y1, float x2, float y2, float x3, float y3) {
	push();
	State& state = peek();

    // this should do for now, optimally the shape itself would be scaled in order to compensate for the SHAPE_PXRANGE,
    // unfortunately this is hard to do with the unit triangle being transformed by the matrix,
    // therefore I'm ok with transforming the texture instead
    glActiveTexture(GL_TEXTURE0 + (msdf._index % 8));
	glBindTexture(GL_TEXTURE_2D, msdf._texture);
    Shape& triangle = shapes[2];
    float sx = triangle.xoff + SHAPE_PXRANGE;
    float sy = triangle.yoff + SHAPE_PXRANGE;
    float sWidth = SHAPE_WIDTH - 2.0 * SHAPE_PXRANGE;
    float sHeight = SHAPE_HEIGHT - 2.0 * SHAPE_PXRANGE;
	state.shaderRelevantState.textureMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(sx / msdf.width, (msdf.height - sy - sHeight) / msdf.height, 0.0f));
	state.shaderRelevantState.textureMatrix = glm::scale(state.shaderRelevantState.textureMatrix, glm::vec3(sWidth / msdf.width, sHeight / msdf.height, 1.0f));
    state.shaderRelevantState.textureIndex = msdf._index % 8;

    translate(x1, y1);

    glm::mat4 m = glm::mat4(
		x1, y1, 1.0f, 0.0f,
		x2, y2, 1.0f, 0.0f,
		x3, y3, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
	) * glm::mat4(
        -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 0.0f,
         1.0f, 0.0f, 0.0f, 0.0f,
         0.0f, 0.0f, 0.0f, 1.0f
    );
	state.shaderRelevantState.modelMatrix = state.shaderRelevantState.modelMatrix * m;
    shaderRelevantStateVector.push_back(state.shaderRelevantState);
	pop();
}

// PRIVATE

State& Graphics::peek() {
    return stateStack.top();
}

void Graphics::drawShape(const Shape& shape, DrawMode drawMode, float a, float b, float c, float d) {
    push();
    State& state = peek();
    if (state.shaderIrrelevantState.fill || state.shaderIrrelevantState.stroke) {
        if (state.shaderIrrelevantState.fill && !state.shaderIrrelevantState.stroke) {
            state.shaderRelevantState.strokeWeight = 0.0f;
            // just in case
            state.shaderRelevantState.strokeColor = state.shaderRelevantState.fillColor;
            state.shaderRelevantState.strokeColor.a = 0.0f;
        } else if (!state.shaderIrrelevantState.fill && state.shaderIrrelevantState.stroke) {
            state.shaderRelevantState.fillColor = state.shaderRelevantState.strokeColor;
            state.shaderRelevantState.fillColor.a = 0.0f;
        }
        float sx = shape.xoff, sy = shape.yoff;
        /*if (drawMode == DrawMode::CENTER) {
            // do nothing
        } else */if (drawMode == DrawMode::CORNER) {
            a += c / 2.0f;
            b += d / 2.0f;
        } else if (drawMode == DrawMode::CORNERS) {
            if (a > c) {
                std::swap(a, c);
            }
            if (b > d) {
                std::swap(b, d);
            }
            float w = c - a;
            float h = d - b;
            a += w / 2.0f;
            b += h / 2.0f;
            c = w;
            d = h;
        } else if (drawMode == DrawMode::RADIUS) {
            c *= 2.0f;
            d *= 2.0f;
        }
        state.shaderRelevantState.strokeWeight = glm::clamp(state.shaderRelevantState.strokeWeight * (2.0f / std::max(c, d)), 0.0f, 0.75f); // magic
        imageMode(DrawMode::CENTER);
        image(msdf, a, b, c * 2.0f, d * 2.0f, sx, sy, SHAPE_WIDTH, SHAPE_HEIGHT); // assuming SHAPE_PXRANGE is SHAPE_WIDTH / 4
    }
    pop();
}

void Graphics::_rect(float a, float b, float c, float d) {
    push();
    State& state = peek();
    switch(state.shaderIrrelevantState.rectMode) {
        case DrawMode::CENTER:
            translate(a - c / 2, b - d / 2);
            scale(c, d);
            break;
        case DrawMode::CORNER:
            translate(a, b);
            scale(c, d);
            break;
        case DrawMode::CORNERS:
            if (a > c) {
                std::swap(a, c);
            }
            if (b > d) {
                std::swap(b, d);
            }
            translate(a, b);
            scale(c - a, d - b);
            break;
        case DrawMode::RADIUS:
            translate(a - c, b - d);
            scale(c * 2, d * 2);
            break; 
    }
    shaderRelevantStateVector.push_back(state.shaderRelevantState);
    pop();
}

} // namespace nv
