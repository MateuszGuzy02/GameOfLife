#include "timer.h"
#include <iostream>
#include <iomanip>

using namespace std;

void Timer::setInterval(int newInterval)
{
    interval = newInterval;
}

int Timer::getInterval() const
{
    return interval;
}

bool Timer::getRunning() const
{
    return running;
}

void Timer::start()
{
    if (!running)
    {
        startTime = chrono::steady_clock::now();
        running = true;
    }
}

void Timer::pause()
{
    if (running)
    {
        pauseTime = chrono::steady_clock::now();
        running = false;
    }
}

void Timer::resume()
{
    if (!running)
    {
        auto now = chrono::steady_clock::now();
        totalPausedTime += chrono::duration_cast<chrono::milliseconds>(now - pauseTime);
        running = true;
    }
}

void Timer::stop()
{
    if (running)
    {
        auto now = chrono::steady_clock::now();
        running = false;
        displayElapsedTime();               // Wyświetlenie czasu po zatrzymaniu symulacji
    }
}


void Timer::displayElapsedTime() const
{
    auto now = chrono::steady_clock::now();
    auto elapsedMilliseconds = chrono::duration_cast<chrono::milliseconds>(now - startTime - totalPausedTime).count();  // Obliczenie upływu czasu

    // Obliczenie wartości w sekundach i milisekundach
    auto seconds = elapsedMilliseconds / 1000;
    auto milliseconds = elapsedMilliseconds % 1000;

    cout << "Elapsed time: " << seconds << "." << setw(3) << milliseconds << " seconds." << endl;
}
