#include "shape.hpp"
#ifdef __EMSCRIPTEN__
#define GLFW_INCLUDE_ES3
#include <GLFW/glfw3.h>
#include <GLES3/gl3.h>
#else
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#endif

#define LARGE_VALUE 1e240

namespace piksel {

Image msdf;

bool Shape::operator==(const Shape& shape) const {
    return xoff == shape.xoff && yoff == shape.yoff;
}

static void invertColor(msdfgen::Bitmap<msdfgen::FloatRGB>& bitmap) {
    for (int y = 0; y < bitmap.height(); y++) {
        for (int x = 0; x < bitmap.width(); x++) {
            bitmap(x, y).r = 1.0f - bitmap(x, y).r;
            bitmap(x, y).g = 1.0f - bitmap(x, y).g;
            bitmap(x, y).b = 1.0f - bitmap(x, y).b;
        }
    }
}

static void calculateShapeOffset(int shapeIndex, int& xoff, int& yoff) {
    xoff = (shapeIndex * SHAPE_WIDTH) % TEXTURE_WIDTH;
    yoff = ((shapeIndex * SHAPE_WIDTH) / TEXTURE_WIDTH) * SHAPE_HEIGHT;
}

int registerShape(msdfgen::Shape& msdfgenShape, Shape& pikselShape) {
    static int shapeIndex = 0;

    calculateShapeOffset(shapeIndex, pikselShape.xoff, pikselShape.yoff);

    if (TEXTURE_WIDTH - pikselShape.xoff < SHAPE_WIDTH || TEXTURE_HEIGHT - pikselShape.yoff < SHAPE_HEIGHT) {
        fputs("Failed to register shape. Maximum number of shapes has been reached.", stderr);
        return -1;
    }

    if (msdfgenShape.contours.empty()) {
        return shapeIndex++;
    }

    msdfgen::Vector2 translate;
    msdfgen::Vector2 scale = 1;
    double angleThreshold = 3;

    msdfgenShape.normalize();
    // msdfgenShape.inverseYAxis = !msdfgenShape.inverseYAxis;

    double avgScale = 0.5 * (scale.x + scale.y);
    struct {
        double l, b, r, t;
    } bounds = {
        LARGE_VALUE, LARGE_VALUE, -LARGE_VALUE, -LARGE_VALUE
    };

    msdfgenShape.bounds(bounds.l, bounds.b, bounds.r, bounds.t);

    double l = bounds.l, b = bounds.b, r = bounds.r, t = bounds.t;
    msdfgen::Vector2 frame(SHAPE_WIDTH, SHAPE_HEIGHT);
    frame -= 2.0 * SHAPE_PXRANGE;
    if (l >= r || b >= t) {
        l = 0.0, b = 0.0, r = 1.0, t = 1.0;
    }
    msdfgen::Vector2 dims(r - l, t - b);
    if (dims.x * frame.y < dims.y * frame.x) {
        translate.set(0.5 * (frame.x / frame.y * dims.y - dims.x) - l, -b);
        scale = avgScale = frame.y / dims.y;
    } else {
        translate.set(-l, 0.5 * (frame.y / frame.x * dims.x - dims.y) - b);
        scale = avgScale = frame.x / dims.x;
    }
    translate += SHAPE_PXRANGE / scale;

    double range = SHAPE_PXRANGE / std::min(scale.x, scale.y);

    msdfgen::edgeColoringSimple(msdfgenShape, angleThreshold);
    msdfgen::Bitmap<msdfgen::FloatRGB> bitmap(SHAPE_WIDTH, SHAPE_HEIGHT);
    msdfgen::generateMSDF(bitmap, msdfgenShape, range, scale, translate);

    // Get sign of signed distance outside bounds
    msdfgen::Point2 p(bounds.l - (bounds.r - bounds.l) - 1.0, bounds.b - (bounds.t - bounds.b) - 1.0);
    double dummy;
    msdfgen::SignedDistance minDistance;
    for (std::vector<msdfgen::Contour>::const_iterator contour = msdfgenShape.contours.begin(); contour != msdfgenShape.contours.end(); ++contour) {
        for (std::vector<msdfgen::EdgeHolder>::const_iterator edge = contour->edges.begin(); edge != contour->edges.end(); ++edge) {
            msdfgen::SignedDistance distance = (*edge)->signedDistance(p, dummy);
            if (distance < minDistance) {
                minDistance = distance;
            }
        }
    }
    if (minDistance.distance > 0) {
        invertColor(bitmap);
    }

    if (msdf._texture == 0) {
        msdf._index = 0;
        msdf.width = TEXTURE_WIDTH;
        msdf.height = TEXTURE_HEIGHT;
        glGenTextures(1, &msdf._texture);
	    glBindTexture(GL_TEXTURE_2D, msdf._texture);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, msdf.width, msdf.height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }

    unsigned char pixels[SHAPE_WIDTH * SHAPE_HEIGHT * 3];
    for (int y = 0; y < SHAPE_HEIGHT; y++) {
        for (int x = 0; x < SHAPE_WIDTH; x++) {
            pixels[(y * SHAPE_WIDTH + x) * 3 + 0] = msdfgen::clamp(int(bitmap(x, y).r * 0x100), 0xff);
            pixels[(y * SHAPE_WIDTH + x) * 3 + 1] = msdfgen::clamp(int(bitmap(x, y).g * 0x100), 0xff);
            pixels[(y * SHAPE_WIDTH + x) * 3 + 2] = msdfgen::clamp(int(bitmap(x, y).b * 0x100), 0xff);
        }
    }

    glBindTexture(GL_TEXTURE_2D, msdf._texture);
    glTexSubImage2D(GL_TEXTURE_2D, 0, pikselShape.xoff, msdf.height - pikselShape.yoff - SHAPE_HEIGHT, SHAPE_WIDTH - 1, SHAPE_HEIGHT - 1, GL_RGB, GL_UNSIGNED_BYTE, pixels);
    // glGenerateMipmap(GL_TEXTURE_2D); // dont think this is necessary
    glBindTexture(GL_TEXTURE_2D, 0);

    return shapeIndex++;
}

int describeShape(std::string shapeDescription, Shape& pikselShape) {
    msdfgen::Shape msdfgenShape;

    if (!shapeDescription.empty()) {
        msdfgen::readShapeDescription(shapeDescription.c_str(), msdfgenShape);
    }

    return registerShape(msdfgenShape, pikselShape);
}

} // namespace nv
