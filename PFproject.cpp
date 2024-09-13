#include <SFML/Graphics.hpp>

#include <SFML/Audio.hpp>
#include <iostream>
#include<fstream>

using namespace std;
using namespace sf;
//constant values
const int numRows = 5;
const int numCols = 5;
const float circleSize = 35.f;
const float spacing = 144.f;

int board[numRows][numCols] = {
    {1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1},
    {1, 1, 0, 2, 2},
    {2, 2, 2, 2, 2},
    {2, 2, 2, 2, 2},
};
//data type
enum GameState
{
    MainMenu,
    toss,
    Playing,
    RULE,
    Exiting,
    resume,
};
GameState gameState = MainMenu;

const int EMPTY = 0;
const int RED = 1;
const int BLUE = 2;
//for coin toss
Clock vibrationClock;

bool tossAnimationComplete = false;

//function for saving game
void saveGame(const string& filePath, const int board[][numCols], int selectedBeadX, int selectedBeadY, int destinationBeadX, int destinationBeadY, int selectedBeadColor)
{
    //ow
    ofstream file(filePath);

    if (file.is_open())
    {
        // Save boa
        // rd state
        for (int i = 0; i < numRows; ++i)
        {
            for (int j = 0; j < numCols; ++j)
            {
                file << board[i][j] << " ";
            }
            file << endl;
        }

        // Save selected bead information
        file << selectedBeadX << " " << selectedBeadY << endl;
        file << destinationBeadX << " " << destinationBeadY << endl;
        file << selectedBeadColor << endl;

        file.close();
        cout << "Game saved successfully." << endl;
    }
    else
        cout << "Unable to open file for saving." << endl;
}
//function for loading game
void loadGame(const string& filePath, int board[][numCols], int& selectedBeadX, int& selectedBeadY, int& destinationBeadX, int& destinationBeadY, int& selectedBeadColor)
{
    //ir
    ifstream file(filePath);

    if (file.is_open())
    {
        // Load board state
        for (int i = 0; i < numRows; ++i)
        {
            for (int j = 0; j < numCols; ++j)
            {
                file >> board[i][j];
            }
        }

        // Load selected bead information
        file >> selectedBeadX >> selectedBeadY;
        file >> destinationBeadX >> destinationBeadY;
        file >> selectedBeadColor;

        file.close();
        cout << "Game loaded successfully." << endl;
    }
    else
    {
        cout << "Unable to open file for loading." << endl;
    }
}



void animateCoinToss(RenderWindow& window)
{
    const int numFrames = 20;
    const float rotationPerFrame = 16.0f;

    Texture coinTexture;
    if (!coinTexture.loadFromFile("C:\\c3.png")) {
        cout<< "Image not found" << endl;
        return;
    }

    Sprite coinSprite(coinTexture);
    coinSprite.setScale(0.5f, 0.5f);
    coinSprite.setPosition(400, 200);
    //for numbers of rotation
    Clock animationClock;

    srand(time(NULL));
    int frames = rand() % numFrames + numFrames * 4;  // Adjust frame count for more spins

    for (int i = 0; i < frames; ++i) 
    {
        coinSprite.rotate(rotationPerFrame);

        // window.clear();
        window.draw(coinSprite);
        window.display();
        window.clear();

        sf::sleep(sf::milliseconds(40)); // animation speed
    }

    tossAnimationComplete = true;
}
bool isGameOver() 
{
    int redBeads = 0;
    int blueBeads = 0;

    // Count the number of red and blue beads on the board
    for (int i = 0; i < numRows; ++i) 
    {
        for (int j = 0; j < numCols; ++j) 
        {
            if (board[i][j] == RED)
            {
                redBeads++;
            }
            else if (board[i][j] == BLUE)
            {
                blueBeads++;
            }
        }
    }

    // Check if any player has no beads left
    if (redBeads == 0) {
        cout << "white Player Wins!" << endl;
        return true;
    }
    else if (blueBeads == 0) {
        cout << "blue Player Wins!" << endl;
        return true;
    }
    return false;
}

//main start
int main()
{

    int selectedBeadX = -1;
    int selectedBeadY = -1;
    int destinationBeadX = -1;
    int destinationBeadY = -1;
    int selectedBeadColor = EMPTY;
    //sfml circle//circlesize parameter
    sf::CircleShape bead(circleSize);
    //window created
    sf::RenderWindow window(sf::VideoMode(880, 890), "12 BEAD GAME");
    //FONT STYLE
    Font font;
    if (!font.loadFromFile("C:\\pacifico.ttf"))
    {
        cout << "Failed to load font!" << endl;
        return 0;
    }
    //board back 1(brown menu) picture
    sf::Texture back;
    if (!back.loadFromFile("C:\\back.jpg"))
    {
        cerr << "Image not found" << endl;
        return 0;
    }
    sf::Sprite backSprite(back);
    backSprite.setScale(780.f / backSprite.getLocalBounds().width, 780.f / backSprite.getLocalBounds().height);
    //board back 1(menu) pic ends

    //text display
    Text beadText("12 BEADS GAME", font, 65);
    beadText.setPosition(150, 100);
    beadText.setFillColor(sf::Color::Black);
    beadText.setStyle(sf::Text::Style::Bold | sf::Text::Style::Underlined);
    beadText.setOutlineColor(sf::Color::Cyan);

    Text tossText("Toss", font, 60);
    tossText.setPosition(350, 200);
    tossText.setFillColor(sf::Color::Black);
    tossText.setStyle(sf::Text::Style::Bold);

    Text playText("Play", font, 60);
    playText.setPosition(350, 280);
    playText.setFillColor(sf::Color::Black);
    playText.setStyle(sf::Text::Style::Bold);

    Text rulesText("Rules", font, 60);
    rulesText.setPosition(350, 360);
    rulesText.setFillColor(sf::Color::Black);
    rulesText.setStyle(sf::Text::Style::Bold);


    Text resumeText("Resume", font, 60);
    resumeText.setPosition(350, 440);
    resumeText.setFillColor(sf::Color::Black);
    resumeText.setStyle(sf::Text::Style::Bold);


    Text exitText("Exit", font, 60);
    exitText.setPosition(350, 520);
    exitText.setFillColor(sf::Color::Black);
    exitText.setStyle(sf::Text::Style::Bold);

    Text cyan(" cyan win  ", font, 70);
    cyan.setPosition(340, 360);
    cyan.setFillColor(sf::Color::Black);
    cyan.setStyle(sf::Text::Style::Bold);

    Text white(" magenta win  ", font, 70);
    white.setPosition(340, 360);
    white.setFillColor(sf::Color::Black);
    white.setStyle(sf::Text::Style::Bold);

    //Text tie("OPPS! its tie  ", font, 70);
    //tie.setPosition(200, 450);
    //tie.setFillColor(sf::Color::Black);
    //tie.setStyle(sf::Text::Style::Bold);

    Text pause1(" BACK  ", font, 40);
    pause1.setPosition(600, 400);
    pause1.setFillColor(sf::Color::Black);
    pause1.setStyle(sf::Text::Style::Bold);

    Text pause("  PAUSE  ", font, 52);
    pause.setPosition(630, 400);
    pause.setFillColor(sf::Color::White);
    pause.setStyle(sf::Text::Style::Bold);
    //winner image
    sf::Texture winner;
    if (!winner.loadFromFile("C:\\win.png"))
    {
        cerr << "Image not found" << endl;
        return 0;
    }

    sf::Sprite winnerSprite(winner);
    winnerSprite.setScale(880.f / winnerSprite.getLocalBounds().width, 880.f / winnerSprite.getLocalBounds().height);
    //winner image

    //board game picture
    sf::Texture boardpic;
    if (!boardpic.loadFromFile("C:\\boardss.png"))
    {
        cout << "Image not found" << endl;
        return 0;
    }

    sf::Sprite boardSprite(boardpic);
    boardSprite.setScale(660.f / boardSprite.getLocalBounds().width, 660.f / boardSprite.getLocalBounds().height);
    //board piture ends

    //SOUND EFFECTS

    //illegal move
    sf::SoundBuffer buffer;
    if (!buffer.loadFromFile("C:\\buzzer.mp3"))
    {
        cout << "Failed to load sound effect!" << endl;
        return -1;
    }
    sf::Sound sound;
    sound.setBuffer(buffer);
    //eliminate sound
    sf::SoundBuffer eliminationSoundBuffer;
    if (!eliminationSoundBuffer.loadFromFile("C:\\sound2.wav"))
    {
        cout << "Failed to load elimination sound effect!" << endl;
        return -1;
    }
    sf::Sound eliminationSound;
    eliminationSound.setBuffer(eliminationSoundBuffer);

    // click sound
    sf::SoundBuffer clickSoundBuffer;
    if (!clickSoundBuffer.loadFromFile("C:\\click.wav"))
    {
        cerr << "Failed to load click sound effect!" << endl;
        return -1;
    }
    sf::Sound clickSound;
    clickSound.setBuffer(clickSoundBuffer);

    // click sound ends

    //red bead texture
    sf::Texture redBeadTexture;
    if (!redBeadTexture.loadFromFile("C:\\texture.jpg")) {
        cerr << "Failed to load red bead texture!" << endl;
        return -1;
    }
    //blue bead texture
    sf::Texture blueBeadTexture;
    if (!blueBeadTexture.loadFromFile("C:\\texture.jpg")) {
        cerr << "Failed to load blue bead texture!" << endl;
        return -1;
    }
    //rules page sprite

    sf::Texture rule;
    if (!rule.loadFromFile("C:\\rules.png"))
    {
        cout << "Image not found" << endl;
        return -1;
    }

    sf::Sprite ruleSprite(rule);
    ruleSprite.setScale(580.f / ruleSprite.getLocalBounds().width, 880.f / ruleSprite.getLocalBounds().height);
    //rules page sprite ends

    //window starts
    while (window.isOpen())
    {
        sf::Event event;
        //pending events
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                gameState = Exiting;
            }
            else if (event.type == sf::Event::MouseButtonPressed)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                //SWITCH CASES
                switch (gameState)
                {
                case MainMenu:
                    //case1 
                    if (tossText.getGlobalBounds().contains(mousePos))
                    {
                        gameState = toss;
                    }

                    else if (playText.getGlobalBounds().contains(mousePos))
                    {
                        gameState = Playing;
                    }
                    else if (rulesText.getGlobalBounds().contains(mousePos))
                    {
                        gameState = RULE;
                        //pause (back)
                        if (pause.getGlobalBounds().contains(mousePos))
                            gameState = MainMenu;
                    }
                    else if (exitText.getGlobalBounds().contains(mousePos))
                    {
                        gameState = Exiting;
                    }
                    else if (resumeText.getGlobalBounds().contains(mousePos))
                    {
                        // Load the game when the user clicks the resume button
                        loadGame("resumegame.txt", board, selectedBeadX, selectedBeadY, destinationBeadX, destinationBeadY, selectedBeadColor);
                        gameState = Playing;
                    }
                    clickSound.play();
                    break;
                    //case2
                case toss:
                    //size+position bounds
                    if (tossText.getGlobalBounds().contains(mousePos))
                    {

                        animateCoinToss(window);
                        /*  srand(time(NULL));
                          int coinResult = rand() % 2;
                          if (coinResult == 0)
                          {
                              window.draw(cyan);

                          }
                          else
                          {
                              window.draw(white);
                          }*/
                          // Move to the Playing state after the toss
                    }
                    gameState = Playing;
                    break;
                    //case3
                case RULE:
                    if (rulesText.getGlobalBounds().contains(mousePos))
                    {
                        gameState = RULE;
                        /*  window.draw(backSprite);
                          window.draw(ruleSprite);
                          window.draw(pause1);*/
                    }
                    if (pause1.getGlobalBounds().contains(mousePos))
                        gameState = MainMenu;

                    break;
                    //case4
                case Playing:
                    window.draw(pause);
                    if (pause.getGlobalBounds().contains(mousePos))
                        gameState = MainMenu;
                    for (int i = 0; i < numRows; ++i)
                    {
                        for (int j = 0; j < numCols; ++j)
                        {
                            FloatRect beadBounds(j * spacing + 5.f, i * spacing + 5.f, circleSize, circleSize);
                            if (beadBounds.contains(mousePos))
                            {
                                cout << "bead clicked at" << "(" << i << "," << j << ")" << endl;
                                //first click
                                //X col
                                //Y row
                                if (selectedBeadX == -1 || selectedBeadY == -1)
                                {
                                    selectedBeadX = j;
                                    selectedBeadY = i;
                                    selectedBeadColor = board[i][j];
                                }
                                else {
                                    destinationBeadX = j;
                                    destinationBeadY = i;
                                    // Check for valid moves and eliminate conditions
                                    if ((selectedBeadX == 0 && selectedBeadY == 1) && (destinationBeadX == 1 && destinationBeadY == 0) ||
                                        (selectedBeadX == 1 && selectedBeadY == 2) && (destinationBeadX == 0 && destinationBeadY == 1) ||
                                        (selectedBeadX == 0 && selectedBeadY == 3) && (destinationBeadX == 1 && destinationBeadY == 2) ||
                                        (selectedBeadX == 1 && selectedBeadY == 4) && (destinationBeadX == 0 && destinationBeadY == 3) ||
                                        (selectedBeadX == 2 && selectedBeadY == 1) && (destinationBeadX == 1 && destinationBeadY == 0) ||
                                        (selectedBeadX == 1 && selectedBeadY == 2) && (destinationBeadX == 2 && destinationBeadY == 1) ||
                                        (selectedBeadX == 2 && selectedBeadY == 3) && (destinationBeadX == 1 && destinationBeadY == 2) ||
                                        (selectedBeadX == 1 && selectedBeadY == 4) && (destinationBeadX == 2 && destinationBeadY == 3) ||
                                        (selectedBeadX == 2 && selectedBeadY == 1) && (destinationBeadX == 3 && destinationBeadY == 0) ||
                                        (selectedBeadX == 3 && selectedBeadY == 2) && (destinationBeadX == 2 && destinationBeadY == 1) ||
                                        (selectedBeadX == 3 && selectedBeadY == 2) && (destinationBeadX == 3 && destinationBeadY == 2) ||
                                        (selectedBeadX == 3 && selectedBeadY == 4) && (destinationBeadX == 2 && destinationBeadY == 3) ||
                                        (selectedBeadX == 4 && selectedBeadY == 1) && (destinationBeadX == 3 && destinationBeadY == 0) ||
                                        (selectedBeadX == 3 && selectedBeadY == 2) && (destinationBeadX == 4 && destinationBeadY == 1) ||
                                        (selectedBeadX == 4 && selectedBeadY == 3) && (destinationBeadX == 3 && destinationBeadY == 2) ||
                                        (selectedBeadY == 3 && selectedBeadX == 4) && (destinationBeadX == 4 && destinationBeadY == 3) ||
                                        (selectedBeadY == 2 && selectedBeadX == 3) && (destinationBeadX == 3 && destinationBeadY == 2) ||
                                        (destinationBeadX == 0 && destinationBeadY == 1) && (selectedBeadX == 1 && selectedBeadY == 0) ||
                                        (destinationBeadX == 1 && destinationBeadY == 2) && (selectedBeadX == 0 && selectedBeadY == 1) ||
                                        (destinationBeadX == 0 && destinationBeadY == 3) && (selectedBeadX == 1 && selectedBeadY == 2) ||
                                        (destinationBeadX == 1 && destinationBeadY == 4) && (selectedBeadX == 0 && selectedBeadY == 3) ||
                                        (destinationBeadX == 2 && destinationBeadY == 1) && (selectedBeadX == 1 && selectedBeadY == 0) ||
                                        (destinationBeadX == 1 && destinationBeadY == 2) && (selectedBeadX == 2 && selectedBeadY == 1) ||
                                        (destinationBeadX == 2 && destinationBeadY == 3) && (selectedBeadX == 1 && selectedBeadY == 2) ||
                                        (destinationBeadX == 1 && destinationBeadY == 4) && (selectedBeadX == 2 && selectedBeadY == 3) ||
                                        (destinationBeadX == 2 && destinationBeadY == 1) && (selectedBeadX == 3 && selectedBeadY == 0) ||
                                        (destinationBeadX == 3 && destinationBeadY == 2) && (selectedBeadX == 2 && selectedBeadY == 1) ||
                                        (destinationBeadX == 3 && destinationBeadY == 2) && (selectedBeadX == 3 && selectedBeadY == 2) ||
                                        (destinationBeadX == 3 && destinationBeadY == 4) && (selectedBeadX == 2 && selectedBeadY == 3) ||
                                        (destinationBeadX == 4 && destinationBeadY == 1) && (selectedBeadX == 3 && selectedBeadY == 0) ||
                                        (destinationBeadX == 3 && destinationBeadY == 2) && (selectedBeadX == 4 && selectedBeadY == 1) ||
                                        (destinationBeadX == 4 && destinationBeadY == 3) && (selectedBeadX == 3 && selectedBeadY == 2) ||
                                        (destinationBeadY == 3 && destinationBeadX == 4) && (selectedBeadX == 4 && selectedBeadY == 3) ||
                                        (destinationBeadY == 2 && destinationBeadX == 3) && (selectedBeadX == 3 && selectedBeadY == 2))
                                    {
                                        // Example: Check if the movement is across the top-right to bottom-left diagonal
                                        cout << "Movement across the diagonal detected MAKE YOUR MOVE BACK OR TRY AGAIN!" << endl;
                                        sound.play();
                                    }
                                    if (abs(selectedBeadY - destinationBeadY) > 2 || abs(selectedBeadX - destinationBeadX) > 2)
                                    {
                                        return 0;
                                    }
                                    // Check and eliminate conditions for vertical lines
                                    if (selectedBeadX == destinationBeadX && selectedBeadY != destinationBeadY)
                                    {
                                        int step = (selectedBeadY < destinationBeadY) ? 1 : -1;
                                        for (int i = selectedBeadY + step; i != destinationBeadY; i += step)
                                        {
                                            if (board[i][selectedBeadX] != selectedBeadColor)
                                            {
                                                // Eliminate the opposite-colored bead
                                                board[i][selectedBeadX] = EMPTY;
                                                cout << "BEAD ELIMINATED!" << endl;
                                                eliminationSound.play();
                                            }

                                            else {
                                                sound.play();
                                                cout << "ILLEGAL MOVE! TAKE YOUR MOVE BACK OR TRY AGAIN!";
                                            }
                                        }
                                    }
                                    // Check and eliminate conditions for horizontal lines
                                    else if (selectedBeadY == destinationBeadY && selectedBeadX != destinationBeadX)
                                    {
                                        int step = (selectedBeadX < destinationBeadX) ? 1 : -1;
                                        for (int j = selectedBeadX + step; j != destinationBeadX; j += step)
                                        {
                                            if (board[selectedBeadY][j] != selectedBeadColor)
                                            {
                                                // Eliminate the opposite-colored bead
                                                board[selectedBeadY][j] = EMPTY;
                                                cout << "BEAD ELIMINATED!" << endl;
                                                eliminationSound.play();

                                            }
                                            else {
                                                sound.play();
                                                cout << "ILLEGAL MOVE! TAKE YOUR MOVE BACK OR TRY AGAIN!";
                                            }
                                        }
                                    }
                                    // Check and eliminate conditions for diagonal lines
                                    else if (abs(selectedBeadX - destinationBeadX) == abs(selectedBeadY - destinationBeadY) &&
                                        selectedBeadX != destinationBeadX)
                                    {
                                        int stepX = (selectedBeadX < destinationBeadX) ? 1 : -1;
                                        int stepY = (selectedBeadY < destinationBeadY) ? 1 : -1;
                                        for (int i = selectedBeadY + stepY, j = selectedBeadX + stepX;
                                            i != destinationBeadY && j != destinationBeadX;
                                            i += stepY, j += stepX)
                                        {
                                            if (board[i][j] != selectedBeadColor)
                                            {
                                                // Eliminate the opposite-colored bead
                                                board[i][j] = EMPTY;
                                                cout << "BEAD ELIMINATED!" << endl;
                                                eliminationSound.play();
                                            }
                                            else {
                                                sound.play();
                                                cout << "ILLEGAL MOVE! TAKE YOUR MOVE BACK OR TRY AGAIN!";
                                            }

                                        }
                                    }


                                    //move bead if the destion place is empty
                                    if (board[destinationBeadY][destinationBeadX] == EMPTY)
                                    {
                                        board[selectedBeadY][selectedBeadX] = EMPTY;
                                        board[destinationBeadY][destinationBeadX] = selectedBeadColor;

                                        //yaha pe save game hogi
                                        saveGame("resumegame.txt", board, selectedBeadX, selectedBeadY, destinationBeadX, destinationBeadY, selectedBeadColor);
                                    }
                                    //else play sound
                                    else
                                    {
                                        sound.play();

                                    }
                                    //reseting all conditions
                                    selectedBeadX = -1;
                                    selectedBeadY = -1;
                                    destinationBeadX = -1;
                                    destinationBeadY = -1;

                                }
                            }
                        }
                    }
                    clickSound.play();
                    break;
                }
            }
        }

        window.clear();
        //swtich 2 for drawing
        switch (gameState)
        {
        case MainMenu:
            window.draw(backSprite);
            window.draw(beadText);
            window.draw(tossText);
            window.draw(playText);
            window.draw(rulesText);
            window.draw(resumeText);
            window.draw(exitText);
            break;
        case toss:
            if (!tossAnimationComplete)
            {
                animateCoinToss(window);
            }
            else
            {
                window.draw(backSprite);
                srand(time(NULL));
                int coinResult = rand() % 2;
                if (coinResult == 0)
                {
                    window.draw(cyan);
                    cout << "blue is winner" << endl;
                }
                else if (coinResult == 1)
                {
                    window.draw(white);
                    cout << "white is winner" << endl;
                }
                window.display();
                sf::sleep(sf::seconds(2));
                gameState = Playing;
                // Move to the Playing state after the toss

            }
            break;

        case RULE:
            window.draw(backSprite);
            window.draw(ruleSprite);
            window.draw(pause1);
            break;

        case Playing:
            window.draw(pause);
            window.draw(boardSprite);
            for (int i = 0; i < numRows; ++i)
            {
                for (int j = 0; j < numCols; ++j)
                {
                    if (board[i][j] == 1)
                    {
                        bead.setTexture(&redBeadTexture);
                        bead.setPosition(j * spacing + 5.f, i * spacing + 5.f);
                        bead.setFillColor(sf::Color::Cyan);
                        window.draw(bead);
                    }
                    else if (board[i][j] == 2)
                    {
                        bead.setTexture(&blueBeadTexture);
                        bead.setPosition(j * spacing + 5.f, i * spacing + 5.f);
                        bead.setFillColor(sf::Color::Magenta);
                        window.draw(bead);
                    }
                    else if (board[i][j] == 0)
                    {
                        bead.setFillColor(sf::Color::Transparent);
                        window.draw(bead);
                    }
                }
            }
            if (isGameOver())
            {
                window.draw(winnerSprite);

            }

            //outlining the beads 
            if (selectedBeadX != -1 && selectedBeadY != -1)
            {
                CircleShape outline(30.f);
                outline.setPosition(selectedBeadX * spacing + 12.f, selectedBeadY * spacing + 8.f);
                outline.setFillColor(Color::Transparent);
                outline.setOutlineColor(Color::Red);
                outline.setOutlineThickness(8.f);
                window.draw(outline);
            }

            break;
        }

        window.display();
    }

    return 0;
    
}