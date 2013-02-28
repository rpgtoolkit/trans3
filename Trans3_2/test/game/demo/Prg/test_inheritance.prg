class A
{
public:
	method messageA()
	{
		Mwin("Message from [A]");
		wait();
		mwincls();
	}
};

class B : A
{
public:
	method messageB()
	{
		Mwin("Message from [B]");
		wait();
		mwincls();
	}
};

local(x) = B();
x->messageA();
x->messageB();
