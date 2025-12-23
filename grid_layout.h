#ifndef GRID_LAYOUT_H
#define GRID_LAYOUT_H

#include <QWidget>
#include <QGridLayout>
#include <QPushButton>
#include <vector>

class GridLayout : public QWidget
{
    Q_OBJECT
public:
    explicit GridLayout(QWidget *parent = nullptr);
    ~GridLayout();

private:
    /*
        0 = normal grid spot
        1 = bomb
    */
    const int rows = 9;
    const int cols = 9;
    enum states {normal = 0, bomb = 1};
    struct Cell {
        int row;
        int col;
        int adjacent_mines = 0;
        QPushButton *button;
        int state;
        bool revealed;
    };
    std::vector<std::vector<Cell>> cells;
    void onButtonClicked();
    void RevealAllMines();
    void DisplayBoard();
    void RevealAdjacentCells(int, int);
    bool CheckWinCondition() const;
    void PlaceBombs();
    void PutAdjacentCounts();
    GridLayout::Cell &GetButtonStruct(QPushButton*);
    QGridLayout *m_layout;
    static constexpr std::array<std::array<int, 2>, 8> directions {{
        {{-1, -1}}, {{-1, 0}}, {{-1, 1}},
        {{0, -1}},  {{0, 1}},
        {{1, -1}},  {{1, 0}},  {{1, 1}}
    }};
};

#endif
