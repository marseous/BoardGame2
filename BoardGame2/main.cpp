#include <iostream>
#include <SFML/Graphics.hpp>

#define BOARD_W 8
#define BOARD_H 8
#define WINDOW_W 400
#define WINDOW_H 400
#define STEP WINDOW_W / BOARD_W

using namespace sf;
using namespace std;

class Board
{
private:
    RectangleShape whiteCell;
    RectangleShape blackCell;
public:
    Board()
    {
        whiteCell.setSize(sf::Vector2f(50, 50));
        blackCell.setSize(sf::Vector2f(50, 50));
        whiteCell.setFillColor(sf::Color(130, 130, 130));
        blackCell.setFillColor(sf::Color(53, 56, 57));
    }

    void drawBoard(RenderWindow& window)
    {
        for (int j = 0; j < BOARD_H; ++j)
            for (int i = 0; i < BOARD_W; ++i)
                if (j % 2 == 0)
                {
                    whiteCell.setPosition(STEP * i, STEP * j);
                    window.draw(whiteCell);
                    blackCell.setPosition(STEP * ++i, STEP * j);
                    window.draw(blackCell);
                }
                else
                {
                    blackCell.setPosition(STEP * i, STEP * j);
                    window.draw(blackCell);
                    whiteCell.setPosition(STEP * ++i, STEP * j);
                    window.draw(whiteCell);
                }
    }

};



class Pawn
{
private:
    int x, y;
    bool selected;
    static bool computerTurn;

public:
    CircleShape shape;

    Pawn(int X, int Y) : x(X), y(Y)
    {
        shape.setRadius(25.f);
        shape.setFillColor(sf::Color(70, 130, 180));
        shape.setOutlineColor(sf::Color(65, 105, 225));
        shape.setOutlineThickness(2.f);
        shape.setPosition(x, y);

        selected = false;
    }
    static void setComputerTurn(bool state) { computerTurn = state; }
    static bool isComputerTurn() { return computerTurn; }

    void select(Event event, int cursorX, int cursorY)
    {
        if (event.key.code == sf::Mouse::isButtonPressed(sf::Mouse::Left))
            if (shape.getGlobalBounds().contains(cursorX, cursorY))
            {
                selected = true;
                shape.setFillColor(sf::Color(0, 100, 0));
            }
            else
            {
                selected = false;
                shape.setFillColor(sf::Color(70, 130, 180));
            }
    }
    bool isSelected() { return selected; }
    bool canMove(Vector2f newPosition)
    {
        if (newPosition == Vector2f(350,250)) // TODO check real pawn
        {
            return false;
        }
        else
        {
            return true;
        }
    }
    void makeStep()
    {
        if (Keyboard::isKeyPressed(Keyboard::W) && y > 0)
        {
            Vector2f newPosition(x, y - STEP);
            if (canMove(newPosition))
            {
                shape.setPosition(x, y += -STEP);
                selected = false;
                setComputerTurn(true);

            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && y < WINDOW_H - STEP)
        {
            sf::Vector2f newPosition(x, y + STEP);
            if (canMove(newPosition))
            {
                shape.setPosition(x, y += STEP);
                selected = false;

            }

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && x > 0)
        {
            sf::Vector2f newPosition(x - STEP, y);
            if (canMove(newPosition))
            {
                shape.setPosition(x += -STEP, y);
                selected = false;

            }

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && x < WINDOW_W - STEP)
        {
            sf::Vector2f newPosition(x + STEP, y);
            if (canMove(newPosition))
            {
                shape.setPosition(x += STEP, y);
                selected = false;

            }

        }

    }
    void move()
    {
        if (isSelected())
        {
            makeStep();

        }
        else
        {

        }
    }



};

bool Pawn::computerTurn = false;


class AI : public Pawn
{
private:

public:
    AI(int x, int y) : Pawn(x, y)
    {
        shape.setFillColor(sf::Color(178, 236, 93));
    }

    void move() 
    { 
        cout << "AI moved" << endl; 
        Pawn::setComputerTurn(false);
    }

};



int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Game");
    Board board;
    Pawn p(350, 350);
    Pawn p2(350, 250);

    AI ai(150, 150);

    while (window.isOpen())
    {
        sf::Vector2i cursor = sf::Mouse::getPosition(window);


        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (!Pawn::isComputerTurn())
        {
            p.select(event, cursor.x, cursor.y);
            if (p.isSelected())
            {
                p.move();

            }

        }
        else
        {
            ai.move();
        }

        window.clear();

        cout << Pawn::isComputerTurn() << endl;

        board.drawBoard(window);

        window.draw(p.shape);
        window.draw(p2.shape);
        window.draw(ai.shape);


        window.display();
    }

    return 0;
}