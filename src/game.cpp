/*
Author: Baruch Chau
Description: Modified from the sample code given by Udacity.
Game constructor is modified to accommodate second snake and different 
initial positions that are not too close to each other.

Run function is to run a Check_Collision function in a separate thread to constantly
monitor the snakes for game ending collisions. Food and snakes are rendered separately.
Clear screen only happens once per render to reduce flickering. Game now has a end goal
value to achieve.

Place_Food function now also checks for second snake position.

Update function now includes updates for second snake.

Check_Collision function derived and split from Snake class to become it's own check.
At every the frequency of every frame checks if the snake collides with itself or the 
other snake, which game ending consequences for each scenario. 
*/

#include "game.h"
#include <iostream>
#include "SDL.h"

Game::Game(std::size_t grid_width, std::size_t grid_height, int goal)
    : snake1(grid_width, grid_height, 1), snake2(grid_width, grid_height, 2), 
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)) {
  Game::Set_Goal(goal);
  PlaceFood();
  // Snake1 starts at the top right
  snake1.head_x = 60;
  snake1.head_y = 100;
  snake1.direction = Snake::Direction::kUp;
  // Snake2 starts at the bottom left
  snake2.head_x = 100;
  snake2.head_y = 60;
  snake2.direction = Snake::Direction::kDown;
}

void Game::Run(Controller const &controller,
               Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;

  // Run thread to always check for collision of 2 snakes in the background
  std::thread collision_thread(&Game::Check_Collision, this);
  
  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake1, snake2);
    Update();
    renderer.RenderFood(food);
    //std::thread snake1_thread (&Renderer::RenderSnake, &renderer, snake1);
    renderer.RenderSnake(snake1);
    renderer.RenderSnake(snake2);
    renderer.ClearScreen();

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(&score1, &score2, &frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
  // End collision check
  collision_thread.join();
}

void Game::PlaceFood() {
  int x, y;
  if ((score1 < _goal) || (score2 < _goal)) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if ((!snake1.SnakeCell(&x, &y)) && (!snake2.SnakeCell(&x, &y))) {
      food.x = x;
      food.y = y;
      return;
    }
  } 
}

void Game::Update() {
  if (!snake1.alive || !snake2.alive) return;

  snake1.Update();
  snake2.Update();

  int new_x1 = static_cast<int>(snake1.head_x);
  int new_y1 = static_cast<int>(snake1.head_y);
  int new_x2 = static_cast<int>(snake2.head_x);
  int new_y2 = static_cast<int>(snake2.head_y);

  // Check if there's food over here
  // Increase other snake's speed too to keep up?
  // Snake 1 gets the food
  if (food.x == new_x1 && food.y == new_y1) {
    score1++;
    PlaceFood();
    // Grow snake and increase speed.
    snake1.GrowBody();
    snake1.speed += 0.02;
  }
  // Snake 2 gets the food
  if (food.x == new_x2 && food.y == new_y2) {
    score2++;
    PlaceFood();
    // Grow snake and increase speed.
    snake2.GrowBody();
    snake2.speed += 0.02;
  }
  // Check if game is over
  if ((score1 == _goal) || (score2 == _goal)) running = false;
}

void Game::Check_Collision(){

  while (running){
    //std::cout << "Check collision running" << "\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(17)); // Check same speed as frame rate (60)

    // Get snake1 head position
    SDL_Point current_head_cell1{
        static_cast<int>(snake1.head_x),
        static_cast<int>(snake1.head_y)};

    // Get snake2 head positon
    SDL_Point current_head_cell2{
        static_cast<int>(snake2.head_x),
        static_cast<int>(snake2.head_y)};
    
    // Check if snake1 has eaten itself
    for (auto const &item : snake1.body) {
      if (current_head_cell1.x == item.x && current_head_cell1.y == item.y) {
        snake1.alive = false;
        running = false;
      }
    }
    // Check if snake2 has eaten itself
    for (auto const &item : snake2.body) {
      if (current_head_cell2.x == item.x && current_head_cell2.y == item.y) {
        snake2.alive = false;
        running = false;
      }
    }
    // Check if the snake heads hit each other
    if (current_head_cell1.x == current_head_cell2.x && current_head_cell1.y == current_head_cell2.y) {
        snake1.alive = false;
        snake2.alive = false;
    }
    // Check if snake1 has eaten snake2
    for (auto const &item : snake2.body) {
      if (current_head_cell1.x == item.x && current_head_cell1.y == item.y) {
        snake2.alive = false;
      }
    }
    // Check if snake2 has eaten snake1
    for (auto const &item : snake1.body) {
      if (current_head_cell2.x == item.x && current_head_cell2.y == item.y) {
        snake1.alive = false;
      }
    }
  }
}