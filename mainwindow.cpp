#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Inicjalizacja obiektu GameOfLife
    game = new GameOfLife(initialWidth, initialHeight); // Ustaw tu początkowe szerokość i wysokość

    // Połączenie zmiany wartości spinboxów z odpowiednimi slotami w GameOfLife
    connect(ui->columnChanger, SIGNAL(valueChanged(int)), game, SLOT(onColumnChanged(int)));
    connect(ui->rowChanger, SIGNAL(valueChanged(int)), game, SLOT(onRowChanged(int)));

    // Ukrycie nagłówków kolumn i wierszy w tableWidget
    ui->tableWidget->verticalHeader()->setVisible(false); // Ukrycie nagłówka kolumn
    ui->tableWidget->horizontalHeader()->setVisible(false); // Ukrycie nagłówka wierszy

    // Utworzenie początkowej planszy gry
    createGameBoard(initialWidth, initialHeight);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createGameBoard(int rows, int columns) {
    ui->tableWidget->setRowCount(rows);
    ui->tableWidget->setColumnCount(columns);

    // Ukrycie nagłówków kolumn i wierszy
    ui->tableWidget->verticalHeader()->setVisible(false);
    ui->tableWidget->horizontalHeader()->setVisible(false);

    // Ustawienie właściwości planszy
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            QTableWidgetItem* item = new QTableWidgetItem();
            item->setFlags(item->flags() & ~Qt::ItemIsEditable); // Wyłączenie edycji komórek
            ui->tableWidget->setItem(i, j, item);
        }
    }

    // Ustawienie rozciągliwego rozmiaru kolumn i wierszy
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MainWindow::on_columnChanger_valueChanged(int newValue) {
    // Aktualizuj planszę gry po zmianie liczby kolumn
    createGameBoard(ui->rowChanger->value(), newValue);
}

void MainWindow::on_rowChanger_valueChanged(int newValue) {
    // Aktualizuj planszę gry po zmianie liczby wierszy
    createGameBoard(newValue, ui->columnChanger->value());
}
