#include "queue.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct sEdge {
	char name[50];
	long id;
	int length;
	struct sEdge *next;
} tEdge;

typedef struct sVertex {
	char name[50];
	long id;
	tEdge *firstEdge;
	struct sVertex *next;
} tVertex;

void *chkMalloc(size_t sz) {
	void *mem = malloc(sz); // Just fail immediately on error.  
	if (mem == NULL) { printf("Out of memory! Exiting.\n");
		exit(1);
	}

	// Otherwise we know it worked.

	return mem;
}

void addVertex(tVertex **first, char *name, int *numVertices) {
	// Insert new item at start.

	tVertex *newest = chkMalloc(sizeof(*newest));
	strcpy(newest->name, name);
	newest->next = *first;
	newest->id = (*numVertices)++;
	*first = newest;
}

void addVertexEdge(tVertex *first, char *name_a, char *name_b, int length_b) {
	// Insert at start of list.
	tVertex *selected, *pointed;
	tEdge *newest = chkMalloc(sizeof(*newest));
	while (first != NULL) {
		if (strcmp(first->name, name_a) == 0) {
			selected = first;
		} else if (strcmp(first->name, name_b) == 0) {
			pointed = first;
		}
		first = first->next;
	}

	strcpy(newest->name, name_b);
	newest->next = selected->firstEdge;
	newest->length = length_b;
	newest->id = pointed->id;
	selected->firstEdge = newest;
}

void addEdge(tVertex *first, char *name_a, char *name_b, int length_b,
		int dir) {
	if (dir == 0) {
		addVertexEdge(first, name_a, name_b, length_b);
		addVertexEdge(first, name_b, name_a, length_b);
	} else if (dir > 0) {
		addVertexEdge(first, name_a, name_b, length_b);
	} else if (dir < 0) {
		addVertexEdge(first, name_b, name_a, length_b);
	}
}

void dumpDetails(tVertex *currVertex) {
	// For every child.

	while (currVertex != NULL) {
		printf("%s(%lu) has:\n", currVertex->name, currVertex->id);

		// For every toy that child has.

		tEdge *currEdge = currVertex->firstEdge;
		if (currEdge == NULL) {
			printf("   <<nothing>>\n");
		} else {
			while (currEdge != NULL) {
				printf("   [%s(%lu), %d]\n", currEdge->name, currEdge->id,
						currEdge->length);
				currEdge = currEdge->next;
			}
		}
		currVertex = currVertex->next;
	}
}

void primAlg(tVertex *currVertex, tVertex **minGraph, int *numVertices) {
	tVertex *firstVertex = currVertex;
	int numEdge = 0, selected[*numVertices], min, minPointedId,
	    minGraphVertices = 0;
	char minVertexName[50], minPointedName[50];

	memset(selected, 0, sizeof(selected));

	selected[*numVertices - 1] = 1;

	while (currVertex != NULL) {
		addVertex(*&minGraph, currVertex->name, &minGraphVertices);
		currVertex = currVertex->next;
	}

	while (numEdge < *numVertices - 1) {
		min = INT_MAX;
		currVertex = firstVertex;

		while (currVertex != NULL) {
			if (selected[currVertex->id]) {
				tEdge *currEdge = currVertex->firstEdge;
				while (currEdge != NULL) {
					if (!selected[currEdge->id]) {
						if (min > currEdge->length) {
							min = currEdge->length;
							selected[currEdge->id] = 1;
							addEdge(*minGraph, currVertex->name, currEdge->name,
									currEdge->length, 0);
							/*strcpy (minPointedName, currEdge->name);*/
							/*strcpy (minVertexName, currVertex->name);*/
							/*minPointedId = currEdge->id;*/
							/*y = currEdge->id;*/
							/*printf("[%s] - [%s] : %d\n", currVertex-> name, currEdge->name,
							 * currEdge->length);*/
						}
					}
					currEdge = currEdge->next;
				}
			}
			currVertex = currVertex->next;
		}
		/*selected[minPointedId] = 1;*/
		/*addEdge (*minGraph, minVertexName, minPointedName, min, 0);*/
		numEdge++;
	}
	*&minGraph = minGraph;
}

void kruskalAlg(tVertex *currVertex, tVertex **minGraph, int *numVertices) {
	tVertex *firstVertex = currVertex;
	int numEdge = 0, selected[*numVertices], min, minGraphVertices = 0,
	    iMin_vertex, iMin_pointed;
	char cMin_vertex[50], cMin_pointed[50];

	memset(selected, 0, sizeof(selected));

	while (currVertex != NULL) {
		addVertex(*&minGraph, currVertex->name, &minGraphVertices);
		currVertex = currVertex->next;
	}

	while (numEdge < *numVertices - 1) {
		min = INT_MAX;
		currVertex = firstVertex;

		while (currVertex != NULL) {
			tEdge *currEdge = currVertex->firstEdge;
			while (currEdge != NULL) {
				if ((selected[currVertex->id] || numEdge == 0) &&
						!selected[currEdge->id]) {
					if (min > currEdge->length) {
						min = currEdge->length;
						iMin_vertex = currVertex->id;
						iMin_pointed = currEdge->id;
						strcpy(cMin_vertex, currVertex->name);
						strcpy(cMin_pointed, currEdge->name);
					}
				}
				currEdge = currEdge->next;
			}
			currVertex = currVertex->next;
		}
		selected[iMin_vertex] = 1;
		selected[iMin_pointed] = 1;
		addEdge(*minGraph, cMin_vertex, cMin_pointed, min, 0);
		numEdge++;
	}
	*&minGraph = minGraph;
}

void bfsAlg(tVertex *currVertex, char *pivot, tVertex **minGraph,
		int *numVertices) {
	tVertex *firstVertex = currVertex;
	int numEdge = 0, selected[*numVertices], min, minGraphVertices = 0,
	    iMin_vertex, iMin_pointed;
	char cMin_vertex[50], cMin_pointed[50], order[*numVertices][50];
	struct Queue *queue = createQueue(*numVertices);

	memset(selected, 0, sizeof(selected));

	while (currVertex != NULL) {
		addVertex(*&minGraph, currVertex->name, &minGraphVertices);
		currVertex = currVertex->next;
	}

	while (numEdge < *numVertices) {
		currVertex = firstVertex;

		/*printf("\n#(queue)[%d]=> ", front(queue));*/
		/*for(unsigned num1=0; num1<queue->capacity; num1++) {*/
			/*printf("%d ", queue->array[num1]);*/
		/*}*/
		/*printf("\n");*/

		/*printf("\n*(selected)=> ");*/
		/*for(int num2=0; num2<*numVertices; num2++) {*/
			/*printf("%d ", selected[num2]);*/
		/*}*/
		/*printf("\n");*/

		/*printf("\n*(order)=> ");*/
		/*for(int num3=0; num3<numEdge; num3++) {*/
			/*printf("%s ", order[num3]);*/
		/*}*/
		/*printf("\n");*/

		/*int i;*/
		while (currVertex != NULL) {
			tEdge *currEdge = currVertex->firstEdge;
			while (currEdge != NULL) {
				if (numEdge == 0 && strcmp(currVertex->name, pivot) == 0) {
					selected[currVertex->id] = 1;
					selected[currEdge->id] = 1;
					printf("[%s] - [%s] : %d\n", currVertex-> name, currEdge->name, currEdge->length);
					enqueue(queue, currEdge->id);
					strcpy(order[numEdge], currVertex->name);
				} else if (numEdge > 0 && front(queue) == currVertex->id) {
					/*printf("[%s] - [%s] : %d\n", currVertex-> name, currEdge->name, currEdge->length);*/
					if (!selected[currEdge->id]) {
						printf("[%s] - [%s] : %d\n", currVertex-> name, currEdge->name, currEdge->length);
						enqueue(queue, currEdge->id);
					}
					strcpy(order[numEdge+1], currVertex->name);
				}
				currEdge = currEdge->next;
			}
			currVertex = currVertex->next;
		}
		if (numEdge > 0) dequeue(queue);
		/*strcpy(order[numEdge], dequeue(queue));*/
		numEdge++;
	}

	printf("\n*(order)=> ");
	for(int num3=0; num3<numEdge+1; num3++) {
		printf("%s", order[num3]);
	}
	printf("\n");
	*&minGraph = minGraph;
}

int main(void) {
	tVertex *firstVertex = NULL, *minGraph = NULL;
	int firstGraphVertices = 0;

	addVertex(&firstVertex, "A", &firstGraphVertices);
	addVertex(&firstVertex, "B", &firstGraphVertices);
	addVertex(&firstVertex, "C", &firstGraphVertices);
	addVertex(&firstVertex, "D", &firstGraphVertices);
	addVertex(&firstVertex, "E", &firstGraphVertices);
	addVertex(&firstVertex, "F", &firstGraphVertices);

	/*addEdge (firstVertex, "A", "B", 4, 0);*/
	/*addEdge (firstVertex, "A", "C", 4, 0);*/
	/*addEdge (firstVertex, "B", "C", 2, 0);*/
	/*addEdge (firstVertex, "C", "D", 3, 0);*/
	/*addEdge (firstVertex, "C", "E", 2, 0);*/
	/*addEdge (firstVertex, "C", "F", 4, 0);*/
	/*addEdge (firstVertex, "D", "F", 3, 0);*/
	/*addEdge (firstVertex, "E", "F", 3, 0);*/

	/*addEdge (firstVertex, "A", "D", 1, 0);*/

	
	addEdge(firstVertex, "A", "B", 1, 0);
	addEdge(firstVertex, "A", "D", 4, 0);
	addEdge(firstVertex, "A", "E", 3, 0);
	addEdge(firstVertex, "B", "D", 4, 0);
	addEdge(firstVertex, "D", "E", 4, 0);
	addEdge(firstVertex, "B", "E", 2, 0);
	addEdge(firstVertex, "E", "F", 7, 0);
	addEdge(firstVertex, "F", "C", 5, 0);
	addEdge(firstVertex, "C", "E", 4, 0);

	dumpDetails(firstVertex);

	/*kruskalAlg (firstVertex, &minGraph, &firstGraphVertices);*/
	bfsAlg(firstVertex, "A", &minGraph, &firstGraphVertices);
	/*printf("\n*** MINIMIZED ***\n");*/

	/*dumpDetails(minGraph);*/

	/*struct Queue *queue = createQueue(1000);*/

	/*enqueue(queue, 10);*/
	/*enqueue(queue, 20);*/
	/*enqueue(queue, 30);*/
	/*enqueue(queue, 40);*/

	/*printf("%d dequeued from queue\n\n", dequeue(queue));*/

	/*printf("Front item is %d\n", front(queue));*/
	/*printf("Rear item is %d\n", rear(queue));*/

	return 0;
}