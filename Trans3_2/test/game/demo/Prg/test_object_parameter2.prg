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
debugger("0");
c = thirdclass(); // c::mVal = 2
debugger("1");
d = thirdclass(c); // d::mVal = 4
a = c->value();
d->takevalue(c); // d::mVal = 2
b = d->value();
//e = 5;
debugger("line");
//e->takevalue(d);
mwin("c=<a>, d=<b>");
wait();

