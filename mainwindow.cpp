#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "QGridLayout.h"
#include "QPushButton.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QWidget *window = new QWidget(this);
    QGridLayout *layout = new QGridLayout(window);
    this->setLayout(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(6);

    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 3; ++col) {
            QString label = QString("Row %1, Col %2").arg(row).arg(col);
            QPushButton *button = new QPushButton(label);

            // addWidget(widget, row, column, rowSpan, columnSpan)
            button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            layout->addWidget(button, row, col);
        }
    }

    setCentralWidget(window);
}

MainWindow::~MainWindow()
{
    delete ui;
}
