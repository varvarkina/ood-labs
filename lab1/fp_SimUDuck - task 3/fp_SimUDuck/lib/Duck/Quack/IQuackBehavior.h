#ifndef IQUAKBEHAVIOR_H
#define IQUAKBEHAVIOR_H

struct QuackBehaviorStrategy
{
    std::function<void()> Quack;
};

#endif
