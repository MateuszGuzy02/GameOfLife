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
    void boardUpdated(const std::vector<std::vector<int>>& cells) const;

private:

    Board board;
    bool isRunning;

    unsigned int randomSeed;

public:

    GameOfLife(int width, int height) : QObject(), board(width, height), isRunning(false), randomSeed(0) {
        connect(this, SIGNAL(boardUpdated(const std::vector<std::vector<int>>&)),
                this, SLOT(handleBoardUpdated(const std::vector<std::vector<int>>&)));
    }

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

    void startSimulationInThread();

public slots:
    void onColumnChange(int newWidth);
    void onRowChange(int newWidth);
    void handleBoardUpdated(const std::vector<std::vector<int>>& cells);
};

#endif // GAMEOFLIFE_H
