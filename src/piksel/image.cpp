#include "image.hpp"
#include <stb/stb_image.h>
#include <cpp-base64/base64.h>

namespace piksel {

int Image::load(std::string filename) {
    stbi_set_flip_vertically_on_load(1);
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, NULL, 4);
	return _load(data);
}

int Image::loadBase64(std::string encoded) {
	stbi_set_flip_vertically_on_load(1);
	std::string decoded = base64_decode(encoded);
	unsigned char* data = stbi_load_from_memory(reinterpret_cast<const unsigned char *>(decoded.c_str()), decoded.length(), &width, &height, NULL, 4);
	return _load(data);
}

int Image::_load(unsigned char* data) {
	static int imageIndex = 1; // shape texture uses index 0
	if (!data) {
		fprintf(stderr, "%s\n", stbi_failure_reason());
		return 0;
	}
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	stbi_image_free(data);
    _index = imageIndex++;
    return 1;
}

} // namespace nv
