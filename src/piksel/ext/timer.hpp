#ifndef TIMER_HPP
#define TIMER_HPP

namespace piksel {

class Timer {
public:
    Timer() : startTimeInMillis(0) {}
    void reset(int currentTimeInMillis);
    int getElapsedTimeInMillis(int currentTimeInMillis);
private:
    int startTimeInMillis;
};

} // namespace piksel

#endif /* TIMER_HPP */
