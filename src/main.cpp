#include <iostream>
#include "controller.h"
#include "game.h"
#include "renderer.h"

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
  Game game(kGridWidth, kGridHeight, goal);
  game.Run(controller, renderer, kMsPerFrame);
  std::cout << "Game has terminated successfully!\n";
  std::cout << "Snake 1 Score: " << game.GetScore1() << "\n";
  std::cout << "Snake 1 Size: " << game.GetSize1() << "\n";
  std::cout << "Snake 2 Score: " << game.GetScore2() << "\n";
  std::cout << "Snake 2 Size: " << game.GetSize2() << "\n";
  return 0;
}