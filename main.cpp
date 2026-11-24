
#include <iostream>
#include <fstream>
#include <SFML/Graphics.hpp>
using namespace std;

// Store 7 pieces
char pieces[7][4][4];

// Load pieces from file
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

// Check whether piece can be placed
bool canPlace(char shape[4][4], int row, int col, int board[20][10])
{
    for (int r = 0; r < 4; r++)
    {
        for (int c = 0; c < 4; c++)
        {
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

    const float CELL = 30.f;
    const float board_x = 20.f;
    const float board_y = 20.f;

    // Colours
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

    while (window.isOpen())
    {
        // Keyboard events
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

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
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        // Draw board
        for (int r = 0; r < 20; r++)
        {
            for (int c = 0; c < 10; c++)
            {
                block.setFillColor(colors[board[r][c]]);

                block.setPosition({
                    board_x + c * CELL + 1,
                    board_y + r * CELL + 1
                    });

                window.draw(block);
            }
        }

        // Draw current piece
        for (int r = 0; r < 4; r++)
        {
            for (int c = 0; c < 4; c++)
            {
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

        window.display();
    }

    return 0;
}
