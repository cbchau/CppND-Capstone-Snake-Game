/*
Author: Baruch Chau
Description: Modified from the sample code given by Udacity.
Header code for the Snake class. 
Included player enumeration to indentify the snakes.
*/
#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include <iostream>
#include "SDL.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height, int player)
      : grid_width(grid_width),
        grid_height(grid_height),
        head_x(grid_width / 2),
        head_y(grid_height / 2) {
    Set_Player(player);
  }

  void Set_Player(int player_val) {
    player = player_val;
  }

  void Update();
  void GrowBody();
  bool SnakeCell(int *x, int *y);

  //Direction direction = Direction::kUp;
  Direction direction;

  float speed{0.1f};
  int size{1};
  bool alive{true};
  float head_x;
  float head_y;
  std::vector<SDL_Point> body;
  int player; //Player 1 = blue, Player 2 = green

 private:
  void UpdateHead();
  void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

  bool growing{false};
  int grid_width;
  int grid_height;
};

#endif