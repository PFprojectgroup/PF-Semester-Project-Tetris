<<<<<<< HEAD
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
=======

#include <iostream>
#include <fstream>
>>>>>>> ff6bfabd05160b20cdadcee36b6b2d3a4d122357
#include <SFML/Graphics.hpp>

using namespace std;

<<<<<<< HEAD
const int ROWS = 20;
const int COLS = 10;

char pieces[7][4][4];

=======
// Store 7 pieces
char pieces[7][4][4];

// Load pieces from file
>>>>>>> ff6bfabd05160b20cdadcee36b6b2d3a4d122357
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
<<<<<<< HEAD
=======

>>>>>>> ff6bfabd05160b20cdadcee36b6b2d3a4d122357
                pieces[p][r][c] = ch;
            }
        }
    }

    return true;
}

<<<<<<< HEAD
// Checks if a 4x4 grid of '#' cells fits inside the board without hitting walls/floor
bool fitsOnBoard(const char grid[4][4], int topRow, int leftCol, const int board[ROWS][COLS])
=======
// Check whether piece can be placed
bool canPlace(char shape[4][4], int row, int col, int board[20][10])
>>>>>>> ff6bfabd05160b20cdadcee36b6b2d3a4d122357
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
<<<<<<< HEAD
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
=======
            if (shape[r][c] == '#')
            {
                int newRow = row + r;
                int newCol = col + c;

                if (newRow < 0 || newRow >= 20 ||
                    newCol < 0 || newCol >= 10)
                {
                    return false;
                }

                if (board[newRow][newCol] != 0)
                {
                    return false;
                }
            }
        }
    }

    return true;
}

int main()
{
    // Load pieces
    if (!loadpieces())
    {
        cout << "Error: pieces.txt file load nahi hui!" << endl;
        return -1;
    }

    cout << "Shapes successfully load ho gayin!" << endl;

    // Game board
    int board[20][10] = { 0 };

>>>>>>> ff6bfabd05160b20cdadcee36b6b2d3a4d122357
    const float CELL = 30.f;
    const float board_x = 20.f;
    const float board_y = 20.f;

<<<<<<< HEAD
=======
    // Colours
>>>>>>> ff6bfabd05160b20cdadcee36b6b2d3a4d122357
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
<<<<<<< HEAD
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
=======

    sf::RenderWindow window(
        sf::VideoMode({ 520, 640 }),
        "Tetris"
    );

    window.setFramerateLimit(60);

    // First piece
    int current_piece = 0;

    int pieceRow = 0;
    int pieceCol = 3;

    // Current piece shape
    char currentShape[4][4];

    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
            currentShape[r][c] = pieces[current_piece][r][c];
        }
    }

    int fallCounter = 0;
>>>>>>> ff6bfabd05160b20cdadcee36b6b2d3a4d122357

    while (window.isOpen())
    {
        // Keyboard events
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
<<<<<<< HEAD
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
=======

            if (const auto* key = event->getIf<sf::Event::KeyPressed>())
            {
                // LEFT
                if (key->code == sf::Keyboard::Key::Left ||
                    key->code == sf::Keyboard::Key::A)
                {
                    if (canPlace(currentShape, pieceRow, pieceCol - 1, board))
                    {
                        pieceCol--;
                    }
                }

                // RIGHT
                if (key->code == sf::Keyboard::Key::Right ||
                    key->code == sf::Keyboard::Key::D)
                {
                    if (canPlace(currentShape, pieceRow, pieceCol + 1, board))
                    {
                        pieceCol++;
                    }
                }

                // DOWN
                if (key->code == sf::Keyboard::Key::Down ||
                    key->code == sf::Keyboard::Key::S)
                {
                    if (canPlace(currentShape, pieceRow + 1, pieceCol, board))
                    {
                        pieceRow++;
                    }
                }

                // ROTATE
                if (key->code == sf::Keyboard::Key::Up ||
                    key->code == sf::Keyboard::Key::W)
                {
                    char rotated[4][4];

                    for (int r = 0; r < 4; r++)
                    {
                        for (int c = 0; c < 4; c++)
                        {
                            rotated[c][3 - r] = currentShape[r][c];
                        }
                    }

                    if (canPlace(rotated, pieceRow, pieceCol, board))
                    {
                        for (int r = 0; r < 4; r++)
                        {
                            for (int c = 0; c < 4; c++)
                            {
                                currentShape[r][c] = rotated[r][c];
                            }
                        }
                    }
                }
            }
        }

        // Automatic falling
        fallCounter++;

        if (fallCounter >= 30)
        {
            fallCounter = 0;

            if (canPlace(currentShape, pieceRow + 1, pieceCol, board))
            {
                pieceRow++;
            }
            else
            {
                // Lock piece into board
                for (int r = 0; r < 4; r++)
                {
                    for (int c = 0; c < 4; c++)
                    {
                        if (currentShape[r][c] == '#')
                        {
                            board[pieceRow + r][pieceCol + c] =
                                current_piece + 1;
                        }
                    }
                }

                // Select next piece in order
                current_piece++;

                if (current_piece >= 7)
                {
                    current_piece = 0;
                }

                pieceRow = 0;
                pieceCol = 3;

                // Copy next piece
                for (int r = 0; r < 4; r++)
                {
                    for (int c = 0; c < 4; c++)
                    {
                        currentShape[r][c] = pieces[current_piece][r][c];
>>>>>>> ff6bfabd05160b20cdadcee36b6b2d3a4d122357
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

<<<<<<< HEAD
        // Draw board grid
        for (int r = 0; r < ROWS; r++)
=======
        // Draw board
        for (int r = 0; r < 20; r++)
>>>>>>> ff6bfabd05160b20cdadcee36b6b2d3a4d122357
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

<<<<<<< HEAD
        // Draw active falling piece at (pieceRow, pieceCol)
=======
        // Draw current piece
>>>>>>> ff6bfabd05160b20cdadcee36b6b2d3a4d122357
        for (int r = 0; r < 4; r++)
        {
            for (int c = 0; c < 4; c++)
            {
<<<<<<< HEAD
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
=======
                if (currentShape[r][c] == '#')
                {
                    block.setFillColor(colors[current_piece + 1]);

                    block.setPosition({
                        board_x + (pieceCol + c) * CELL + 1,
                        board_y + (pieceRow + r) * CELL + 1
                        });

                    window.draw(block);
                }
            }
        }

>>>>>>> ff6bfabd05160b20cdadcee36b6b2d3a4d122357
        window.display();
    }



    return 0;
}
