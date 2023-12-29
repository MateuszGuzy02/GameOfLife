#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QTableWidget>
#include <QTimer>
#include <random>

#include "gameoflife.h"
#include "board.h"
#include "timer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    unsigned int randomSeed();

private slots:

    void on_columnChanger_valueChanged(int newValue);
    void on_rowChanger_valueChanged(int newValue);

    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_pauseButton_clicked();

    void onBoardUpdated();
    void on_randomSeedButton_clicked();


private:

    Ui::MainWindow *ui;
    GameOfLife *game;
    Board *board;
    QTableWidget *tableWidget;
    QTimer *updateTimer; // Deklaracja zmiennej updateTimer

    const int initialWidth = 10; // Zmień to na swoje preferowane szerokości
    const int initialHeight = 10; // Zmień to na swoje preferowane wysokości

    void createGameBoard(int rows, int columns);

};
#endif // MAINWINDOW_H
