#ifndef PERMUTATIONGENERATOR_H
#define PERMUTATIONGENERATOR_H

#include <vector>

/**
 * responsible for generating all possible length sequence permutations.
 * Example:  for a set for {"a", "b"}, with maxLength 2 it will generate {"a"},
 * {"a", "a"} ..
 * {"b"}, {"b", "a"} .. {"b", "b"}, etc
 * Reasons for not using std::next_permutation:
 *   1. Permutation with repetition is needed. std::next_permutation will
 * permute the existing elements, therefore I would need to generate a separate
 * sequence for each repetition (the one where a occurs twice, the one where b
 * occurs twice, combination of them, etc...).
 *   2. we need to generate sequences of varying length, which would also
 * require additional workarounds, and running
 *
 *  It is easier to simply to treat the problem
 * space as a recursive B+ tree with children of each node being all the
 * elements of the initial set
 */
template <typename T> class PermutationGenerator {
public:
  /**
   * creates a new permutation generator.
   * \param initialSet will be used to select elements for sequence permutations
   * \param maxLenght is a limit for maximum sequence length
   *
   * The reason std::vector is used for the initialSet is because of
   * availability of operator[].
   * The permutations are done on integer indices and then used to retreive
   * elements from the initialSet. Explained in more detail in member
   * permutations.
   */
  PermutationGenerator(std::vector<T> initialSet, int maxLength);
  PermutationGenerator();
  /**
   * \return the next permutation
   * The permutation selection order follows inorder traversal of the tree. It
   * will start out with a first element of the set
   */
  std::vector<T> nextPermutation();
  /**
   * \return whether all possible permutations of all length have been returned
   * previosly
   */
  bool isDone();
  /**
   * will blacklist all sequences that start with the sequence last generated.
   * Ie stop exploring the path
   */
  void blacklistPermutation();

private:
  /**
   * elements of the sequences that will be permuted */
  std::vector<T> initialSet;
  /**
   * this is the vector that is actually permuted. Stored elements are indices
   * of the initialSet */
  std::vector<int> permutations;
  /**
   * maximum value that element in permutations can take */
  int setSize;
  int maxLength;
  int counter;
  /**
   * which element in permutations should be incremented next */
  int currentIndex = 0;
  /**
   * saves last index so it can blacklist the last generated path */
  int lastIndex = 0;
};

#endif // PERMUTATIONGENERATOR_H
