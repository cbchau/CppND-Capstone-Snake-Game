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
}

void Game::Set_Goal (int goal){
  _goal = goal;
}

void Game::Run(Controller const &controller,
               Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;

  // Different starting directions for snakes
  snake1.direction = Snake::Direction::kUp;
  snake2.direction = Snake::Direction::kDown;

  //SDL_Thread *food_thread = SDL_CreateThread(renderer.RenderFood, "Food thread", (SDL_Point const&)food);
  
  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake1, snake2);
    Update();
    //renderer.RenderFood(food);
    std::thread food_thread (renderer.RenderFood, food);
    renderer.RenderSnake(snake1);
    //std::thread snake1_thread = std::thread(renderer.RenderSnake, snake1);
    renderer.RenderSnake(snake2);
    //std::thread snake2_thread = std::thread(renderer.RenderSnake, snake2);
    renderer.ClearScreen();

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score1, score2, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    // Removed to unlock unlimited frame rate
    if (frame_duration < target_frame_duration) {
      //SDL_Delay(target_frame_duration - (target_frame_duration - frame_duration));
      SDL_Delay(target_frame_duration - frame_duration);
    }

    // Check if game is over
    if ((score1 == _goal) || (score2 == _goal)) running = false;
  }
  // Terminal rendering threads
  //SDL_WaitThread (food_thread, NULL);
}

void Game::PlaceFood() {
  int x, y;
  if ((score1 < _goal) || (score2 < _goal)) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if ((!snake1.SnakeCell(x, y)) && (!snake2.SnakeCell(x, y))) {
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
  if (food.x == new_x1 && food.y == new_y1) {
    score1++;
    PlaceFood();
    // Grow snake and increase speed.
    snake1.GrowBody();
    snake1.speed += 0.02;
  }
  if (food.x == new_x2 && food.y == new_y2) {
    score2++;
    PlaceFood();
    // Grow snake and increase speed.
    snake2.GrowBody();
    snake2.speed += 0.02;
  }
}
/*
void Game::Check_Collision() {
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  std::unique_lock<std::mutex> lock(_mutex);
  
}*/

int Game::GetScore1() const { return score1; }
int Game::GetScore2() const { return score2; }
int Game::GetSize1() const { return snake1.size; }
int Game::GetSize2() const { return snake2.size; }
