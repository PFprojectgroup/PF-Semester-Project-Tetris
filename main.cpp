#include <SFML/Graphics.hpp>
using namespace std;

int main()
{
    int board[20][10] = {0};
    const float CELL = 30.f;

    const float board_x = 20.f;
    const float board_y = 20.f;

    // Colours for board cells
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

    // Create one rectangle
    sf::RectangleShape block({ CELL - 2, CELL - 2 });
    sf::RenderWindow window(
        sf::VideoMode({ 520, 640 }),
        "Tetris"
    );

    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear(sf::Color::Black);
        for (int r = 0; r < 20; r++)
        {
            for (int c = 0; c < 10; c++)
            {
                block.setFillColor(colors[board[r][c]]);

                block.setPosition({
                    board_x+ c * CELL + 1,
                    board_y+ r * CELL + 1
                    });

                window.draw(block);
            }
        }

        window.display();
    }
    
    return 0;
}