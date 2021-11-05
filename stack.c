#include "stack.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

struct Stack *createStack(unsigned capacity) {
	struct Stack *stack = (struct Stack *)malloc(sizeof(struct Stack));
	stack->capacity = capacity;
	stack->top = -1;
	stack->array = (int *)malloc(stack->capacity * sizeof(int));
	return stack;
}

int isStackFull(struct Stack *stack) { return stack->top == stack->capacity - 1; }

int isStackEmpty(struct Stack *stack) { return stack->top == -1; }

void push(struct Stack *stack, int item) {
	if (isStackFull(stack))
		return;
	stack->array[++stack->top] = item;
}

int pop(struct Stack *stack) {
	if (isStackEmpty(stack))
		return INT_MIN;
	return stack->array[stack->top--];
}

int top(struct Stack *stack) {
	if (isStackEmpty(stack))
		return INT_MIN;
	return stack->array[stack->top];
}
