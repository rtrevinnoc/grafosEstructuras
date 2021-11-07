#include "queue.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// Crear cola de maxima capacidad capacity
struct Queue *createQueue(unsigned capacity) {
	struct Queue *queue = (struct Queue *)malloc(sizeof(struct Queue)); // Reservar espacio para el struct Queue
	queue->capacity = capacity; // El campo capacidad es igual al parametro
	queue->front = queue->size = 0; // El frente de la cola es igual al tamaño, que es 0

	queue->rear = capacity - 1; // El final de la cola es de la capacidad maxima menos 1
	queue->array = (int *)malloc(queue->capacity * sizeof(int)); // crear un array de tamaño del campo capacidad para enteros
	return queue;
}

// Si la cola esta llena, el campo del tamaño debera ser igual al de capacidad maxima
int colaLlena(struct Queue *queue) { return (queue->size == queue->capacity); }

// Si la cola esta vacia, su campo tamaño es 0
int colaVacia(struct Queue *queue) { return (queue->size == 0); }

// Agregar elemento a la cola
void enqueue(struct Queue *queue, int item) {
	if (colaLlena(queue)) // Si esta llena, terminar
		return;
	queue->rear = (queue->rear + 1) % queue->capacity; // El final de la cola es el modulo de 1 mas el final anterior con la capacida maxima
	queue->array[queue->rear] = item; // Insertar el elemento en el arreglo, en el indice del campo final
	queue->size = queue->size + 1; // Aumentar el tamaño de la cola
}

// Eliminar elemento de la cola
int dequeue(struct Queue *queue) {
	if (colaVacia(queue)) // Si esta vacia
		return INT_MIN; // Regresar entero minimo posible
	int item = queue->array[queue->front]; // El elemento a regresar es el que esta en el indice del campo front del array que simula la pila
	queue->front = (queue->front + 1) % queue->capacity; // El frente de la pila es el modulo entre 1 masel frente actual limitado por la capacidad maxima de la pila
	queue->size = queue->size - 1; // Aumentar el tamaño de la cola
	return item; // Regresar el elemento borrado
}

// frente de la cola
int front(struct Queue *queue) {
	if (colaVacia(queue)) // Si la cola esta vacia
		return INT_MIN; // Regresar minimo
	return queue->array[queue->front]; // Mostrar el elemento de la pila que esta en el indice del campo frente
}

// final de la cola
int rear(struct Queue *queue) {
	if (colaVacia(queue)) // Si la cola esta vacia
		return INT_MIN; // Regresar entero minimo
	return queue->array[queue->rear]; // Regresar elemento en el indice del campo rear del arreglo que simula la pila
}
