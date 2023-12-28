#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board {
private:

    std::vector<std::vector<int>> cells;
    int width = 0;
    int height = 0;

public:

    Board(int w, int h);

    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int& getCell(int x, int y) { return cells[x][y]; }
    std::vector<std::vector<int>>& getCells() { return cells; }

    void initializeBoard();
    void initializeBoardWithSeed(unsigned int seed);
    void resizeBoard(int newWidth, int newHeight);
    void nextGeneration();
    void printBoard() const;
    void clear();

    int countAliveNeighbors(int x, int y) const;
    bool isAlive(const int x, const int y) const;

};
#endif // BOARD_H
