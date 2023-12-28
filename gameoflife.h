#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include "board.h"
#include "timer.h"

class GameOfLife {
private:

    Board board;
    bool isRunning;
    unsigned int randomSeed;

public:

    GameOfLife(int width, int height) : board(width, height), isRunning(false), randomSeed(0) {}

    Timer timer;
    Board& getBoard() { return board; }

    void start();
    void pause();
    void resume();
    void stop();
    void step();
    void setBoardSize(int width, int height);
    void setRandomSeed(unsigned int seed);
    void resizeBoard(int width, int height);
    bool getIsRunning() const { return isRunning; }
    void displayBoard() const;

};

#endif // GAMEOFLIFE_H
