/*
Author: Baruch Chau
Description: Modified from the sample code given by Udacity. 
Header code for the Game class. 
Included getter functions for accessing the private Snake variables.
Included setter function to assign goal and to hide invariant.
Changed running to be a public function so other classes can know if the game is running.
Included Check_Collision function to monitor snake status and decide outcomes.
Included second snake (player).
*/
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
  
  // Included getter functions to access private Snake variable status
  bool Get_Status(int snake){ 
    if (snake == 1)
      return snake1.alive;
    else
      return snake2.alive;
  }
  int GetScore (int snake) {
    if (snake == 1)
      return score1;
    else
      return score2;
  }
  int GetSize (int snake) {
    if (snake == 1)
      return snake1.size;
    else
      return snake2.size;
  }
  // Included setter function to set private variable goal to hide invariant
  void Set_Goal (int goal){
    _goal = goal;
  }

  void Check_Collision();

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

  void PlaceFood();
  void Update();

  int _goal;
};

#endif