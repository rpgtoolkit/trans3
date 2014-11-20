#include "test_include_inheritance_base.prg"

class B : A
{
public:
	method B()
	{
		mVal = "B::mVal";
	}
};

sample = B();
debugger("Line");
c = sample->getVal();
mwin("Value of object: " + c);
wait();
