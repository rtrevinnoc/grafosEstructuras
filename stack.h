#ifndef STACK_H_ /* Include guard */
#define STACK_H_

// A structure to represent a queue
struct Stack {
  int top;
  unsigned capacity;
  int *array;
};

struct Stack *createStack(unsigned capacity);

// int isFull(struct Stack* stack);

// int isEmpty(struct Stack* stack);

void push(struct Stack *stack, int item);

int pop(struct Stack *stack);

int top(struct Stack *stack);

#endif
