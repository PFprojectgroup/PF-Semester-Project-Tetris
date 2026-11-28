#ifndef HIGHSCORE_H
#define HIGHSCORE_H

#include <iostream>
#include <fstream>
#include <string>
#include <SFML/Graphics.hpp>

using namespace std;

// reads high score from file
int loadHighScore()
{
    ifstream fin("highscore.txt");
    int best = 0;

    if (fin)
    {
        fin >> best;
        if (best < 0)
            best = 0;
    }

    return best;
}

// saves high score to file
void saveHighScore(int best)
{
    ofstream fout("highscore.txt");
    if (fout)
    {
        fout << best << endl;
    }
}

// checks if score beat the high score
void checkHighScore(int score, int& highScore)
{
    if (score > highScore)
    {
        highScore = score;
        saveHighScore(highScore);
    }
}

// draws high score on screen
void drawHighScore(sf::RenderWindow& window, sf::Font& font, int highScore)
{
    sf::Text text(font, "HIGH SCORE: " + to_string(highScore), 24);
    text.setFillColor(sf::Color::Yellow);
    text.setPosition({ 340.f, 60.f });
    window.draw(text);
}

#endif
