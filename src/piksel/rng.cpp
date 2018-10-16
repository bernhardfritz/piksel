#include "rng.hpp"

namespace piksel {

float Rng::random(float min, float max) {
    std::uniform_real_distribution<float> dist(min, max);
	return dist(mt);
}

void Rng::randomSeed(unsigned int seed) {
	mt.seed(seed);
}

}
