#ifndef SQUEAKBEHAVIOR_H
#define SQUEAKBEHAVIOR_H

#include "IQuackBehavior.h"
#include <iostream>

QuackBehaviorStrategy MakeSqueakBehavior()
{
    return { []() {
        std::cout << "Squeek!!!" << std::endl;
    } };
};

#endif
