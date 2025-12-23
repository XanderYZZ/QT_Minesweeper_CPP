#include "grid_layout.h"
#include <QSizePolicy>
#include <QRandomGenerator>
#include <QTimer>
#include <QCoreApplication>
#include <queue>

GridLayout::GridLayout(QWidget *parent) : QWidget(parent)
{
    m_layout = new QGridLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    m_layout->setSpacing(6);

    for (int row = 0; row < 9; ++row) {
        cells.push_back({});

        for (int col = 0; col < 9; ++col) {
            Cell cell{row, col, 0, new QPushButton(QString(""), this), normal, false};
            cell.button->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
            m_layout->addWidget(cell.button, row, col);
            connect(cell.button, &QPushButton::clicked, this, &GridLayout::onButtonClicked);
            cells[row].push_back(cell);
        }
    }

    PlaceBombs();
    PutAdjacentCounts();
    DisplayBoard();
    setLayout(m_layout);
}

GridLayout::~GridLayout() = default;

void GridLayout::PlaceBombs() {
    int placed = 0;

    while (placed < 10) {
        int row = QRandomGenerator::global()->bounded(9);
        int col = QRandomGenerator::global()->bounded(9);
        auto &cell = cells[row][col];

        if (cell.state != bomb) {
            cell.state = bomb;
            placed++;
        }
    }
}

void GridLayout::PutAdjacentCounts() {
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (cells[i][j].state == bomb)
            {
                continue;
            }

            int mine_count = 0;

            for (const auto &direction : directions)
            {
                int new_row = i + direction[0];
                int new_col = j + direction[1];

                if (new_row < 0 || new_col < 0 || new_row >= 9 || new_col >= 9)
                {
                    continue;
                }
                if (cells[new_row][new_col].state != bomb)
                {
                    continue;
                }

                mine_count++;
            }

            auto &cell = cells[i][j];
            cell.adjacent_mines = mine_count;
        }
    }
}

GridLayout::Cell& GridLayout::GetButtonStruct(QPushButton *btn) {
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            if (cells[row][col].button != btn) { continue; }

            return cells[row][col];
        }
    }

    return cells[0][0];
}

void GridLayout::RevealAllMines() {
    for (auto &vec : cells) {
        for (auto &cell : vec) {
            if (cell.state != bomb) { continue; }

            cell.revealed = true;
        }
    }
}

void GridLayout::DisplayBoard() {
    for (auto &vec : cells) {
        for (auto &cell : vec) {
            if (!cell.revealed) {
                cell.button->setText(QString("X"));
            } else {
                if (cell.state != bomb) {
                    cell.button->setText(QString(QString::number(cell.adjacent_mines)));
                } else {
                    cell.button->setText(QString("*"));
                }
            }
        }
    }
}

void GridLayout::RevealAdjacentCells(int start_row, int start_col)
{
    std::queue<std::pair<int, int>> q;
    auto &cell = cells[start_row][start_col];
    cell.revealed = true;
    q.push({start_row, start_col});

    while (!q.empty())
    {
        auto [r, c] = q.front();
        q.pop();

        for (const auto& dir : directions)
        {
            int nr = r + dir[0];
            int nc = c + dir[1];

            if (nr < 0 || nr >= 9 || nc < 0 || nc >= 9) { continue; }

            auto &neighbor = cells[nr][nc];

            if (neighbor.revealed || neighbor.state == bomb) { continue; }

            neighbor.revealed = true;

            if (neighbor.adjacent_mines == 0)
            {
                q.push({nr, nc});
            }
        }
    }
}

bool GridLayout::CheckWinCondition() const {
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            if (cells[i][j].state != bomb && !cells[i][j].revealed)
            {
                return false;
            }
        }
    }

    return true;
}

void GridLayout::onButtonClicked() {
    QPushButton *btn = qobject_cast<QPushButton*>(sender());

    if (!btn) { return; }

    auto &cell = GetButtonStruct(btn);

    if (cell.revealed) { return; }

    cell.revealed = true;

    if (cell.state == normal) {
        if (cell.adjacent_mines == 0) {
            RevealAdjacentCells(cell.row, cell.col);
        }

        DisplayBoard();

        if (CheckWinCondition()) {
            QTimer::singleShot(2000, [](){
                QCoreApplication::quit();
            });
        }
    } else if (cell.state == bomb) {
        // Reveal all the mines and display the board.
        RevealAllMines();
        DisplayBoard();

        QTimer::singleShot(2000, [](){
            QCoreApplication::quit();
        });
    }
}
