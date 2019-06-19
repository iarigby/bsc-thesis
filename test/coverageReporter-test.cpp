#include "catch.hpp"
#include "coverageReporter.h"

struct TestData {
  std::vector<std::string> shortSequence{"f1"};
  std::vector<std::string> sequence1{"f1", "f2"};
  std::vector<std::string> sequence2{"f2", "f3"};
  std::vector<std::string> longSequence{"f1", "f2", "f3"};
};

using pc_set = std::set<std::string>;
SCENARIO("CoverageReporter has no coverage", "[coveragereporter]") {
  TestData testData;
  CoverageReporter cr;
  const pc_set smallCoverage = {"pc1"};
  const pc_set largeCoverage = {"pc1", "pc2"};
  GIVEN("new coverage blocks") {
    cr.startCoverage(testData.sequence1);
    WHEN("current sequence is flushed") {
      cr.addPCForSequence("pc1");
      REQUIRE(cr.currentPC == smallCoverage);
      cr.flush();
      THEN("currentPC should be inserted to coverage and cleared") {
        std::set<pc_set> expectedCoverage{{"pc1"}};
        REQUIRE(cr.coverage() == expectedCoverage);
        REQUIRE(cr.currentPC.empty());
      }
    }
  }
}

SCENARIO("CoverageReporter has existing coverage", "[coveragereporter]") {
  TestData testData;
  CoverageReporter cr;
  const pc_set smallCoverage = {"pc1"};
  const pc_set largeCoverage = {"pc1", "pc2"};
  GIVEN("new coverage") {
    cr.startCoverage(testData.sequence1);
    WHEN("exists coverage that is a subset of new reporting") {
      cr.addPCForSequence("pc1");
      cr.flush();
      cr.startCoverage(testData.shortSequence);
      cr.addPCForSequence("pc2");
      cr.flush();
      cr.startCoverage(testData.sequence2);
      cr.addPCForSequence("pc1");
      cr.addPCForSequence("pc2");
      REQUIRE(cr.currentPC == largeCoverage);
      cr.flush();
      THEN("all of them should be removed") {
        std::set<pc_set> expectedCoverage{largeCoverage};
        REQUIRE(cr.coverage() == expectedCoverage);
        REQUIRE(cr.coverageSequences.size() == 1);
      }
    }

    WHEN("new coverage is not contained in one of the sets") {
      cr.addPCForSequence("pc1");
      cr.addPCForSequence("pc2");
      cr.flush();
      cr.startCoverage(testData.sequence2);
      cr.addPCForSequence("pc3");
      cr.flush();
      THEN("new coverage should be inserted") {
        std::set<pc_set> expectedCoverage{{"pc3"}, largeCoverage};
        REQUIRE(cr.coverage() == expectedCoverage);
      }
    }
    WHEN("new coverage is already contained in one of the sets") {
      cr.addPCForSequence("pc1");
      cr.addPCForSequence("pc2");
      cr.flush();
      cr.startCoverage(testData.sequence2);
      cr.addPCForSequence("pc2");
      cr.flush();
      THEN("new coverage should not be inserted") {
        std::set<pc_set> expectedCoverage{largeCoverage};
        REQUIRE(cr.coverage() == expectedCoverage);
      }
    }
  }
}


SCENARIO("coverage for new sequence already exists", "[coveragereporter]") {
  TestData testData;
  CoverageReporter cr;
  cr.startCoverage(testData.sequence1);
  pc_set coverage = {"pc1", "pc2"};
  cr.addPCForSequence("pc1");
  cr.addPCForSequence("pc2");
  cr.flush();
  auto sequence = cr.coverageSequences.find(coverage)->second;
  REQUIRE(sequence == testData.sequence1);
  
  GIVEN("new coverage with longer sequence") {
    cr.startCoverage(testData.longSequence);
    cr.addPCForSequence("pc1");
    cr.addPCForSequence("pc2");
    cr.flush();
    THEN("existing sequence for coverage should not be replaced") {
      auto sequence = cr.coverageSequences.find(coverage)->second;
      REQUIRE(sequence == testData.sequence1);
    }
  }
  
  GIVEN("new coverage with sequence of same length") {
    cr.startCoverage(testData.sequence2);
    cr.addPCForSequence("pc1");
    cr.addPCForSequence("pc2");
    cr.flush();
    THEN("existing sequence for coverage should not be replaced") {
      auto sequence = cr.coverageSequences.find(coverage)->second;
      REQUIRE(sequence == testData.sequence1);
    }
  }

  GIVEN("new coverage with shorter sequence") {
    cr.startCoverage(testData.shortSequence);
    cr.addPCForSequence("pc1");
    cr.addPCForSequence("pc2");
    cr.flush();
    THEN("existing sequence for coverage should be updated") {
      auto sequence = cr.coverageSequences.find(coverage)->second;
      REQUIRE(sequence == testData.shortSequence);
    }
  }
}

SCENARIO("user forgot to set current sequence", "[coveragereporter]") {
  CoverageReporter cr;
  std::vector<std::string> sequence{"f1", "f2"};
  cr.startCoverage(sequence);
  cr.addPCForSequence("pc1");
  cr.flush();
  GIVEN("coverage reporter without current sequence") {
    cr.addPCForSequence("pc2");
    THEN("flushing would cause an exception") { REQUIRE_THROWS(cr.flush()); }
  }
}
