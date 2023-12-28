#ifndef TIMER_H
#define TIMER_H

#include <chrono>

class Timer {
private:

    int interval;
    bool running;

    std::chrono::steady_clock::time_point startTime;    // Punkt w czasie, od którego rozpoczynamy pomiar czasu.
    std::chrono::steady_clock::time_point pauseTime;    // Punkt w czasie, w którym koñczymy pomiar czasu.
    std::chrono::milliseconds totalPausedTime;          // Ca³kowity czas, który zosta³ "spauzowany" w trakcie pomiaru czasu.

public:
    void start();
    void pause();
    void resume();
    void stop();
    void displayElapsedTime() const;
    void setInterval(int newInterval);

    int getInterval() const;
    bool getRunning() const;
};
#endif // TIMER_H
