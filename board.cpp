#include "board.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "gameoflife.h"
#include <random>

using namespace std;

Board::Board(int w, int h) : width(w), height(h)
{
    initializeBoard();
}

void Board::initializeBoard()
{
    mt19937 rng(static_cast<unsigned int>(time(nullptr)));  // Inicjalizacja generatora liczb pseudolosowych
    uniform_int_distribution<int> dist(0, 1);               // Dystrybucja dla losowania liczb 0 lub 1

    cells.resize(height, vector<int>(width, 0));            // Inicjalizacja tablicy dynamicznej

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
            cells[i][j] = dist(rng);                        // Losowa wartość: 0 lub 1
    }
}

void Board::printBoard() const
{
    cout << endl;

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
            cout << (cells[i][j] ? 'O' : '.') << ' ';

        cout << endl;
    }
    cout << endl;
}


void Board::resizeBoard(int newWidth, int newHeight)
{
    width = newWidth;
    height = newHeight;

    cells.assign(height, vector<int>(width, 0));            // Tworzymy nową tablicę o zadanych wymiarach i wypełniamy ją zerami
}

void Board::clear()
{
    cells.clear();
    width = 0;
    height = 0;
}


void Board::nextGeneration()
{
    vector<vector<int>> newCells(height, vector<int>(width, 0));

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
        {
            if (isAlive(i, j))
                newCells[i][j] = 1;         // Komórka ożywa
            else
                newCells[i][j] = 0;         // Komórka umiera
        }
    }

    cells = newCells;
}

int Board::countAliveNeighbors(int x, int y) const
{
    int alive = 0;

    for (int ni = max(0, x - 1); ni <= min(height - 1, x + 1); ++ni)
    {
        for (int nj = max(0, y - 1); nj <= min(width - 1, y + 1); ++nj)
        {
            if (ni != x || nj != y)               // Pomijamy współrzędne tej samej komórki
                alive += cells[ni][nj];
        }
    }

    return alive;
}

bool Board::isAlive(const int x, const int y) const
{
    int dx[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
    int dy[] = { -1, 0, 1, -1, 1, -1, 0, 1 };

    int alive = 0;

    for (int i = 0; i < 8; ++i) {
        int nx = x + dx[i];
        int ny = y + dy[i];

        if (nx >= 0 && nx < height && ny >= 0 && ny < width) {
            alive += cells[nx][ny];
        }
    }

    // ZASADY
    if (cells[x][y] == 1 && alive < 2) return false;
    if (cells[x][y] == 1 && (alive == 2 || alive == 3)) return true;
    if (cells[x][y] == 1 && alive > 3) return false;
    if (cells[x][y] == 0 && alive == 3) return true;

    return false;
}

void Board::initializeBoardWithSeed(unsigned int seed)
{
    random_device rd;
    mt19937 gen(seed);                                  // Użyj ziarna dla generatora liczb losowych
    uniform_int_distribution<> dis(0, 1);               // Zakres losowania: 0 lub 1

    cells.resize(height, vector<int>(width, 0));        // Inicjalizacja tablicy dynamicznej

    for (int i = 0; i < height; ++i)
    {
        for (int j = 0; j < width; ++j)
            cells[i][j] = dis(gen);                     // Użyj generatora liczb losowych z danym ziarnem
    }
}
