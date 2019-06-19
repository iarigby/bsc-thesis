#ifndef FUNCTIONPOINTERMAP_H
#define FUNCTIONPOINTERMAP_H

#include <cassert>
#include <iostream>
#include <map>
#include <string>
#include <typeindex>
#include <typeinfo>
#include <vector>

template <typename A> using voidFunctionType = void (A::*)(void);
template <typename A> using voidFunction = void (*)(A &);

/**
 * \param A typename that the members will be stored for
 */
template <typename A> class FunctionPointerMap {
public:
  /**
   * insert new function to the map
   * casts the function to void *(void) and stores the typeid to use for
   * assertion later
   * \param functionName key used for looking up the function pointer in the map
   * \param functionPointer pointer to the member function
   */
  template <typename T>
  void insert(std::string functionName, T functionPointer);
  void insertNonVoid(std::string functionName, voidFunction<A> functionPointer);
  /**
   * This function is capable of passing the arguments to the member function
   * and returning the result of the type T specified in the parameter.
   * Originally, type_index is used to assert that T and Args conform to the
   * function signature.
   *  Currently this feature is turned off because of reasons
   * specified in description of CombinationTester class
   * \param a reference to the instance which the function will be called on
   * \param functionName key used for looking up the function pointer in the map
   * \param T return type
   * \param args arguments for function
   */
  template <typename T, typename... Args>
  T searchAndCall(A &instance, std::string functionName, Args &&... args);
  std::vector<std::string> getFunctions();

private:
  /**
   * stores functions cast to void along with the typeid infered during insert()
   * call.
   */
  std::map<std::string, std::pair<voidFunctionType<A>, std::type_index>> m1;
  /**
   * stores pointers to user defined functions which will in turn call member
   * functions with correct arguments
   */
  std::map<std::string, voidFunction<A>> map;
  // revisit this
  bool outputMessages = false;
  std::vector<std::string> functions;
};

template <typename A>
template <typename T>
void FunctionPointerMap<A>::insert(std::string functionName,
                                   T functionPointer) {
  auto typeIndex = std::type_index(typeid(functionPointer));
  m1.insert(std::make_pair(
      functionName,
      std::make_pair((voidFunctionType<A>)functionPointer, typeIndex)));
  functions.push_back(functionName);
}
template <typename A>
void FunctionPointerMap<A>::insertNonVoid(std::string functionName,
                                          voidFunction<A> f2) {
  map.insert(std::make_pair(functionName, f2));
  functions.push_back(functionName);
}

template <typename A>
template <typename T, typename... Args>
T FunctionPointerMap<A>::searchAndCall(A &instance, std::string functionName,
                                       Args &&... args) {
  if (this->outputMessages) {
    std::cout << "calling " << functionName << "\n";
  }
  auto mapIter = m1.find(functionName);
  if (mapIter != m1.end()) {
    auto mapVal = mapIter->second;
    auto typeCastedFun = (T(A::*)(Args...))(mapVal.first);
    // currently the type is not checked, since right now all return values
    // are discarded by casting to void, in order to allow calling all functions
    // in a loop
    // assert(mapVal.second == std::type_index(typeid(typeCastedFun)));
    return (instance.*typeCastedFun)(std::forward<Args>(args)...);
  } else {
    return (map.find(functionName)->second)(instance);
  }
}

template <typename A>
std::vector<std::string> FunctionPointerMap<A>::getFunctions() {
  return functions;
}

#endif // FUNCTIONPOINTERMAP_H
