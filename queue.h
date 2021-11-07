#ifndef QUEUE_H_ /* Include guard */
#define QUEUE_H_

// Struct con un array que representa una cola
struct Queue {
  int front, rear, size; // enteros para el indice del frente, el final y el tamaño de la cola
  unsigned capacity; // entero con el tamaño de la cola
  int *array; // array que representa la cola
};

struct Queue *createQueue(unsigned capacity);

 int colaVacia(struct Queue *queue);

void enqueue(struct Queue *queue, int item);

int dequeue(struct Queue *queue);

int front(struct Queue *queue);

int rear(struct Queue *queue);

#endif
