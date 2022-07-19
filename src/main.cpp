/*
Author: Baruch Chau
Description: Modified from the sample code provided by Udacity.
This class launches the entire game and initializes the game size parameters. 
Now prompts the user to end the goal for the game.
Exit text includes second player's scores.
Checks game ending conditions to see which player wins.
*/

#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"
#include "snake.h"

int main() {
  constexpr std::size_t kFramesPerSecond{60};
  constexpr std::size_t kMsPerFrame{1000 / kFramesPerSecond};
  constexpr std::size_t kScreenWidth{640};
  constexpr std::size_t kScreenHeight{640};
  constexpr std::size_t kGridWidth{32};
  constexpr std::size_t kGridHeight{32};

  // Prompt user to enter game length, check if value is valid
  int goal{20};
  std::cout << "Enter game length" << "\n";
  std::cin >> goal;
  while (std::cin.fail()){
    std::cout << "Enter valid integer for game length" << "\n";
    std::cin.clear();
    std::cin.ignore(256, '\n');
    std::cin >> goal;
  }

  Renderer renderer(kScreenWidth, kScreenHeight, kGridWidth, kGridHeight);
  Controller controller;
  Game game(kGridWidth, kGridHeight, goal); // Pass user entered goal to constructor 
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Snake 1 Score: " << game.GetScore(1) << "\n";
  std::cout << "Snake 1 Size: " << game.GetSize(1) << "\n";
  std::cout << "Snake 2 Score: " << game.GetScore(2) << "\n";
  std::cout << "Snake 2 Size: " << game.GetSize(2) << "\n";

  // Win condition if one snake eats another
  if (game.Get_Status(2) == false && game.Get_Status(1) == false) {
    std::cout << "Tie!\n";
    return 0;
  }
  else if (game.Get_Status(2) == false) {
    std::cout << "Snake1 wins!\n";
    return 0;
  }
  else if (game.Get_Status(1) == false) {
    std::cout << "Snake2 wins!\n";
    return 0;
  }
  
  // Win condition if one snake eats more than the other by the end of the game
  if (game.GetScore(1) > game.GetScore(2))
    std::cout << "Snake1 wins!\n";
  else if (game.GetScore(2) > game.GetScore(1))
    std::cout << "Snake2 wins!\n";

  return 0;
}