#ifndef INTERNALS_HPP
#define INTERNALS_HPP

#include <string>

namespace piksel {

// Include all the core shaders at compile time

#ifdef __EMSCRIPTEN__
const std::string glsl_version = "#version 300 es\n";
#else
const std::string glsl_version = "#version 330 core\n";
#endif

#include "shaders/main.vert"
#include "shaders/main.frag"
#include "shaders/post.vert"
#include "shaders/post.frag"

const std::string minecraft_regular_encoded = "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";

extern Font minecraft_regular;

} // namespace nv

#endif /* INTERNALS_HPP */
