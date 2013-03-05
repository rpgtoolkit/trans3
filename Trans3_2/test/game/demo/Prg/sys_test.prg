
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
}

class otherClass
{
public:
	function otherClass(oneClass obj)
	{
		mVal = obj->value;
	}

	function value()
	{
		return mVal;
	}

private:
	var mVal;
}

a = oneClass(2);
b = otherClass(a);
