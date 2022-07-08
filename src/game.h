#ifndef GAME_H
#define GAME_H

#include <random>
//#include <mutex>
//#include <condition_variable>
#include <future>
#include <thread>
#include "SDL.h"
#include "SDL_thread.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height, int goal);
  void Run(Controller const &controller, 
           Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore1() const;
  int GetScore2() const;
  int GetSize1() const;
  int GetSize2() const;

  // Listed as public so other classes can monitor if the game is running
  bool running = true;

 private:
  Snake snake1, snake2;
  SDL_Point food;

  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;

  int score1{0};
  int score2{0};

  void Set_Goal(int goal);
  void PlaceFood();
  void Update();

  int _goal;
  //std::mutex _mutex;
  //std::condition_variable _conditon;
};

#endif