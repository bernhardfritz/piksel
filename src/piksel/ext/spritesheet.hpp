#ifndef SPRITESHEET_HPP
#define SPRITESHEET_HPP

#include "sprite.hpp"

namespace piksel {

class Spritesheet {
public:
    std::vector<Sprite> sprites;

    int load(std::string filename);
private:
    Image img;
};

}

#endif /* SPRITESHEET_HPP */
