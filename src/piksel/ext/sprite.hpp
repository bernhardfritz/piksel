#ifndef SPRITE_HPP
#define SPRITE_HPP

#include "../image.hpp"
#include "../graphics.hpp"

namespace piksel {

class Sprite {
public:
    Sprite(Image& img, float sx, float sy, float sWidth, float sHeight)
        : img(img)
        , sx(sx)
        , sy(sy)
        , sWidth(sWidth)
        , sHeight(sHeight) {
    }

    void draw(Graphics& g, float dx, float dy, float dWidth, float dHeight, bool flipHorizontally = false, bool flipVertically = false, bool flipDiagonally = false);
private:
    Image& img;
    float sx;
    float sy;
    float sWidth;
    float sHeight;
};

} // namespace

#endif /* SPRITE_HPP */
