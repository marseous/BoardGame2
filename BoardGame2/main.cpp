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

    bool isSelected() { return selected; }

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

    bool canMove(Vector2f newPosition, vector<Pawn*> player)
    {
        for (int i = 0; i < player.size(); ++i)
                if (newPosition == player[i]->shape.getPosition())
                    return false;
    }

    void makeStep(vector<Pawn*> player)
    {
        if (Keyboard::isKeyPressed(Keyboard::W) && y > 0)
        {
            Vector2f newPosition(x, y - STEP);
            if (canMove(newPosition, player))
            {
                shape.setPosition(x, y += -STEP);
                selected = false;
                setComputerTurn(true);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && y < WINDOW_H - STEP)
        {
            sf::Vector2f newPosition(x, y + STEP);
            if (canMove(newPosition, player))
            {
                shape.setPosition(x, y += STEP);
                selected = false;
                setComputerTurn(true);

            }

        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && x > 0)
        {
            sf::Vector2f newPosition(x - STEP, y);
            if (canMove(newPosition, player))
            {
                shape.setPosition(x += -STEP, y);
                selected = false;
                setComputerTurn(true);
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && x < WINDOW_W - STEP)
        {
            sf::Vector2f newPosition(x + STEP, y);
            if (canMove(newPosition, player))
            {
                shape.setPosition(x += STEP, y);
                selected = false;
                setComputerTurn(true);
            }
        }
    }

    void move(vector<Pawn*> player)
    {
        if (isSelected())
            makeStep(player);
    }
};

bool Pawn::computerTurn = false;



class AI : public Pawn
{
public:
    AI(int x, int y) : Pawn(x, y)
    {
        shape.setFillColor(sf::Color(178, 236, 93));
    }

    bool canMove(Vector2f newPosition, vector<Pawn*> ai)
    {
        for (int i = 0; i < ai.size(); i++)
            if (newPosition == ai[i]->shape.getPosition())
                return false;
    }

    void move(vector<Pawn*> ai) //TODO
    { 
        srand(time(NULL));

        if (Pawn::isComputerTurn())
        {

            Vector2f right(x + STEP, y);
            Vector2f down(x, y + STEP);
           // Vector2f left(x - STEP, y);
           // Vector2f up(x, y - STEP);


            if (shape.getPosition().x < 350 || shape.getPosition().y < 350)
            {
                cout << "AI moved" << endl;
                //cout << i << endl;

                int j = rand() % 2;
                if (j == 0)
                {
                    if (canMove(right, ai) && x < 350)
                    {
                        shape.setPosition(x += STEP, y);
                        Pawn::setComputerTurn(false);
                    }
                    else if (canMove(down, ai) && y < 350)
                    {
                        shape.setPosition(x, y += STEP);
                        Pawn::setComputerTurn(false);
                    }
                }
                if (j == 1)
                {
                    if (canMove(down, ai) && y < 350)
                    {
                        shape.setPosition(x, y += STEP);
                        Pawn::setComputerTurn(false);
                    }
                    else if (canMove(right, ai) && x < 350)
                    {
                        shape.setPosition(x += STEP, y);
                        Pawn::setComputerTurn(false);
                    }
                }
            }
            else if (shape.getPosition().x < 300 || shape.getPosition().y < 300)
            {
                int j = rand() % 2;
                if (j == 0)
                {
                    if (canMove(right, ai) && x < 300)
                    {
                        shape.setPosition(x += STEP, y);
                        Pawn::setComputerTurn(false);
                    }
                    else if (canMove(down, ai) && y < 300)
                    {
                        shape.setPosition(x, y += STEP);
                        Pawn::setComputerTurn(false);
                    }
                }
                if (j == 1)
                {
                    if (canMove(down, ai) && y < 300)
                    {
                        shape.setPosition(x, y += STEP);
                        Pawn::setComputerTurn(false);
                    }
                    else if (canMove(right, ai) && x < 300)
                    {
                        shape.setPosition(x += STEP, y);
                        Pawn::setComputerTurn(false);
                    }
                }
            }
            else if (shape.getPosition().x < 250 || shape.getPosition().y < 250)
            {
                int j = rand() % 2;
                if (j == 0)
                {
                    if (canMove(right, ai) && x < 250)
                    {
                        shape.setPosition(x += STEP, y);
                        Pawn::setComputerTurn(false);
                    }
                    else if (canMove(down, ai) && y < 250)
                    {
                        shape.setPosition(x, y += STEP);
                        Pawn::setComputerTurn(false);
                    }
                }
                if (j == 1)
                {
                    if (canMove(down, ai) && y < 250)
                    {
                        shape.setPosition(x, y += STEP);
                        Pawn::setComputerTurn(false);
                    }
                    else if (canMove(right, ai) && x < 250)
                    {
                        shape.setPosition(x += STEP, y);
                        Pawn::setComputerTurn(false);
                    }
                }
            }
            else 
            {
                if (x >= 300 && y <= 250)
                {
                    shape.setPosition(x -= STEP, y);
                    Pawn::setComputerTurn(false);
                }
                if (x <= 250 && y >= 300)
                {
                    shape.setPosition(x, y -= STEP);
                    Pawn::setComputerTurn(false);
                }
            }
        }
    }
};



int main()
{
    RenderWindow window(VideoMode(WINDOW_W, WINDOW_H), "Game");
    Board board;

    vector<AI*> ai // start position
    {
        new AI(0,0),
        new AI(0,50),
        new AI(0,100),
        new AI(50,00),
        new AI(50,50),
        new AI(50,100),
        new AI(100,0),
        new AI(100,50),
        new AI(100,100)
    };

    vector<Pawn*> player // start position
    {
        new Pawn(250,250),
        new Pawn(250,300),
        new Pawn(250,350),
        new Pawn(300,250),
        new Pawn(300,300),
        new Pawn(300,350),
        new Pawn(350,250),
        new Pawn(350,300),
        new Pawn(350,350)
    };

    player.insert(player.end(), ai.begin(), ai.end());

    
    while (window.isOpen())
    {
        sf::Vector2i cursor = sf::Mouse::getPosition(window);


        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }


     
        for (auto& p : player)
        {
            if (Pawn::isComputerTurn())
            {
                int i = rand() % ai.size();
                ai[i]->move(player);
            }
            else
            {
                p->select(event, cursor.x, cursor.y);
                if (p->isSelected())
                {
                    p->move(player);
                }
            }
        }

        window.clear();
        
        board.drawBoard(window);

        for (auto& i : ai)
        {
            i->shape.setFillColor(sf::Color(178, 236, 93));
            window.draw(i->shape);
        }
        for (auto& p : player)
        {
            window.draw(p->shape);
        }


        window.display();
    }

    return 0;
}