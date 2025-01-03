#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"
#include<SDL_Image.h>
#include<SDL_ttf.h>
#include<SDL_mixer.h>
#include<SDL_timer.h>

#define SPRITE_SIZE = 32;

static struct GameData {
    int player1Y = 0;
    int player2Y = 0;
    int ballX = 0;
    int ballY = 0;
} game_data;

class MyGame {

    private:
        SDL_Rect player1 = { 0, 50, 20, 60 };
        SDL_Rect player2 = { 780, 50, 20, 60 };
        SDL_Rect ball = { 390, 50, 20, 60 };
        std::vector<SDL_Texture*> scores;
        std::vector<SDL_Texture*> paddles;
        SDL_Texture* background;
        SDL_Texture* ballSprite;

        int p1Score = 0;
        int p2Score = 0;

        bool recentlyScored = false;
        bool recentlyBounced = false;
        int cooldown = 0;
        
        bool gameOver = false;
        bool p1Wins = false;
        bool p2Wins = false;
        bool restart = false;
        int p1Winstreak = 0;
        int p2Winstreak = 0;

        SDL_Rect player1Rect = { 200, 0, 20, 60};
        SDL_Rect player2Rect = { 600, 0, 20, 60 };
        SDL_Rect score1Rect = { 250, 60, 128, 128 };
        SDL_Rect score2Rect = { 500, 60, 128, 128 };
        SDL_Rect ballRect = { 0, 0, 32, 32 };
        SDL_Rect backgroundRect = { 0,0,800,600 };

        Mix_Chunk* bounceSound = NULL;
        Mix_Chunk* scoreSound = NULL;
        Mix_Chunk* winSound = NULL;


        int mercy = 0;

    public:
        std::vector<std::string> messages;


        void startSDL(const char* title);
        void on_receive(std::string message, std::vector<std::string>& args);
        void send(std::string message);
        void input(SDL_Event& event);
        void update();
        void render(SDL_Renderer* renderer);
        void checkGameOver();
        void checkBounce();
        void updateGUI(SDL_Renderer* renderer);
        void restartGame();
        void movement();
        void createGameObjects(SDL_Renderer* renderer);

};


#endif