#ifndef COVERAGEREPORTER_H
#define COVERAGEREPORTER_H

#include <algorithm> // for includes()
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

using pc_set = std::set<std::string>;

/**
 * Stores reported coverage
 */
class CoverageReporter {
public:
  /**
   * saves passed sequence as current one
   * \param functionSequence sequence of function names for which the coverage
   * should be recorded
   */
  void startCoverage(std::vector<std::string> functionSequence);
  /**
   * \param pc will be added to the current set of collected pcs
   */
  void addPCForSequence(const std::string &pc);
  /**
   * saves current sequence and associated coverage and resets data.
   * if exact same coverage has been found with same or shorter sequence,
   * the coverageSequences won't be updated, if longer one, the sequence for
   * coverage will be replaced.
   * otherwise, the function will check if new coverage contains any
   * of the existing ones as a subset, in which case the old coverage will
   * be removed and replaced with the larger set.
   */
  void flush();
  /**
   * get all sets coverered so far
   * \return keys of coverageSequences, set of sets
   */
  std::set<pc_set> coverage();
  /**
   *  set of all coverage points collected for current permutation
   */
  pc_set currentPC;
  /**
   * stores the shortest recorded function sequence for given coverage
   */
  std::map<pc_set, std::vector<std::string>> coverageSequences;
  /**
   * print results to std::cout
   */
  void printResults();
  /**
   * print results to "results.txt" of working directory
   */
  void printResultsToFile();
  /** 
   * Print results to fileName
   * \param fileName path of the file
   */
  void printResultsToFile(std::string fileName);
  /**
   * PC blocks that have been discovered across all sequences
   */
  pc_set coveredBlocks;
  /**
   * Flag for SanitizerCoverage callbacks
   */
  bool recordingCoverage;
private:
  /**
   * \return if first set is subset of the second
   */
  bool isSubsetOf(pc_set &s1, pc_set &s2);
  /**
   *  which sequence the current coverage will be mapped to
   */
  std::vector<std::string> currentSequence;
  /**
   * prints the coverageSequence member
   */
  void writeResults(std::ostream &outstream);
  /**
   * Check if new coverage was discovered during currentPC 
   */
  bool newPcWasFound();
};

#endif // COVERAGEREPORTER_H
