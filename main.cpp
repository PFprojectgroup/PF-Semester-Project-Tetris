#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <SFML/Graphics.hpp>

using namespace std;

const int ROWS = 20;
const int COLS = 10;

char pieces[7][4][4];

bool loadpieces()
{
    ifstream fin("pieces.txt");
    if (!fin)
        return false;

    int count;
    fin >> count;
    if (count != 7)
        return false;

    for (int p = 0; p < count; p++)
    {
        for (int r = 0; r < 4; r++)
        {
            for (int c = 0; c < 4; c++)
            {
                char ch;
                fin >> ch;
                if (!fin)
                    return false;
                pieces[p][r][c] = ch;
            }
        }
    }
    return true;
}

// Checks if a 4x4 grid of '#' cells fits inside the board without hitting walls/floor
bool fitsOnBoard(const char grid[4][4], int topRow, int leftCol, const int board[ROWS][COLS])
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            if (grid[r][c] == '#')
            {
                int targetR = topRow + r;
                int targetC = leftCol + c;

                if (targetC < 0 || targetC >= COLS || targetR >= ROWS)
                    return false;

                if (targetR >= 0 && board[targetR][targetC] != 0)
                    return false;
            }
        }
    }
    return true;
}

// Rotates a 4x4 matrix 90 degrees clockwise into rotatedOut
void rotateCW(const char src[4][4], char rotatedOut[4][4])
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            rotatedOut[c][3 - r] = src[r][c];
        }
    }
}
int clearLines(int board[ROWS][COLS])
{
    int cleared = 0;
    int wRow = ROWS - 1;

    for (int rRow = ROWS - 1; rRow >= 0; rRow--)
    {
        bool full = true;
        for (int c = 0; c < COLS; c++)
        {
            if (board[rRow][c] == 0)
            {
                full = false;
                break;
            }
        }

        if (full)
        {
            cleared++;
            continue;
        }

        if (wRow != rRow)
        {
            for (int c = 0; c < COLS; c++)
                board[wRow][c] = board[rRow][c];
        }
        wRow--;
    }

    for (int r = wRow; r >= 0; r--)
        for (int c = 0; c < COLS; c++)
            board[r][c] = 0;

    return cleared;
}
int main()
{

    if (!loadpieces())
    {
        cout << "Error: pieces.txt file load nahi hui!" << endl;
        return -1;
    }
    cout << "Shapes successfully load ho gayin!" << endl;

    int board[ROWS][COLS] = { 0 };
    const float CELL = 30.f;
    const float board_x = 20.f;
    const float board_y = 20.f;

    sf::Color colors[8] =
    {
        sf::Color(25, 25, 45),
        sf::Color::Cyan,
        sf::Color::Yellow,
        sf::Color(170, 90, 255),
        sf::Color::Green,
        sf::Color::Red,
        sf::Color::Blue,
        sf::Color(255, 150, 0)
    };

    sf::RectangleShape block({ CELL - 2, CELL - 2 });
    sf::RenderWindow window(sf::VideoMode({ 520, 640 }), "Tetris");
    window.setFramerateLimit(60);

    srand(static_cast<unsigned>(time(0)));

    int current_piece = rand() % 7;
    char activeShape[4][4];
    for (int r = 0; r < 4; r++)
        for (int c = 0; c < 4; c++)
            activeShape[r][c] = pieces[current_piece][r][c];

    int pieceRow = 0;
    int pieceCol = 3; // roughly centered across 10 columns

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>())
            {
                if (keyPressed->code == sf::Keyboard::Key::Left)
                {
                    if (fitsOnBoard(activeShape, pieceRow, pieceCol - 1, board))
                        pieceCol--;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Right)
                {
                    if (fitsOnBoard(activeShape, pieceRow, pieceCol + 1, board))
                        pieceCol++;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Down)
                {
                    if (fitsOnBoard(activeShape, pieceRow + 1, pieceCol, board))
                        pieceRow++;
                }
                else if (keyPressed->code == sf::Keyboard::Key::Up)
                {
                    char rotated[4][4];
                    rotateCW(activeShape, rotated);
                    if (fitsOnBoard(rotated, pieceRow, pieceCol, board))
                    {
                        for (int r = 0; r < 4; r++)
                            for (int c = 0; c < 4; c++)
                                activeShape[r][c] = rotated[r][c];
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        // Draw board grid
        for (int r = 0; r < ROWS; r++)
        {
            for (int c = 0; c < COLS; c++)
            {
                block.setFillColor(colors[board[r][c]]);
                block.setPosition({
                    board_x + c * CELL + 1,
                    board_y + r * CELL + 1
                    });
                window.draw(block);
            }
        }

        // Draw active falling piece at (pieceRow, pieceCol)
        for (int r = 0; r < 4; r++)
        {
            for (int c = 0; c < 4; c++)
            {
                if (activeShape[r][c] == '#')
                {
                    int drawRow = pieceRow + r;
                    int drawCol = pieceCol + c;
                    if (drawRow >= 0 && drawRow < ROWS && drawCol >= 0 && drawCol < COLS)
                    {
                        block.setFillColor(colors[current_piece + 1]);
                        block.setPosition({
                            board_x + drawCol * CELL + 1,
                            board_y + drawRow * CELL + 1
                            });
                        window.draw(block);
                    }
                }
            }
        }
        int lines = clearLines(board);
        window.display();
    }

    return 0;
}