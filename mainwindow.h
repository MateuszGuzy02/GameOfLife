#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSpinBox>
#include <QTableWidget>

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

private slots:

    void on_columnChanger_valueChanged(int newValue);
    void on_rowChanger_valueChanged(int newValue);

private:

    Ui::MainWindow *ui;
    GameOfLife *game;
    QTableWidget *tableWidget;

    const int initialWidth = 10; // Zmień to na swoje preferowane szerokości
    const int initialHeight = 10; // Zmień to na swoje preferowane wysokości

    void createGameBoard(int rows, int columns);

};
#endif // MAINWINDOW_H
