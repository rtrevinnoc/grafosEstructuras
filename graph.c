#include "queue.h"
#include "stack.h"
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
	if (mem == NULL) {
		printf("Out of memory! Exiting.\n");
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

	tEdge *currEdge = selected->firstEdge;
	int i = 0;
	while (currEdge != NULL) {
		if (strcmp(currEdge->name, name_b) == 0)
			i++;
		currEdge = currEdge->next;
	}

	if (selected->firstEdge == NULL || i == 0) {
		strcpy(newest->name, name_b);
		newest->next = selected->firstEdge;
		newest->length = length_b;
		newest->id = pointed->id;
		selected->firstEdge = newest;
	}
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
	int numEdge = 0, selected[*numVertices], minGraphVertices = 0;
	char order[*numVertices][50];
	struct Queue *queue = createQueue(*numVertices);

	memset(selected, 0, sizeof(selected));

	while (currVertex != NULL) {
		addVertex(*&minGraph, currVertex->name, &minGraphVertices);
		currVertex = currVertex->next;
	}

	while (numEdge < *numVertices) {
		currVertex = firstVertex;
		while (currVertex != NULL) {
			tEdge *currEdge = currVertex->firstEdge;
			while (currEdge != NULL) {
				if (numEdge == 0 && strcmp(currVertex->name, pivot) == 0) {
					selected[currVertex->id] = 1;
					selected[currEdge->id] = 1;
					enqueue(queue, currEdge->id);
					strcpy(order[numEdge], currVertex->name);
					addEdge(*minGraph, currVertex->name, currEdge->name, currEdge->length,
							0);
				} else if (numEdge > 0 && front(queue) == currVertex->id) {
					if (!selected[currEdge->id]) {
						enqueue(queue, currEdge->id);
						addEdge(*minGraph, currVertex->name, currEdge->name,
								currEdge->length, 0);
					}
					strcpy(order[numEdge], currVertex->name);
				}
				currEdge = currEdge->next;
			}
			currVertex = currVertex->next;
		}
		if (numEdge > 0)
			dequeue(queue);
		numEdge++;
	}

	printf("\n#(order)=> ");
	for (int i = 0; i < numEdge; i++) {
		printf("->%s", order[i]);
	}
	printf("\n");
	*&minGraph = minGraph;
}

void dfsAlg(tVertex *currVertex, char *pivot, tVertex **minGraph,
		int *numVertices) {
	tVertex *firstVertex = currVertex;
	int numEdge = 0, selected[*numVertices], minGraphVertices = 0;
	char order[*numVertices][50];
	struct Stack *stack = createStack(*numVertices);

	memset(selected, 0, sizeof(selected));

	while (currVertex != NULL) {
		addVertex(*&minGraph, currVertex->name, &minGraphVertices);
		currVertex = currVertex->next;
	}

	while (numEdge < *numVertices) {
		currVertex = firstVertex;

		while (currVertex != NULL) {
			tEdge *currEdge = currVertex->firstEdge;
			if (numEdge == 0 && strcmp(currVertex->name, pivot) == 0) {
				while (currEdge != NULL) {
					addEdge(*minGraph, currVertex->name, currEdge->name, currEdge->length, 0);
					push(stack, currEdge->id);
					strcpy(order[numEdge], currVertex->name);
					selected[currEdge->id] = 1;
					currEdge = currEdge->next;
				}
				selected[currVertex->id] = 1;
			} else if (numEdge > 0 && top(stack) == currVertex->id) {
				pop(stack);
				while (currEdge != NULL) {
					if (!selected[currEdge->id]) {
						push(stack, currEdge->id);
						addEdge(*minGraph, currVertex->name, currEdge->name, currEdge->length, 0);
					}
					currEdge = currEdge->next;
				}
				strcpy(order[numEdge], currVertex->name);
				selected[currVertex->id] = 1;
				numEdge++;
			}
			currVertex = currVertex->next;
		}
		if (numEdge == 0) numEdge++;
	}

	printf("\n#(order)=> ");
	for (int i = 0; i < numEdge; i++) {
		printf("->%s", order[i]);
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

	addVertex(&firstVertex, "G", &firstGraphVertices);
	addVertex(&firstVertex, "H", &firstGraphVertices);

	/*addEdge (firstVertex, "A", "B", 4, 0);*/
	/*addEdge (firstVertex, "A", "C", 4, 0);*/
	/*addEdge (firstVertex, "B", "C", 2, 0);*/
	/*addEdge (firstVertex, "C", "D", 3, 0);*/
	/*addEdge (firstVertex, "C", "E", 2, 0);*/
	/*addEdge (firstVertex, "C", "F", 4, 0);*/
	/*addEdge (firstVertex, "D", "F", 3, 0);*/
	/*addEdge (firstVertex, "E", "F", 3, 0);*/

	/*addEdge (firstVertex, "A", "D", 1, 0);*/

	/*addEdge(firstVertex, "A", "B", 1, 0);*/
	/*addEdge(firstVertex, "A", "D", 4, 0);*/
	/*addEdge(firstVertex, "A", "E", 3, 0);*/
	/*addEdge(firstVertex, "B", "D", 4, 0);*/
	/*addEdge(firstVertex, "D", "E", 4, 0);*/
	/*addEdge(firstVertex, "B", "E", 2, 0);*/
	/*addEdge(firstVertex, "E", "F", 7, 0);*/
	/*addEdge(firstVertex, "F", "C", 5, 0);*/
	/*addEdge(firstVertex, "C", "E", 4, 0);*/

	addEdge(firstVertex, "A", "B", 4, 0);
	addEdge(firstVertex, "B", "G", 4, 0);
	addEdge(firstVertex, "B", "H", 4, 0);
	addEdge(firstVertex, "A", "C", 4, 0);
	addEdge(firstVertex, "C", "E", 4, 0);
	addEdge(firstVertex, "C", "D", 4, 0);
	addEdge(firstVertex, "C", "F", 4, 0);


	dumpDetails(firstVertex);

	printf("\n*** MINIMIZED ***\n");
	/*kruskalAlg (firstVertex, &minGraph, &firstGraphVertices);*/
	bfsAlg(firstVertex, "A", &minGraph, &firstGraphVertices);

	/*dumpDetails(minGraph);*/

	return 0;
}
