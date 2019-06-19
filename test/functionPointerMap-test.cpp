#include "functionPointerMap.hpp"
#include "catch.hpp"

class MyClass {
public:
  bool b;
  bool f1();
  int f2(bool t, int &i);
};

TEST_CASE("init", "[functionpointermap]") {
  FunctionPointerMap<MyClass> m1;
  m1.insert("f1", &MyClass::f1);
  m1.insert("f2", &MyClass::f2);
  MyClass c;
  // not checking return values because I've altered fpmap
  // to call member functions and non-member functions
  // changed MyClass to take "return" values as references
  // REQUIRE(m1.searchAndCall<bool>(c, "f1") == true);
  // REQUIRE(m1.searchAndCall<int>(c, "f2", true) == 1);
  // REQUIRE(m1.searchAndCall<int>(c, "f2", false) == 0);
  
  SECTION("function calls work properly") {
    int i1 = 5;
    int i2 = 5;
    m1.searchAndCall<void>(c, "f2", true, i1);
    m1.searchAndCall<void>(c, "f2", false, i2);
    REQUIRE(i1 == 1);
    REQUIRE(i2 == 0);
  }
  
  SECTION("functions affecting members work properly") {
    m1.searchAndCall<void>(c, "f1");
    REQUIRE(c.b == true);
  }
  // TODO test what happens when it's not there
}

bool MyClass::f1() {
  this->b = true;
  return true;
}

int MyClass::f2(bool t, int &i) {
  if (t) {
    i = 1;
    return 1;
  } else {
    i = 0;
    return 0;
  }
}
