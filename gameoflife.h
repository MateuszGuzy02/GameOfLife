#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include <QObject>
#include <QThread>
#include <QtConcurrent/QtConcurrent>
#include <chrono>
#include <iostream>

#include "board.h"
#include "timer.h"

class GameOfLife: public QObject {
    Q_OBJECT

signals:
    void boardUpdated(); // Sygnał wywoływany przy aktualizacji planszy
    void simulationFinishedSignal(); // Sygnał informujący o zakończeniu symulacji
    void simulationPausedSignal();   // Sygnał informujący o wstrzymaniu symulacji

private:

    Board board;
    bool isRunning;
    bool isPaused; // Zmienna informująca o tym, czy symulacja jest wstrzymana

    unsigned int randomSeed;

public:

    GameOfLife(int width, int height) : QObject(), board(width, height), isRunning(false), randomSeed(0) {}

    Timer timer;
    Board& getBoard() { return board; }

    void start();
    void runSimulation();
    void pause();
    void resume();
    void stop();
    void step();
    void setBoardSize(int width, int height);
    void setRandomSeed(unsigned int seed);
    void resizeBoard(int width, int height);
    bool getIsRunning() const { return isRunning; }
    void displayBoard() const;
    void updatedBoard();

public slots:
    void onColumnChange(int newWidth);
    void onRowChange(int newWidth);
    void simulationFinished(); // Metoda sygnału informującego o zakończeniu symulacji
    void simulationPaused();   // Metoda sygnału informującego o wstrzymaniu symulacji

};

#endif // GAMEOFLIFE_H
