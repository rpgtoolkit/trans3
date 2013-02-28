// The following code will produce an expected error on "obj->bar()"

class A {
   public:
   function A() {}

   function foo() { debugger("foo") }
}

class B:A {
   public:
   function B() {}

   function bar() { debugger("bar") }
}

class C:B {
   public:
   function C() {}

   function foobar() { debugger("foobar") }
}

obj = C()
obj->foobar()
obj->foo()
obj->bar()

////////////////////////////////////////////////////////////////////////////////////

// The following code will produce an error on "obj->bar()"

class A {
   public:
   function A() {}

   function foo() { debugger("foo"); }
};

class B {
   public:
   function B() {}

   function bar() { debugger("bar"); }
};

class C:B {
   public:
   function C() {}

   function foobar() { debugger("foobar"); }
};

obj = C();
obj->foobar();
obj->bar();
