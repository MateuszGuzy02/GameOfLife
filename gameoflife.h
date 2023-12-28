#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include <QObject>

#include "board.h"
#include "timer.h"

class GameOfLife: public QObject {
    Q_OBJECT

private:

    Board board;
    bool isRunning;
    unsigned int randomSeed;

public:

    GameOfLife(int width, int height) : QObject(), board(width, height), isRunning(false), randomSeed(0) {}

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

public slots:
    void onColumnChange(int newWidth);
    void onRowChange(int newWidth);

};

#endif // GAMEOFLIFE_H
