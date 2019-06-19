
#include "integrationTestClass.h"

void IntegrationTestClass::increaseCounter() {
  counter++;
}

void IntegrationTestClass::setToggle(bool value) {
  toggle = value;
}

int IntegrationTestClass::f2() {
  if (toggle) {
    if (counterIs2()) {
      return f4();
    } else {
      return 0;
    }
  } else {
    return 7;
  }
}

bool IntegrationTestClass::counterIs2() {
  return counter > 2;
}

int IntegrationTestClass::f4() {
  return 5;
}

