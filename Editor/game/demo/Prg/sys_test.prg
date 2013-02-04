test = A();
mwin(test->me);
wait();

struct A 
{
    var me = "Hello";
}

class C
{
public:
    
    method C()
    {
        
    }
    
    method display()
    {
        mwin("Hello World!"); 
        wait();  
    }
}

class D : C
{
public:
    
    method D()
    {
        
    }
}