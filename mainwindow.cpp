#include "mainwindow.h"
#include "./ui_mainwindow.h"
//#include "QGridLayout.h"
//#include "QPushButton.h"
#include "grid_layout.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    GridLayout *gridWidget = new GridLayout(this);
    gridWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    QWidget *central = new QWidget;
    central->setLayout(new QVBoxLayout);
    central->layout()->addWidget(gridWidget);
    setCentralWidget(central);

    resize(600, 600);
}

MainWindow::~MainWindow()
{
    delete ui;
}
