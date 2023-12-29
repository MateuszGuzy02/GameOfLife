#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Inicjalizacja obiektu GameOfLife
    game = new GameOfLife(initialWidth, initialHeight);

    // Ukrycie nagłówków kolumn i wierszy w tableWidget
    ui->tableWidget->verticalHeader()->setVisible(false); // Ukrycie nagłówka kolumn
    ui->tableWidget->horizontalHeader()->setVisible(false); // Ukrycie nagłówka wierszy

    // Utworzenie początkowej planszy gry
    createGameBoard(initialWidth, initialHeight);

    // Połączenie zmiany wartości spinboxów z odpowiednimi slotami w GameOfLife
    connect(ui->columnChanger, SIGNAL(valueChanged(int)), game, SLOT(onColumnChanged(int)));
    connect(ui->rowChanger, SIGNAL(valueChanged(int)), game, SLOT(onRowChanged(int)));

    // Połącz przycisk "Start" z nowym slotem startSimulation
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(onStartButtonClicked()));

    // Podłączanie sygnału kliknięcia przycisku do metody obsługującej
    connect(ui->randomSeedButton, &QPushButton::clicked, this, &MainWindow::randomSeed);

    connect(game, SIGNAL(boardUpdated(const std::vector<std::vector<int>>&)),
            this, SLOT(onBoardUpdated()));

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

    // Ustawienie stylu dla zaznaczonych komórek
    QString style = "QTableWidget::item:selected { background-color: rgb(0, 153, 255); }";
    ui->tableWidget->setStyleSheet(style);

    onBoardUpdated(); // Aktualizacja widoku planszy po zainicjowaniu
    qDebug() << "createGameBoard() called";
}

void MainWindow::on_columnChanger_valueChanged(int newValue) {
    // Aktualizuj planszę gry po zmianie liczby kolumn
    createGameBoard(ui->rowChanger->value(), newValue);
}

void MainWindow::on_rowChanger_valueChanged(int newValue) {
    // Aktualizuj planszę gry po zmianie liczby wierszy
    createGameBoard(newValue, ui->columnChanger->value());
}

void MainWindow::on_startButton_clicked()
{
    game->start();
}

void MainWindow::on_stopButton_clicked()
{
    game->stop();
}


void MainWindow::on_pauseButton_clicked()
{
    game->pause();
}

void MainWindow::onBoardUpdated()
{
    qDebug() << "onBoardUpdated() called";
    const std::vector<std::vector<int>>& cells = game->getBoard().getCells();

    for (int i = 0; i < cells.size(); ++i)
    {
        for (int j = 0; j < cells[i].size(); ++j)
        {
            QTableWidgetItem* item = ui->tableWidget->item(i, j);
            if (!item)
            {
                item = new QTableWidgetItem();
                ui->tableWidget->setItem(i, j, item);
            }

            // Ustaw kolor ramki komórki w zależności od stanu
            if (cells[i][j] == 1)
            {
                item->setBackground(QBrush(QColor(0, 153, 255))); // Ustaw kolor na niebieski dla żywych komórek
                item->setForeground(QBrush(Qt::black)); // Ustaw kolor czcionki na czarny
            }
            else
            {
                item->setBackground(QBrush(Qt::white)); // Ustaw biały kolor dla martwych komórek
                item->setForeground(QBrush(Qt::gray)); // Ustaw kolor czcionki na szary
            }
        }
    }
}

// Metoda losująca seed
unsigned int MainWindow::randomSeed()
{
    std::uniform_int_distribution<> dis(0, 1); // Zakres losowania wartości komórek

    int newWidth = ui->tableWidget->columnCount();
    int newHeight = ui->tableWidget->rowCount();

    // Przechowaj stare komórki
    const std::vector<std::vector<int>>& oldCells = game->getBoard().getCells();

    // Inicjalizuj nowy seed
    unsigned int seed = dis(generator);
    game->setRandomSeed(seed);

    // Zaktualizuj planszę gry
    game->getBoard().resizeBoard(newWidth, newHeight);

    // Zainicjuj stare komórki na nowo
    game->getBoard().initializeBoardWithSeed(seed, newWidth, newHeight);

    // Przepisz stare komórki do nowej planszy
    for (int i = 0; i < min(newHeight, static_cast<int>(oldCells.size())); ++i) {
        for (int j = 0; j < min(newWidth, static_cast<int>(oldCells[i].size())); ++j) {
            game->getBoard().setCellState(i, j, oldCells[i][j]);
        }
    }

    // Uaktualnij widok planszy w interfejsie użytkownika
    onBoardUpdated();

    return seed; // Zwróć wygenerowany seed
}

void MainWindow::on_randomSeedButton_clicked()
{
    unsigned int seed = randomSeed();

    // Pobieranie aktualnego rozmiaru planszy
    int newWidth = ui->tableWidget->columnCount();
    int newHeight = ui->tableWidget->rowCount();

    // Ustawienie nowego ziarna w obiekcie GameOfLife
    game->setRandomSeed(seed);

    // Zaktualizowanie planszy gry w życie
    game->getBoard().resizeBoard(newWidth, newHeight);
    game->getBoard().initializeBoardWithSeed(seed, newWidth, newHeight);

    // Pobranie nowego stanu planszy
    const std::vector<std::vector<int>>& newCells = game->getBoard().getCells();

    // Aktualizacja wyświetlanego stanu planszy
    for (int i = 0; i < newHeight; ++i) {
        for (int j = 0; j < newWidth; ++j) {
            QTableWidgetItem* item = ui->tableWidget->item(i, j);
            if (!item) {
                item = new QTableWidgetItem();
                ui->tableWidget->setItem(i, j, item);
            }

            int cellState = newCells[i][j];
            game->getBoard().setCellState(i, j, cellState); // Ustaw wartość komórki

            if (cellState == 1) {
                item->setBackground(QBrush(QColor(0, 153, 255))); // Ustaw kolor na niebieski dla żywych komórek
            } else {
                item->setBackground(QBrush(Qt::white)); // Ustaw biały kolor dla martwych komórek
            }
        }
    }

    // Uaktualnij widok planszy w interfejsie użytkownika
    onBoardUpdated();
}

void MainWindow::onStartButtonClicked()
{
    qDebug() << "onStartButtonClicked() called";
    game->startSimulationInThread();
    onBoardUpdated(); // Dodaj to, aby odświeżyć planszę po starcie symulacji
}

