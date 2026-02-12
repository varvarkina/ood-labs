#ifndef DISABLEDANCEBEHAVIOR_H
#define DISABLEDANCEBEHAVIOR_H

#include "IDanceBehavior.h"

class DisableDanceBehavior : public IDanceBehavior
{
public:
	void Dance() const override {}
};

#endif
