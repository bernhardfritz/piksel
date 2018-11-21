#include "timer.hpp"

namespace piksel {

void Timer::reset(int currentTimeInMillis) {
    startTimeInMillis = currentTimeInMillis;
}

int Timer::getElapsedTimeInMillis(int currentTimeInMillis) {
    return currentTimeInMillis - startTimeInMillis;
}

} // namespace piksel
