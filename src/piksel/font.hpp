#ifndef FONT_HPP
#define FONT_HPP
#include "shape.hpp"
#include <string>
#include <vector>

struct stbtt_fontinfo; // forward declaration

namespace piksel {

struct Letter {
    Shape shape;
    int advanceWidth, leftSideBearing, x0, y0, x1, y1;
};

class Font {
public:
    stbtt_fontinfo* info;
    int ascent, descent, lineGap, x0, y0, x1, y1;
    std::vector<Letter> letters;

    Font() : info(nullptr) {}
    ~Font();
    int load(std::string filename);
};

} // namespace nv


#endif /* FONT_HPP */
