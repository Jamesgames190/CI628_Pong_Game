#include "MyGame.h"


// Text
TTF_Font* font = nullptr;
SDL_Color textColour = { 200, 200, 200 };
SDL_Surface* textSurface = nullptr;
SDL_Texture* textTexture = nullptr;



//Load textures
SDL_Texture* loadTexture(const char* filePath, SDL_Renderer* renderer)
{
    SDL_Surface* tempSurface = SDL_LoadBMP(filePath);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    SDL_FreeSurface(tempSurface);
    return texture;
}



void MyGame::startSDL(const char* title)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0)
    {
        //text
        TTF_Init();
        font = TTF_OpenFont("../src/assets/fonts/arial.ttf", 36);

        //audio
        Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
        Mix_Volume(-1, 5);

        mercy = SDL_GetTicks() / 1000;
    }
}//---


void MyGame::on_receive(std::string cmd, std::vector<std::string>& args) {
    if (cmd == "GAME_DATA") {
        // we should have exactly 4 arguments
        if (args.size() == 4) {
            game_data.player1Y = stoi(args.at(0));
            game_data.player2Y = stoi(args.at(1));
            game_data.ballX = stoi(args.at(2));
            game_data.ballY = stoi(args.at(3));
        }
    } 
    else {
        std::cout << "Received: " << cmd << std::endl;
    }
}

void MyGame::send(std::string message) {
    messages.push_back(message);
}

void MyGame::input(SDL_Event& event) {
    switch (event.key.keysym.sym) {
        case SDLK_w:
            send(event.type == SDL_KEYDOWN ? "W_DOWN" : "W_UP");
            break;
        case SDLK_s:
            send(event.type == SDL_KEYDOWN ? "S_DOWN" : "S_UP");
            break;
        case SDLK_i:
            send(event.type == SDL_KEYDOWN ? "I_DOWN" : "I_UP");
            break;
        case SDLK_k:
            send(event.type == SDL_KEYDOWN ? "K_DOWN" : "K_UP");
            break;
        case SDLK_RETURN:
            if (gameOver == true)
            {
                restart = true;
            }

    }
}


void MyGame::createGameObjects(SDL_Renderer* renderer)
{
    //sounds
    bounceSound = Mix_LoadWAV("../src/assets/audio/boing.wav");
    scoreSound = Mix_LoadWAV("../src/assets/audio/game-start.wav");
    winSound = Mix_LoadWAV("../src/assets/audio/win.wav");

    //textures
    scores = {
        loadTexture("../src/assets/images/0.bmp", renderer),
        loadTexture("../src/assets/images/1.bmp", renderer),
        loadTexture("../src/assets/images/2.bmp", renderer),
        loadTexture("../src/assets/images/3.bmp", renderer),
        loadTexture("../src/assets/images/4.bmp", renderer),
        loadTexture("../src/assets/images/5.bmp", renderer),
        loadTexture("../src/assets/images/6.bmp", renderer),
        loadTexture("../src/assets/images/7.bmp", renderer),
    };


    paddles = {
        loadTexture("../src/assets/images/basePaddle.bmp", renderer),
        loadTexture("../src/assets/images/bronzePaddle.bmp", renderer),
        loadTexture("../src/assets/images/silverPaddle.bmp", renderer),
        loadTexture("../src/assets/images/goldPaddle.bmp", renderer),
        loadTexture("../src/assets/images/diamondPaddle.bmp", renderer),
    };

    background = loadTexture("../src/assets/images/background.bmp", renderer);
    ballSprite = loadTexture("../src/assets/images/ball.bmp", renderer);

}

void MyGame::update() {
    if (!gameOver)
    {
        movement();
        checkBounce();
        checkGameOver();
    }
    else
    {
        if (restart == true)
        {
            restartGame();
        }
    }
}

void MyGame::render(SDL_Renderer* renderer) {
    /*SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &player1);
    SDL_RenderDrawRect(renderer, &player2);
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &ball);*/
    SDL_RenderCopy(renderer, background, NULL, &backgroundRect);
    SDL_RenderCopy(renderer, paddles[p1Winstreak], NULL, &player1Rect);
    SDL_RenderCopy(renderer, paddles[p2Winstreak], NULL, &player2Rect);
    SDL_RenderCopy(renderer, scores[p1Score], NULL, &score1Rect);
    SDL_RenderCopy(renderer, scores[p2Score], NULL, &score2Rect);
    SDL_RenderCopy(renderer, ballSprite, NULL, &ballRect);
    updateGUI(renderer);
}

void MyGame::updateGUI(SDL_Renderer* renderer)
{
    
    SDL_Rect textRect = { 300, 60, 0, 0};
    std::string screenText;
    /*
    //text to be seen
    screenText = std::to_string(int(p1Score));
    
    //render 
    textSurface = TTF_RenderText_Blended(font, screenText.c_str(), textColour);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    //clear memory
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);



    textRect = { 500, 60, 0, 0 };

    //text to be seen
    screenText = std::to_string(int(p2Score));

    //render 
    textSurface = TTF_RenderText_Blended(font, screenText.c_str(), textColour);
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    //clear memory
    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    */

    //declare winner


    if (p1Wins == true)
    {
        textRect = { 250, 300, 0, 0 };

        screenText = "Player 1 wins";

        //render 
        textSurface = TTF_RenderText_Blended(font, screenText.c_str(), textColour);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        //clear memory
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        textRect = { 250, 400, 0, 0 };

        screenText = "Press enter to restart";

        //render 
        textSurface = TTF_RenderText_Blended(font, screenText.c_str(), textColour);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        //clear memory
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }
    else if (p2Wins == true)
    {

        textRect = { 250, 300, 0, 0 };

        screenText = "Player 2 wins";

        //render 
        textSurface = TTF_RenderText_Blended(font, screenText.c_str(), textColour);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        //clear memory
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);

        textRect = { 250, 400, 0, 0 };

        screenText = "Press enter to restart";

        //render 
        textSurface = TTF_RenderText_Blended(font, screenText.c_str(), textColour);
        textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
        SDL_QueryTexture(textTexture, NULL, NULL, &textRect.w, &textRect.h);
        SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

        //clear memory
        SDL_FreeSurface(textSurface);
        SDL_DestroyTexture(textTexture);
    }

   

}

void MyGame::checkBounce()
{
    //top walls

    if ((ball.y <= 25) && (recentlyBounced == false))
    {
        Mix_PlayChannel(-1, bounceSound, 0);
        recentlyBounced = true;
    }
    else if ((ball.y >= 575) && (recentlyBounced == false))
    {
        Mix_PlayChannel(-1, bounceSound, 0);
        recentlyBounced = true;
    }

    if ((ball.y > 25) && (ball.y < 575))
    {
        recentlyBounced = false;
    }


    //score
    if ((SDL_GetTicks() / 1000) > mercy)
    {
        if (ball.x >= 775 && recentlyScored == false) // recently scored is to prevent the ball counting twice sometimes
        {
            p1Score++;
            Mix_PlayChannel(-1, scoreSound, 0);
            recentlyScored = true;
            cooldown = SDL_GetTicks() / 500;
            
        }
        else if (ball.x <= 25 && recentlyScored == false)
        {
            p2Score++;
            Mix_PlayChannel(-1, scoreSound, 0);
            cooldown = SDL_GetTicks() / 500;
            recentlyScored = true;
        }

        if ((SDL_GetTicks() / 500) > cooldown)
        {
            recentlyScored = false;
        }
    }
}

void MyGame::checkGameOver()
{
    if (p1Score >= 7) //>= instead of = incase of any scoring errors
    {
        Mix_PlayChannel(-1, winSound, 0);
        p1Wins = true;
        gameOver = true;
    }
    else if (p2Score >= 7)
    {
        Mix_PlayChannel(-1, winSound, 0);
        p2Wins = true;
        gameOver = true;
    }
}

void MyGame::restartGame()
{
    restart = false;
    gameOver = false;
    p1Score = 0;
    p2Score = 0;
    mercy = SDL_GetTicks() / 1000;

    //when sprites are added, reload them and give the winner a special one

    if (p1Wins)
    {
        p1Winstreak++;
        p2Winstreak = 0;
    }
    else
    {
        p2Winstreak++;
        p1Winstreak = 0;
    }

    if (p1Winstreak > 4)
    {
        p1Winstreak = 4;
    }
    if (p2Winstreak > 4)
    {
        p2Winstreak = 4;
    }

    p1Wins = false;
    p2Wins = false;
}


void MyGame::movement()
{
    player1Rect.y = game_data.player1Y;
    player2Rect.y = game_data.player2Y;
    ball.x = game_data.ballX;
    ball.y = game_data.ballY;
    ballRect.x = game_data.ballX;
    ballRect.y = game_data.ballY;

    //incase of network issues, the ball won't go off screen

    if (ball.y > 575)
    { 
        ball.y = 575;
        ballRect.y = 575;
    }
    else if (ball.y < 0)
    {
        ball.y = 0;
        ballRect.y = 0;
    }
    if (ball.x > 775)
    {
        ball.x = 775;
    }
    else if (ball.x < 0)
    {
        ball.x = 0;
    }
}
