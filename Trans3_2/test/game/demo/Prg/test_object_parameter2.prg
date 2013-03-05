class thirdClass
{
public:
	function thirdClass()
	{
		mVal = 2;
	}

	function thirdClass(thirdClass obj)
	{
		mVal = obj->mVal + 2;
	}

	function value()
	{
		return mVal;
	}

	function takeValue(thirdClass obj)
	{
		mVal = obj->value();
	}
private:
	var mVal;
};

c = thirdclass();
d = thirdclass(c);
a = c->value();
d->takevalue(c);
b = d->value();
mwin("c=<a>, d=<b>");
wait();
