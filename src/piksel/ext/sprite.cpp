#include "sprite.hpp"

namespace piksel {

void Sprite::draw(Graphics& g, float dx, float dy, float dWidth, float dHeight, bool flipHorizontally, bool flipVertically, bool flipDiagonally) {
    if (flipDiagonally) {
        g.push();
        g.imageMode(DrawMode::CENTER);
        g.translate(dx + dWidth / 2.0f, dy + dHeight / 2.0f);
        g.rotate(HALF_PI);
        g.image(img, 0, 0, dWidth, dHeight, sx + (flipVertically ? sWidth : 0), sy + (!flipHorizontally ? sHeight : 0), sWidth * (flipVertically ? -1 : 1), sHeight * (!flipHorizontally ? -1 : 1));
        g.pop();
    } else {
        g.image(img, dx, dy, dWidth, dHeight, sx + (flipHorizontally ? sWidth : 0), sy + (flipVertically ? sHeight : 0), sWidth * (flipHorizontally ? -1 : 1), sHeight * (flipVertically ? -1 : 1));
    }
}

} // namespace piksel
