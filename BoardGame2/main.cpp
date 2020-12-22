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
    bool selected;
    static bool computerTurn;

protected:
    int x, y;

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

    int getX() { return x; }
    int getY() { return y; }

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
        if (newPosition == Vector2f(200, 200)) // TODO check real pawn
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
                setComputerTurn(true);

            }

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && x > 0)
        {
            sf::Vector2f newPosition(x - STEP, y);
            if (canMove(newPosition))
            {
                shape.setPosition(x += -STEP, y);
                selected = false;
                setComputerTurn(true);

            }

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && x < WINDOW_W - STEP)
        {
            sf::Vector2f newPosition(x + STEP, y);
            if (canMove(newPosition))
            {
                shape.setPosition(x += STEP, y);
                selected = false;
                setComputerTurn(true);

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



    void select() {};
    void canMove()
    {

    }

    void move(vector<AI*> ai) 
    { 
        if (Pawn::isComputerTurn())
        {




            Vector2f finalPosition(350, 350);
            int i = rand() % 2;
            while (ai[i]->x < 350 && ai[i]->y < 350)
            {
                cout << "AI moved" << endl;
                shape.setPosition(x, y += STEP);
                Pawn::setComputerTurn(false);

                break;
            }
        }

    }

};



int main()
{
    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Game");

    srand(time(NULL));

    Board board;
    Pawn p(350, 350);
    Pawn p2(200, 200);

    //AI ai(200, 0);
    vector<AI*> ai
    {
        new AI(100,100),
        new AI(100,150)
    };

    
    while (window.isOpen())
    {
        sf::Vector2i cursor = sf::Mouse::getPosition(window);


        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (Pawn::isComputerTurn())
        {
            while (Pawn::isComputerTurn())
            {
                for (auto& i : ai)
                {
                    i->move(ai);
                }
            }
           
        }
        else
        {
            p.select(event, cursor.x, cursor.y);
            if (p.isSelected())
            {
                p.move();

            }

        }

        window.clear();

        //cout << Pawn::isComputerTurn() << endl;

        board.drawBoard(window);

        window.draw(p.shape);
        window.draw(p2.shape);

        for (auto& i : ai)
        {
            window.draw(i->shape);
        }


        window.display();
    }

    return 0;
}