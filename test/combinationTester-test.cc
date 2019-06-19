#include "combinationTester.hpp"
#include "coverageReporter.h"
#include "functionPointerMap.hpp"
#include "integrationTestClass.h"
#include "catch.hpp"

CoverageReporter coverageReporter;
bool started;
bool outputMessages = true;

SCENARIO("integration test", "[integrationtest]") {
  GIVEN("All necessary classes and data for the combinationTester") {
    FunctionPointerMap<IntegrationTestClass> a1;
    a1.insert("increaseCounter", &IntegrationTestClass::increaseCounter);
    a1.insert("f2", &IntegrationTestClass::f2);
    a1.insertNonVoid("toggleTrue",
                     [](IntegrationTestClass &s) { s.setToggle(true); });
    a1.insertNonVoid("toggleFalse",
                     [](IntegrationTestClass &s) { s.setToggle(false); });

    InstanceFunctionPointer<IntegrationTestClass> getTestClassInstance =
        [](int size) {
          IntegrationTestClass s;
          // s.toggleOutput(true);
          return s;
        };
    CombinationTester<IntegrationTestClass> combinationTester(
        5, a1, getTestClassInstance, &coverageReporter);

    WHEN("The tests are run") {
      combinationTester.run();
      coverageReporter.printResults();
      coverageReporter.printResultsToFile("integration-test-results.txt");
      THEN("SanitizerCoverage should report covered pc blocks to coverageReporter") {
        int allCoveredBlocks = coverageReporter.coveredBlocks.size();
        REQUIRE(allCoveredBlocks >= 1);
      }
    }
  }
}
