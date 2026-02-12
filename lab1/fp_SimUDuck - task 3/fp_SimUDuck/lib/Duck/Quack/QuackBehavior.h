#ifndef QUACKBEHAVIOR_H
#define QUACKBEHAVIOR_H

#include "IQuackBehavior.h"
#include <iostream>

QuackBehaviorStrategy MakeQuackBehavior()
{
    return { []() {
        std::cout << "Quack Quack!!!" << std::endl;
    } };
};

#endif
