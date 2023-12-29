#include "gameoflife.h"
#include <chrono>
#include <thread>
#include <iostream>

using namespace std;

void GameOfLife::start()
{
    int StepCount = 0;
    isRunning = true;
    timer.start();


    while (isRunning)       // Włączanie pętli symulacji gry w życie
    {
        displayBoard();
        step();              // Wykonaj krok symulacji
        StepCount++;


        int aliveCount = 0;
        auto cells = board.getCells();
        for (const auto& row : cells)
        {
            for (int cell : row)
            {
                if (cell == 1)
                    aliveCount++;
            }
        }


        if (aliveCount == 0)        // Jeśli liczba komórek żywych wynosi 0, zatrzymaj symulację
        {
            displayBoard();
            stop();
            cout << "No live cells. Simulation stopped after " << StepCount << " Steps." << endl;
            break;
        }
    }

    board.clear();
}

void GameOfLife::step()
{
    if (isRunning)
        board.nextGeneration();
}

void GameOfLife::pause()
{
    isRunning = false;      // Zatrzymywanie symulacji
    timer.pause();
}

void GameOfLife::resume()
{
    isRunning = true;      // Zatrzymywanie symulacji
    timer.resume();
}

void GameOfLife::stop()
{
    isRunning = false;     // Zatrzymywanie symulacji
    timer.stop();
}

void GameOfLife::setBoardSize(int width, int height)
{
    board.resizeBoard(width, height);
}

void GameOfLife::onColumnChange(int newWidth) {
    // Zmiana szerokości planszy na nową wartość `newWidth`
    board.resizeBoard(newWidth, board.getHeight());
}

void GameOfLife::onRowChange(int newHeight) {
    // Zmiana wysokości planszy na nową wartość `newHeight`
    board.resizeBoard(board.getWidth(), newHeight);
}

void GameOfLife::setRandomSeed(unsigned int seed)
{
    randomSeed = seed;
    int width = board.getWidth();
    int height = board.getHeight();
    board.initializeBoardWithSeed(randomSeed, width, height);
}

void GameOfLife::resizeBoard(int width, int height)
{
    board.resizeBoard(width, height);
}

void GameOfLife::displayBoard() const
{
    timer.getRunning();
    board.printBoard();
    this_thread::sleep_for(chrono::milliseconds(timer.getInterval()));
}

void GameOfLife::updatedBoard()
{
    emit boardUpdated();
}
