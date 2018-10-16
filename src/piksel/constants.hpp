#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <glm/gtc/constants.hpp>

namespace piksel {

const int CENTER = 0, CORNER = 1, CORNERS = 2, RADIUS = 3;

const float PI = glm::pi<float>();
const float QUARTER_PI = glm::quarter_pi<float>();
const float HALF_PI = glm::half_pi<float>();
const float TWO_PI = 2.0f * PI;

} // namespace nv

#endif /* CONSTANTS_HPP */
