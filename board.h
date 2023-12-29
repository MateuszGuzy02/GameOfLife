#ifndef BOARD_H
#define BOARD_H

#include <QObject>
#include <vector>

class Board : public QObject {
    Q_OBJECT

signals:
    void cellsUpdated(const std::vector<std::vector<int>>& cells) const;

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
    void initializeBoardWithSeed(unsigned int seed, int newWidth, int newHeight);
    void resizeBoard(int newWidth, int newHeight);
    void nextGeneration();
    void printBoard() const;
    void clear();

    int countAliveNeighbors(int x, int y) const;
    bool isAlive(const int x, const int y) const;
    std::vector<std::vector<int>> getCellsState() const;
    void setCellState(int row, int col, int state);

    void updateCells();

};
#endif // BOARD_H
