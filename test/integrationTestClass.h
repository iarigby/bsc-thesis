#ifndef INTEGRATION_TEST_CLASS_H
#define INTEGRATION_TEST_CLASS_H

class IntegrationTestClass {
public:
  void increaseCounter();
  void setToggle(bool);
  int f2();

private:
  bool toggle = false;
  int counter = 1;
  bool counterIs2();
  int f4();
};

#endif // INTEGRATION_TEST_CLASS_H
