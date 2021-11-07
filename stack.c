#include "stack.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

struct Stack *createStack(unsigned capacity) { // Crear pila de capacidad limitada
	struct Stack *stack = (struct Stack *)malloc(sizeof(struct Stack));
	stack->capacity = capacity; // Capacidad de la pila
	stack->top = -1; // Cima de la pila
	stack->array = (int *)malloc(stack->capacity * sizeof(int)); // Array que simula una pila con capacidad limitada
	return stack;
}

int lleno(struct Stack *stack) { return stack->top == stack->capacity - 1; } // Si la pila esta llena, el campo top es igual al campo capacity menos 1

int vacio(struct Stack *stack) { return stack->top == -1; } // Si la pila esta vacia, la cima es -1, como al comienzo

void push(struct Stack *stack, int item) {
	if (lleno(stack)) // Si la pila esta llena, terminar
		return;
	stack->array[++stack->top] = item; // De lo contrario, aumentar el campo top, y agregar elemento en ese indice del campo array
}

int pop(struct Stack *stack) {
	if (vacio(stack)) // Si la pila esta vacia, regresar minimo entero posible
		return INT_MIN;
	return stack->array[stack->top--]; // Decrementar campo de la cima (top)
}

int top(struct Stack *stack) {
	if (vacio(stack)) // Si la pila esta vacia
		return INT_MIN; // Regresar minimo entero posible
	return stack->array[stack->top]; // Regresar elemento de la cima
}
