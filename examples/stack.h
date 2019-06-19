#ifndef STACK_H
#define STACK_H

#define SIZE 10

template <typename T> class stack {
  T *arr;
  int top;
  int capacity;
  bool outPutMessages = false;

public:
  void push(T);
  T pop();
  T peek();
  int size();
  bool isEmpty();
  bool isFull();
  stack(int size = SIZE); // constructor
  ~stack();
  void toggleOutput(bool newValue);
};

#endif // STACK_H
