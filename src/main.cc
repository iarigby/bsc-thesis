/*******************************************************************************
 *                                main.cc                                      *
 *******************************************************************************
 * This file should be implemented by the user
 * It's divided into sections along with lines of code
 * Example code tests the stack class of type int
 */

/* HEADERS
 * libraries needed for running
 */

// replace this line with the header of your own class
#include "stack.h"
// program
#include "combinationTester.hpp"
#include "functionPointerMap.hpp"
#include <iostream>

/* GLOBALS
 * used by sanitizer coverage to record covered pc blocks
 */
CoverageReporter coverageReporter;

// verbose, report each block that is found, every time
bool outputMessages = true;

int main(int argc, char **argv) {
  // Function that does all the initialization
  InstanceFunctionPointer<stack<int>> getStackInstance =
      [](int sequenceLength) {
        stack<int> s(3);
        return s;
      };
  // Function pointers
  FunctionPointerMap<stack<int>> memberFunctions;
  memberFunctions.insert("pop", &stack<int>::pop);
  memberFunctions.insert("peek", &stack<int>::peek);
  memberFunctions.insert("size", &stack<int>::size);
  memberFunctions.insert("isEmpty", &stack<int>::isEmpty);
  memberFunctions.insert("isFull", &stack<int>::isFull);
  memberFunctions.insertNonVoid("push1",
				[](stack<int> &a) { a.push(1); });
  CombinationTester<stack<int>> combinationTester(
      4, memberFunctions, getStackInstance, &coverageReporter);
  combinationTester.run();
  coverageReporter.printResults();
  coverageReporter.printResultsToFile();
  std::cout << "done\n";
  return 0;
}
