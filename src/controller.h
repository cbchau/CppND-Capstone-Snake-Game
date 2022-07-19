/*
Author: Baruch Chau
Description: Modified from the sample code given by Udacity.
Header for the Controller class.
*/

#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"

class Controller {
 public:
    void HandleInput(bool &running, Snake &snake1, Snake &snake2) const;

 private:
    void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

#endif