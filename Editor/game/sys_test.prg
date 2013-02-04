//A program that test's the editor.

//Declare the Vehicle class.
class Vehicle
{
public:
    var passengers;
    var fuelcap;
    var mpg;
    
    method Vehicle(p, f, m);
    method range();
}

method Vehicle::Vehicle(p, f, m)
{
    passengers = p;
    fuelcap = f;
    mpg = m;
}

method Vehicle::range()
{
    return mpg * fuelcap;
}

//Pass values to constructor
global(minivan) = Vehicle(7, 16, 21);
global(sportscar) = Vehicle(2, 14, 12);

local(range1);
local(range2);

//Compute the range assuming a full tank of fuel.
range1 = minivan->range();
range2 = sportscar->range();

mwin("Minivan can carry " + minivan->passengers + " with a range of " + range1 + ".");
mwin("Sportscar can carry " + sportscar->passengers + " with a range of " + range2 + ".");
wait();
mwinCls();