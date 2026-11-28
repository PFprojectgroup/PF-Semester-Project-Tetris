#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
using namespace std;

// Store 7 pieces
char pieces[7][4][4];

const int MENU = 0;
const int PLAYING = 1;
const int GAME_OVER = 2;

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

// Clear completed lines
int clearLines(int board[20][10])
{
    int lines = 0;

    for (int r = 19; r >= 0; r--)
    {
        bool full = true;

        for (int c = 0; c < 10; c++)
        {
            if (board[r][c] == 0)
            {
                full = false;
                break;
            }
        }

        if (full)
        {
            lines++;

            // Move rows above downward
            for (int i = r; i > 0; i--)
            {
                for (int c = 0; c < 10; c++)
                {
                    board[i][c] = board[i - 1][c];
                }
            }

            // Empty top row
            for (int c = 0; c < 10; c++)
            {
                board[0][c] = 0;
            }

            // Check the same row again
            r++;
        }
    }

    return lines;
}

// Load High Score
int loadHighScore()
{
    ifstream fin("highscore.txt");

    int best = 0;

    if (fin)
    {
        fin >> best;

        if (best < 0)
        {
            best = 0;
        }
    }

    return best;
}

// Save High Score
void saveHighScore(int best)
{
    ofstream fout("highscore.txt");

    if (fout)
    {
        fout << best << endl;
    }
}

// Check High Score
void checkHighScore(int score, int& highScore)
{
    if (score > highScore)
    {
        highScore = score;

        saveHighScore(highScore);
    }
}
// Draw Level
void drawLevel(sf::RenderWindow& window, sf::Font& font, int level)
{
    sf::Text text(font, "LEVEL: " + to_string(level), 18);

    text.setFillColor(sf::Color::Green);
    text.setPosition({ 330.f, 180.f });

    window.draw(text);
}
// Draw High Score
void drawHighScore(sf::RenderWindow& window, sf::Font& font, int highScore)
{
    sf::Text text(font, "HIGH SCORE: " + to_string(highScore), 18);

    text.setFillColor(sf::Color::Yellow);
    text.setPosition({ 330.f, 80.f });

    window.draw(text);
}

// Draw Current Score
void drawScore(sf::RenderWindow& window, sf::Font& font, int score)
{
    sf::Text text(font, "SCORE: " + to_string(score), 18);

    text.setFillColor(sf::Color::White);
    text.setPosition({ 330.f, 130.f });

    window.draw(text);
}

// Draw Main Menu
void drawMenu(sf::RenderWindow& window, sf::Font& font,
    int selected, bool soundOn)
{
    sf::Text title(font, "TETRIS", 50);

    title.setFillColor(sf::Color::Cyan);
    title.setPosition({ 150.f, 150.f });

    window.draw(title);

    sf::Text newGame(font, "NEW GAME", 30);
    sf::Text soundOption(font, "SOUND: ON", 30);
    sf::Text exitGame(font, "EXIT", 30);

    if (soundOn)
    {
        soundOption.setString("SOUND: ON");
    }
    else
    {
        soundOption.setString("SOUND: OFF");
    }

    newGame.setPosition({ 180.f, 300.f });
    soundOption.setPosition({ 150.f, 370.f });
    exitGame.setPosition({ 180.f, 440.f });

    newGame.setFillColor(sf::Color::White);
    soundOption.setFillColor(sf::Color::White);
    exitGame.setFillColor(sf::Color::White);

    if (selected == 0)
    {
        newGame.setFillColor(sf::Color::Yellow);
    }
    else if (selected == 1)
    {
        soundOption.setFillColor(sf::Color::Yellow);
    }
    else
    {
        exitGame.setFillColor(sf::Color::Yellow);
    }

    window.draw(newGame);
    window.draw(soundOption);
    window.draw(exitGame);
}

// Draw Game Over
void drawGameOver(sf::RenderWindow& window, sf::Font& font, int score)
{
    sf::Text title(font, "GAME OVER", 45);

    title.setFillColor(sf::Color::Red);
    title.setPosition({ 120.f, 200.f });

    sf::Text finalScore(font, "FINAL SCORE: " + to_string(score), 25);

    finalScore.setFillColor(sf::Color::White);
    finalScore.setPosition({ 130.f, 290.f });

    sf::Text message(font, "Press Enter to return to Menu", 20);

    message.setFillColor(sf::Color::Yellow);
    message.setPosition({ 80.f, 380.f });

    window.draw(title);
    window.draw(finalScore);
    window.draw(message);
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

    int score = 0;
    int highScore = loadHighScore();
    int level = 1;
    int  totallines = 0;
    int fallingspeed = 30;

    sf::Font font;

    bool fontLoaded = font.openFromFile("C:/Windows/Fonts/arial.ttf");

    // Background music
    sf::Music music;

    bool musicLoaded = music.openFromFile("tetris_background.wav");

    bool soundOn = true;

    if (musicLoaded)
    {
        music.setLooping(true);
        music.setVolume(30.f);

        music.play();
    }
    else
    {
        cout << "Background music load nahi hui!" << endl;
    }

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

    // Game states
    int gameState = MENU;
    int selected = 0;

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
                // MAIN MENU
                if (gameState == MENU)
                {
                    // Move selection
                    if (key->code == sf::Keyboard::Key::Up)
                    {
                        selected--;

                        if (selected < 0)
                        {
                            selected = 2;
                        }
                    }

                    if (key->code == sf::Keyboard::Key::Down)
                    {
                        selected++;

                        if (selected > 2)
                        {
                            selected = 0;
                        }
                    }

                    // Select option
                    if (key->code == sf::Keyboard::Key::Enter)
                    {
                        // NEW GAME
                        if (selected == 0)
                        {
                            score = 0;
                            level = 1;
                            totallines = 0;
                            fallingspeed = 30;


                            // Empty board
                            for (int r = 0; r < 20; r++)
                            {
                                for (int c = 0; c < 10; c++)
                                {
                                    board[r][c] = 0;
                                }
                            }

                            // First piece
                            current_piece = 0;

                            pieceRow = 0;
                            pieceCol = 3;

                            // Copy first shape
                            for (int r = 0; r < 4; r++)
                            {
                                for (int c = 0; c < 4; c++)
                                {
                                    currentShape[r][c] =
                                        pieces[current_piece][r][c];
                                }
                            }

                            fallCounter = 0;

                            gameState = PLAYING;
                        }

                        // SOUND ON / OFF
                        else if (selected == 1)
                        {
                            soundOn = !soundOn;

                            if (musicLoaded)
                            {
                                if (soundOn)
                                {
                                    music.play();
                                }
                                else
                                {
                                    music.pause();
                                }
                            }
                        }

                        // EXIT
                        else if (selected == 2)
                        {
                            window.close();
                        }
                    }
                }

                // GAME OVER
                else if (gameState == GAME_OVER)
                {
                    if (key->code == sf::Keyboard::Key::Enter)
                    {
                        gameState = MENU;
                        selected = 0;
                    }
                }

                // PLAYING
                else if (gameState == PLAYING)
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
        }

        // Automatic falling
        if (gameState == PLAYING)
        {
            fallCounter++;

            if (fallCounter >= fallingspeed)
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

                    // Clear lines
                    int cleared = clearLines(board);
                    totallines += cleared;
                    level = 1 + totallines / 10;
                    fallingspeed = 30 - (level - 1) * 5;
                    if (fallingspeed < 5)
                        fallingspeed = 5;

                    if (cleared == 1)
                    {
                        score += 100;
                    }
                    else if (cleared == 2)
                    {
                        score += 300;
                    }
                    else if (cleared == 3)
                    {
                        score += 500;
                    }
                    else if (cleared == 4)
                    {
                        score += 800;
                    }

                    // Update High Score
                    checkHighScore(score, highScore);

                    // Select next piece
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
                            currentShape[r][c] =
                                pieces[current_piece][r][c];
                        }
                    }

                    // Check Game Over
                    if (!canPlace(currentShape, pieceRow, pieceCol, board))
                    {
                        gameState = GAME_OVER;
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        // MAIN MENU
        if (gameState == MENU)
        {
            if (fontLoaded)
            {
                drawMenu(window, font, selected, soundOn);
            }
        }

        // PLAYING
        else if (gameState == PLAYING)
        {
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
            if (fontLoaded)
            {
                drawHighScore(window, font, highScore);
                drawScore(window, font, score);
                drawLevel(window, font, level);   // NEW
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

            // Draw scores
            if (fontLoaded)
            {
                drawHighScore(window, font, highScore);
                drawScore(window, font, score);
            }
        }

        // GAME OVER
        else if (gameState == GAME_OVER)
        {
            if (fontLoaded)
            {
                drawGameOver(window, font, score);
            }
        }

        window.display();
    }

    return 0;
}