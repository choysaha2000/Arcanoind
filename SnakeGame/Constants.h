#pragma once
#include <string>

const std::string RESOURCES_PATH = "Resources/";
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 1000;

struct Vector2D { float x = 0; float y = 0; };
typedef Vector2D Position2D;

const float PAUSE_LENGTH = 3.f;


// LEADERBOARD PARAMETERS
const int dotsNeed = 20;
const int ScorePlayers = 10;
const float bottomPadding = 50.f;
const float spacing = 20.f;


// PADDLE OPTIONS
const float PADDLE_WIDTH = 120.f;
const float PADDLE_HEIGHT = 20.f;
const float PADDLE_SPEED = 800.f;
const float PADDLE_Y_OFFSET = 50.f; 

// BALL OPTIONS
const float BALL_RADIUS = 10.f;
const float BALL_SPEED = 350.f;

// BLOCK OPTIONS

const float BLOCK_WIDTH = 60.f;
const float BLOCK_HEIGHT = 75.f;

const int rows = 5;
const int cols = 10;
const float startX = 100.f; 
const float startY = 100.f;
const float gapX = 70.f; 
const float gapY = 30.f;