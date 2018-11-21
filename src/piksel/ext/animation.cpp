#include "animation.hpp"

namespace piksel {

void Animation::draw(Graphics& g, float dx, float dy, float dWidth, float dHeight, bool flipHorizontally, bool flipVertically, bool flipDiagonally) {
    if (frames.empty()) {
        return;
    }
    frames[frameIndex].draw(g, dx, dy, dWidth, dHeight, flipHorizontally, flipVertically, flipDiagonally);
    if (g.millis() - lastMillis > millisPerFrame) {
        frameIndex = (frameIndex + 1) % frames.size();
        lastMillis = g.millis();
    }

}

} // namespace piksel
