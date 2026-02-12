#ifndef IDANCEBEHAVIOR_H
#define IDANCEBEHAVIOR_H

struct IDanceBehavior
{
	virtual ~IDanceBehavior() {};
	virtual void Dance() const = 0;
};

#endif
