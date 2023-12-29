#include "gameoflife.h"

using namespace std;

void GameOfLife::start()
{
    // Uruchomienie algorytmu w oddzielnym wątku
    QtConcurrent::run([this]() {
        int stepCount = 0;

        while (isRunning) {
            step(); // Wykonaj krok symulacji
            QThread::msleep(100); // Pauza między krokami (np. 100ms)

            stepCount++;

            int aliveCount = 0;
            auto cells = board.getCells();
            for (const auto& row : cells) {
                for (int cell : row) {
                    if (cell == 1)
                        aliveCount++;
                }
            }

            if (aliveCount == 0) {
                displayBoard();
                stop();
                cout << "No live cells. Simulation stopped after " << stepCount << " Steps." << endl;
                break;
            }
        }

        // Po zakończeniu pętli wyślij sygnał o aktualizacji planszy
        emit boardUpdated();
        board.clear();
    });
}

void GameOfLife::runSimulation()
{
    int stepCount = 0;

    while (isRunning) {
        if (!isPaused) {
            board.nextGeneration(); // Wygeneruj następną generację planszy
            emit boardUpdated(); // Emituj sygnał informujący o aktualizacji planszy

            stepCount++;

            int aliveCount = 0;
            auto cells = board.getCells();
            for (const auto& row : cells) {
                for (int cell : row) {
                    if (cell == 1)
                        aliveCount++;
                }
            }

            if (aliveCount == 0) {
                stop();
                emit simulationFinished();
                break;
            }

            QThread::msleep(timer.getInterval()); // Odczekaj czas określony przez timer
        } else {
            QThread::msleep(100); // Odczekaj, jeśli symulacja jest wstrzymana
        }
    }
    // Opcjonalnie: wywołanie czyszczenia planszy po zakończeniu symulacji
     board.clear();
}

void GameOfLife::step()
{
    if (isRunning)
    {
        board.nextGeneration();
        emit boardUpdated(); // Emituj sygnał informujący o aktualizacji planszy
    }
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

void GameOfLife::simulationFinished() {
    isRunning = false; // Zatrzymaj symulację
    emit simulationFinishedSignal(); // Emituj sygnał informujący o zakończeniu symulacji
}

void GameOfLife::simulationPaused() {
    isPaused = true; // Ustaw zmienną informującą o wstrzymaniu symulacji
    emit simulationPausedSignal(); // Emituj sygnał informujący o wstrzymaniu symulacji
}

