#include "spritesheet.hpp"
#include <json11/json11.hpp>
#include <fstream>
#include <sstream>

namespace piksel {

int Spritesheet::load(std::string filename) {
    std::ifstream ifs(filename);
    std::stringstream ss;
    ss << ifs.rdbuf();
    std::string err;
    const auto json = json11::Json::parse(ss.str(), err);
    img.loadBase64(json["data"].string_value());
    for (auto &sprite : json["sprites"].array_items()) {
        sprites.push_back(Sprite(img, sprite["x"].int_value(), sprite["y"].int_value(), sprite["w"].int_value(), sprite["h"].int_value()));
    }
    return 1;
}

} // namespace piksel
