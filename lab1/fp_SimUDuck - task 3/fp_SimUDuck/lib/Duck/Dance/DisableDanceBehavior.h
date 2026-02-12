#ifndef DISABLEDANCEBEHAVIOR_H
#define DISABLEDANCEBEHAVIOR_H

#include "IDanceBehavior.h"

DanceBehavior MakeDisableDanceBehavior()
{
    return { []() {} };
};

#endif
