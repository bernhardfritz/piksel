#ifndef SHAPE_HPP
#define SHAPE_HPP

#include "image.hpp"
#include <msdfgen/msdfgen.h>

#define TEXTURE_WIDTH 2048 // aka msdfSize.x
#define TEXTURE_HEIGHT TEXTURE_WIDTH // aka msdfSize.y
#define SHAPE_WIDTH 64
#define SHAPE_HEIGHT SHAPE_WIDTH
#define SHAPE_PXRANGE SHAPE_WIDTH / 4 // this ratio is important in order for scaling to work as expected

namespace piksel {

extern Image msdf;

struct Shape {
    int xoff, yoff;
};

/**
 * In: msdfgenShape; Out: nvShape
 * If everything goes well, this function returns an int >= 0, representing the index of the shape.
 * -1 otherwise.
 */
int registerShape(msdfgen::Shape& msdfgenShape, Shape& nvShape);

/**
 * In: shapeDescription (msdfgen shape description syntax); Out: nvShape
 * If everything goes well, this function returns an int >= 0, representing the index of the shape.
 * -1 otherwise.
 */
int describeShape(std::string shapeDescription, Shape& nvShape);

} // namespace nv

#endif /* SHAPE_HPP */
