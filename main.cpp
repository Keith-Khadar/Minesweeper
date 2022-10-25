#include <SFML/Graphics.hpp>
#include <fstream>
#include <string>
#include <iostream>
#include <unordered_map>

#include "board.h"


using namespace std;


void ReadTestBoards(string filename, board& game);
board ReadBoardConfig(string filename);
int MousePosToBoardIndex(sf::Vector2f pos, board game);
int MousePosToButton(sf::Vector2f pos, vector<sf::Sprite> buttons);

int main()
{
    /* _____ Initializing Values _____ */

    // board
    board game = ReadBoardConfig("boards/config.cfg");
    game.randomMines();

    enum gamestates { lost = -1, playing = 0, won = 1};
    int gamestate = playing;

    bool debug = false;

    //window 
    sf::RenderWindow window(sf::VideoMode(game._columns * 32, (game._rows * 32) + 100), "Minesweeper");

    // loading textures
    unordered_map<string, sf::Texture> textures;

    sf::Texture texture;

    const string filenames[] = { "debug.png", "digits.png", "face_happy.png", "face_lose.png",
    "face_win.png", "flag.png", "mine.png", "number_1.png", "number_2.png", "number_3.png",
    "number_4.png","number_5.png", "number_6.png", "number_7.png", "number_8.png", "test_1.png",
    "test_2.png", "test_3.png", "tile_hidden.png", "tile_revealed.png" };


    for(int i = 0; i < sizeof(filenames)/sizeof(filenames[0]); i++)
    {
        texture.loadFromFile("images/" + filenames[i]);
        textures.emplace(filenames[i], texture);
    }


    /* _____ Game Loop _____ */
    while (window.isOpen())
    {
        // Store any event that may occur
        sf::Event event;
        

        /* _____ Drawing Sprites _____ */
        window.clear(sf::Color::White);

        int height = textures["tile_hidden.png"].getSize().y;
        int width = textures["tile_hidden.png"].getSize().x;
        int digit = game._mines;

        // Draw board tiles
        sf::Sprite tile;


        int index = 0;        
        for(int y = 0; y < game._rows; y++)
        {
            for (int x = 0; x < game._columns; x++)
            {
                tile.setPosition(((float)x * width), ((float)y * height));
               
                switch(game.currentBoard.at(index))
                {
                case board::tile_hidden:
                    tile.setTexture(textures["tile_hidden.png"]);
                    window.draw(tile);
                    break;
                case board::tile_revealed:
                    tile.setTexture(textures["tile_revealed.png"]);
                    window.draw(tile);
                    break;
                case board::flag:
                    tile.setTexture(textures["tile_hidden.png"]);
                    window.draw(tile);
                    tile.setTexture(textures["flag.png"]);
                    window.draw(tile);
                    digit--;
                    break;
                case board::mine:
                    tile.setTexture(textures["tile_revealed.png"]);
                    window.draw(tile);
                    tile.setTexture(textures["mine.png"]);
                    window.draw(tile);
                    break;
                default:
                    tile.setTexture(textures["tile_revealed.png"]);
                    window.draw(tile);
                    tile.setTexture(textures["number_" + to_string(game.currentBoard.at(index)) + ".png"]);
                    window.draw(tile);
                }
                if(debug && game.mineBoard.at(index) == 11)
                {
                    tile.setTexture(textures["mine.png"]);
                    window.draw(tile);
                }

                index++;

            }
        }

        // Draw timer
        sf::Sprite timer;
        timer.setTexture(textures["digits.png"]);

        if(digit < 0) // Check if a negative sign needs to be drawn to the window
        {
            timer.setTextureRect(sf::IntRect((21 * 10), 0, 21, 32));
            timer.setPosition(0.f, (game._rows * height));
            window.draw(timer);

            digit *= -1;
        }
        timer.setTextureRect(sf::IntRect((21 * (digit/100)), 0, 21, 32));
        timer.setPosition(21.f, (game._rows * height));
        window.draw(timer);

        timer.setTextureRect(sf::IntRect((21 * ((digit % 100)/10)), 0, 21, 32));
        timer.setPosition(42.f, (game._rows * height));
        window.draw(timer);

        timer.setTextureRect(sf::IntRect((21 * (digit % 10)), 0, 21, 32));
        timer.setPosition(63.f, (game._rows * height));
        window.draw(timer);


        // Draw buttons
        sf::Sprite restartButton;

        if(gamestate == playing)
        {
            restartButton.setTexture(textures["face_happy.png"]);
        }
        else if(gamestate == won)
        {
            restartButton.setTexture(textures["face_win.png"]);
           
            // draw flags where all the mines were
            index = 0;
            for (int j = 0; j < game._rows; j++)
            {
                for (int i = 0; i < game._columns; i++)
                {
                    if (game.mineBoard.at(index) == 11)
                    {
                        game.currentBoard.at(index) = 10;
                    }
                    index++;
                }
            }
        }
        else if(gamestate == lost)
        {
            restartButton.setTexture(textures["face_lose.png"]);
        }

        vector<sf::Sprite> buttons;

        restartButton.setPosition((game._columns * width)/2 - (restartButton.getTextureRect().width / 2), (game._rows * height));
        window.draw(restartButton);
        buttons.push_back(restartButton);

        sf::Sprite debugButton;
        debugButton.setTexture(textures["debug.png"]);
        debugButton.setPosition((game._columns * width) / 2 + (debugButton.getTextureRect().width * 1.5f), (game._rows * height));
        window.draw(debugButton);
        buttons.push_back(debugButton);

        sf::Sprite test_1Button;
        test_1Button.setTexture(textures["test_1.png"]);
        test_1Button.setPosition((game._columns * width) / 2 + (test_1Button.getTextureRect().width * 2.5f), (game._rows * height));
        window.draw(test_1Button);
        buttons.push_back(test_1Button);

        sf::Sprite test_2Button;
        test_2Button.setTexture(textures["test_2.png"]);
        test_2Button.setPosition((game._columns * width) / 2 + (test_2Button.getTextureRect().width * 3.5f), (game._rows * height));
        window.draw(test_2Button);
        buttons.push_back(test_2Button);

        sf::Sprite test_3Button;
        test_3Button.setTexture(textures["test_3.png"]);
        test_3Button.setPosition((game._columns * width) / 2 + (test_3Button.getTextureRect().width * 4.5f), (game._rows * height));
        window.draw(test_3Button);
        buttons.push_back(test_3Button);


        window.display();

        /* _____ Testing Win condition _____ */

        if (gamestate != lost) 
        {
            gamestate = won;

            int index = 0;
            for (int y = 0; y < game._rows; y++)
            {
                for (int x = 0; x < game._columns; x++)
                {
                    // If there are still hidden tiles that do not have bombs under them then the game is still going on
                    // Or if there are misplaced flags
                    if ((game.currentBoard.at(index) == board::tile_hidden && game.mineBoard.at(index) != board::mine) || 
                        (game.currentBoard.at(index) == board::flag && game.mineBoard.at(index) != board::mine))
                    {
                        gamestate = playing;
                    }
                    index++;
                }
            }
        }

        /* _____ Window Events _____ */
        while (window.pollEvent(event))
        {

            switch (event.type)
            {
            // closing the window
            case sf::Event::Closed:
                window.close();
                break;
            // Detecting Mouse input
            case sf::Event::MouseButtonPressed:
                // Left Mouse button clicked
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                {
                    if (gamestate == playing)
                    {

                        // Detecting Mouse input over the grid/board
                        int index = MousePosToBoardIndex(window.mapPixelToCoords(sf::Mouse::getPosition(window)), game);
                        if (index >= 0) // if where you clicked is valid
                        {
                            if (game.mineBoard.at(index) == board::mine)
                            {
                                gamestate = lost;

                                // draw all the mines to the board
                                for (int i = 0; i < game.mineBoard.size(); i++)
                                {
                                    if (game.mineBoard.at(i) == 11)
                                    {
                                        game.currentBoard.at(i) = 11;
                                    }
                                }
                            }
                            else if (game.currentBoard.at(index) == board::tile_hidden)
                            {
                                game.revealBoard(index);
                            }

                        }
                    }

                    // Detecting Mouse input over the buttons
                    switch (MousePosToButton(window.mapPixelToCoords(sf::Mouse::getPosition(window)), buttons))
                    {
                        // The corresponding number for each button is determined by its position in the vector
                    case 0:
                        game = ReadBoardConfig("boards/config.cfg");
                        game.randomMines();
                        gamestate = 0;
                        break;
                    case 1:
                        debug = !debug;
                        break;
                    case 2:
                        ReadTestBoards("boards/testboard1.brd", game);
                        gamestate = 0;
                        break;
                    case 3:
                        ReadTestBoards("boards/testboard2.brd", game);
                        gamestate = 0;
                        break;
                    case 4:
                        ReadTestBoards("boards/testboard3.brd", game);
                        gamestate = 0;
                        break;
                    }

                }

                // Detecting Right Mouse click over the grid/board
                if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && gamestate == playing)
                {
                    int index = MousePosToBoardIndex(window.mapPixelToCoords(sf::Mouse::getPosition(window)), game);
                    if (index >= 0)
                    {
                        if (game.currentBoard.at(index) == board::tile_hidden)
                        {
                            game.currentBoard.at(index) = board::flag;
                        }
                        else if (game.currentBoard.at(index) == board::flag)
                        {
                            game.currentBoard.at(index) = board::tile_hidden;
                        }


                    }

                }
            }
        }
        }

    return 0;
}

board ReadBoardConfig(string filename)
{
    int columns;
    int rows;
    int mines;

    ifstream file(filename);
    string buffer;

    getline(file, buffer);
    columns = stoi(buffer);

    getline(file, buffer);
    rows = stoi(buffer);

    getline(file, buffer);
    mines = stoi(buffer);

    return board(columns, rows, mines);

}
void ReadTestBoards(string filename, board& game)
{
    game = ReadBoardConfig("boards/config.cfg");
    vector<int> mines;

    ifstream file(filename);
    char buffer;
    string endline;
    for (int j = 0; j < game._rows; j++)
    {
        for (int i = 0; i < game._columns; i++)
        {
            file >> buffer;
            mines.push_back(((int)buffer - '0'));
        }
        getline(file, endline);
    }
   
    game.placeMines(mines);
}

int MousePosToBoardIndex(sf::Vector2f pos, board game)
{
    int x = pos.x/32;
    int y = pos.y/32;

    if(x >= game._columns || y >= game._rows)
    {
        return -1;
    }
    
    return (x + (game._columns * y));
}

int MousePosToButton(sf::Vector2f pos, vector<sf::Sprite> buttons)
{
    int index = 0;

    for (int i = 0; i < buttons.size(); i++)
    {
        if(pos.x < buttons.at(i).getPosition().x + buttons.at(i).getTextureRect().width && pos.x > buttons.at(i).getPosition().x &&
            pos.y < buttons.at(i).getPosition().y + buttons.at(i).getTextureRect().height && pos.y > buttons.at(i).getPosition().y)
        {
            return i;
        }
    }
    return -1;
}
