class oneClass
{
public:
	function oneClass(val, val2)
	{
		mVal = val;
	}

	function value()
	{
		return mVal;
	}

private:

	var mVal;
};

class otherClass
{
public:
	function otherClass(obj, val)
	{
		mVal = obj->value();
	}

	function value()
	{
		return mVal;
	}

private:
	var mVal;
};
d = "My goodness";
debugger("LINE 1");
a = oneClass(d, 99);
debugger("LINE 2");
b = otherClass(a, 108);
c = b->value();
mwin("b value is <c>");
wait();


