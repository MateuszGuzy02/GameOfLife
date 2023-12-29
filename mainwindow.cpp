#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Inicjalizacja obiektu GameOfLife
    game = new GameOfLife(initialWidth, initialHeight); // Ustaw tu początkowe szerokość i wysokość

    // Inicjalizacja timera do odświeżania interfejsu
    updateTimer = new QTimer(this);
    connect(updateTimer, SIGNAL(timeout()), this, SLOT(updateBoard()));
    updateTimer->start(100); // Interwał czasowy do odświeżania interfejsu (w milisekundach)

    // Połączenie zmiany wartości spinboxów z odpowiednimi slotami w GameOfLife
    connect(ui->columnChanger, SIGNAL(valueChanged(int)), game, SLOT(onColumnChanged(int)));
    connect(ui->rowChanger, SIGNAL(valueChanged(int)), game, SLOT(onRowChanged(int)));
    // Połączenie sygnałów z przyciskami
    connect(ui->startButton, SIGNAL(clicked()), this, SLOT(onStartButtonClicked()));
    connect(ui->stopButton, SIGNAL(clicked()), this, SLOT(onStopButtonClicked()));
    connect(ui->pauseButton, SIGNAL(clicked()), this, SLOT(onPauseButtonClicked()));

    // Połączenie sygnałów z przyciskami i innymi elementami interfejsu
    connect(game, SIGNAL(boardUpdated(QVector<QVector<int>>)), this, SLOT(onBoardUpdated(QVector<QVector<int>>)));

    // Połączamy sygnał cellsUpdated z slotem onBoardUpdated w MainWindow
    connect(game, SIGNAL(boardUpdated(const std::vector<std::vector<int>>&)), this, SLOT(onBoardUpdated(const std::vector<std::vector<int>>&)));

    // Podłączanie sygnału kliknięcia przycisku do metody obsługującej
    connect(ui->randomSeedButton, &QPushButton::clicked, this, &MainWindow::randomSeed);

    // Połączenie sygnału boardUpdated z funkcją onBoardUpdated
    connect(game, SIGNAL(boardUpdated()), this, SLOT(onBoardUpdated()));


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

    // Ustawienie stylu dla zaznaczonych komórek
    QString style = "QTableWidget::item:selected { background-color: rgb(0, 153, 255); }";
    ui->tableWidget->setStyleSheet(style);

    onBoardUpdated(); // Aktualizacja widoku planszy po zainicjowaniu
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

void MainWindow::onBoardUpdated() {
    std::vector<std::vector<int>> cellsState = game->getBoard().getCellsState();

    // Aktualizuj QTableWidget na podstawie stanu komórek planszy
    for (int i = 0; i < cellsState.size(); ++i) {
        for (int j = 0; j < cellsState[i].size(); ++j) {
            QTableWidgetItem* item = ui->tableWidget->item(i, j);
            if (!item) {
                item = new QTableWidgetItem();
                ui->tableWidget->setItem(i, j, item);
            }
            if (cellsState[i][j] == 1) {
                item->setBackground(QBrush(QColor(0, 153, 255))); // Ustaw kolor tła dla żywych komórek
            } else {
                item->setBackground(QBrush(Qt::white)); // Ustaw biały kolor tła dla martwych komórek
            }
        }
    }
}

// Metoda losująca seed
unsigned int MainWindow::randomSeed()
{
    random_device rd;
    mt19937 gen(rd()); // Użyj generatora liczb pseudolosowych na podstawie rd()
    uniform_int_distribution<> dis(1, 10000); // Zakres losowania seed

    unsigned int seed = dis(gen); // Pobierz losową wartość seed
    game->setRandomSeed(seed); // Ustaw wygenerowany seed w grze (zakładając, że 'game' to obiekt GameOfLife)

    return seed; // Zwróć wygenerowany seed
}

void MainWindow::on_randomSeedButton_clicked()
{
    unsigned int seed = randomSeed();

    // Pobieranie aktulanego rozmiaru planszy
    int newWidth = ui->tableWidget->columnCount();
    int newHeight = ui->tableWidget->rowCount();


    // Ustawienie nowego ziarna w obiekcie GameOfLife
    game->setRandomSeed(seed);

    // Zaktualizowanie planszy gry w życie
    game->getBoard().resizeBoard(newWidth, newHeight);
    game->getBoard().initializeBoardWithSeed(seed, newWidth, newHeight);

    // Pobranie nowego stanu planszy
    std::vector<std::vector<int>> newCells = game->getBoard().getCellsState();

    // Aktualizacja wyświetlanego stanu planszy
    for (int i = 0; i < newCells.size(); ++i) {
        for (int j = 0; j < newCells[i].size(); ++j) {
            QTableWidgetItem* item = ui->tableWidget->item(i, j);
            if (!item) {
                item = new QTableWidgetItem();
                ui->tableWidget->setItem(i, j, item);
            }
            if (newCells[i][j] == 1) {
                item->setBackground(QBrush(QColor(0, 153, 255))); // Ustaw kolor na niebieski dla żywych komórek
            } else {
                item->setBackground(QBrush(Qt::white)); // Ustaw biały kolor dla martwych komórek
            }
        }
    }
}
