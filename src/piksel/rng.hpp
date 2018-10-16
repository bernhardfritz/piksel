#ifndef RNG_HPP
#define RNG_HPP

#include <random>

namespace piksel {

class Rng {
public:
    static Rng& getInstance() {
        static Rng instance;
        return instance;
    }

    float random(float min, float max);
    void randomSeed(unsigned int seed);
private:
    std::mt19937 mt;

    Rng() : mt(std::random_device{}()) {
    }
    Rng(Rng const&) = delete;
    void operator=(Rng const&) = delete;
};

} // namespace nv

#endif /* RNG_HPP */
