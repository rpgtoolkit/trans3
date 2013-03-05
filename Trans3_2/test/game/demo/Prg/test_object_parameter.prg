class oneClass
{
public:
	function oneClass(val)
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
	function otherClass(obj)
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
a = oneClass(d);
debugger("LINE 2");
b = otherClass(a);
c = b->value();
mwin("b value is <c>");
wait();

