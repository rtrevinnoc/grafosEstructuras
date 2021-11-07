#ifndef STACK_H_ /* Include guard */
#define STACK_H_

// Struct que representa una pila
struct Stack {
  int top; // Cima de la pila
  unsigned capacity; // Capacidad de la pila
  int *array; // Puntero a lista que simula una pila
};

struct Stack *createStack(unsigned capacity);

void push(struct Stack *stack, int item);

int pop(struct Stack *stack);

int top(struct Stack *stack);

#endif
