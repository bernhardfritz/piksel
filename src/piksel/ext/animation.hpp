#ifndef ANIMATION_HPP
#define ANIMATION_HPP

#include "sprite.hpp"

namespace piksel {

class Animation {
public:
    std::vector<Sprite> frames;
    int millisPerFrame = 100;
    
    void draw(Graphics& g, float dx, float dy, float dWidth, float dHeight, bool flipHorizontally = false, bool flipVertically = false, bool flipDiagonally = false);
private:
    int frameIndex = 0;
    int lastMillis = 0;
};

} // namespace piksel

#endif /* ANIMATION_HPP */
