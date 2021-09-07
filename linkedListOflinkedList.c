#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int numVertices = 0;

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

void *chkMalloc (size_t sz) {
	void *mem = malloc (sz);

	// Just fail immediately on error.

	if (mem == NULL) {
		printf ("Out of memory! Exiting.\n");
		exit (1);
	}

	// Otherwise we know it worked.

	return mem;
}

void addVertex (tVertex **first, char *name) {
	// Insert new item at start.

	tVertex *newest = chkMalloc (sizeof (*newest));
	strcpy (newest->name, name);
	newest->next = *first;
	newest->id = numVertices++;
	*first = newest;
}

/*void addEdge (tVertex *first, char *name, int length, int dir) {*/
/*// Insert at start of list.*/

/*tEdge *newest = chkMalloc (sizeof (*newest));*/
/*strcpy (newest->name, name);*/
/*newest->next = first->firstEdge;*/
/*newest->length = length;*/
/*newest->dir = dir;*/
/*first->firstEdge = newest;*/
/*}*/

void addVertexEdge (tVertex *first, char* name_a, char *name_b, int length_b) {
	// Insert at start of list.
	tVertex *selected, *pointed;
	tEdge *newest = chkMalloc (sizeof (*newest));
	while (first != NULL) {
		if (strcmp(first->name, name_a) == 0) {
			selected = first;
		} else if (strcmp(first->name, name_b) == 0) {
			pointed = first;
		}
		first = first->next;
	}

	strcpy (newest->name, name_b);
	newest->next = selected->firstEdge;
	newest->length = length_b;
	newest->id = pointed->id;
	selected->firstEdge = newest;
}

void addEdge (tVertex *first, char* name_a, char* name_b, int length_b, int dir) {
	if (dir == 0) {
		addVertexEdge(first, name_a, name_b, length_b);
		addVertexEdge(first, name_b, name_a, length_b);
	} else if (dir > 0) {
		addVertexEdge(first, name_a, name_b, length_b);
	} else if (dir < 0) {
		addVertexEdge(first, name_b, name_a, length_b);
	}
}

void dumpDetails (tVertex *currVertex) {
	// For every child.

	while (currVertex != NULL) {
		printf ("%s(%lu) has:\n", currVertex->name, currVertex->id);

		// For every toy that child has.

		tEdge *currEdge = currVertex->firstEdge;
		if (currEdge == NULL) {
			printf ("   <<nothing>>\n");
		} else {
			while (currEdge != NULL) {
				printf ("   [%s(%lu), %d]\n", currEdge->name, currEdge->id, currEdge->length);
				currEdge = currEdge->next;
			}
		}
		currVertex = currVertex->next;
	}
}

void primAlg (tVertex *currVertex, tVertex **minGraph) {
	tVertex *firstVertex = currVertex;
	int numEdge = 0, selected[numVertices], min;

	memset(selected, 0, sizeof(selected));

	selected[numVertices - 1] = 1;

	while (currVertex != NULL) {
		addVertex (*&minGraph, currVertex->name);
		currVertex = currVertex->next;
	}

	while(numEdge < numVertices - 1) {
		min = 999999;
		currVertex = firstVertex;

		while (currVertex != NULL) {
			if (selected[currVertex->id]) {
				tEdge *currEdge = currVertex->firstEdge;
				while (currEdge != NULL) {
					if(!selected[currEdge->id]) {
						if (min > currEdge->length) {
							min = currEdge->length;
							/*y = currEdge->id;*/
							selected[currEdge->id] = 1;
							/*printf("[%s] - [%s] : %d\n", currVertex-> name, currEdge->name, currEdge->length);*/
							addEdge (*minGraph, currVertex->name, currEdge->name, currEdge->length, 0);
						}
					}
					currEdge = currEdge->next;
				}
			}
			currVertex = currVertex->next;
		}
		numEdge++;
	}
	*&minGraph = minGraph;
}

int main (void) {
	tVertex *firstVertex = NULL;
	tVertex *minGraph = NULL;

	addVertex (&firstVertex, "A");
	addVertex (&firstVertex, "B");
	addVertex (&firstVertex, "C");
	addVertex (&firstVertex, "D");
	addVertex (&firstVertex, "E");
	addVertex (&firstVertex, "F");

	addEdge (firstVertex, "A", "B", 4, 0);
	addEdge (firstVertex, "A", "C", 4, 0);
	addEdge (firstVertex, "B", "C", 2, 0);
	addEdge (firstVertex, "C", "D", 3, 0);
	addEdge (firstVertex, "C", "E", 2, 0);
	addEdge (firstVertex, "C", "F", 4, 0);
	addEdge (firstVertex, "D", "F", 3, 0);
	addEdge (firstVertex, "E", "F", 3, 0);
	/*addEdge (firstVertex, "A", "D", 1, 0);*/

	dumpDetails (firstVertex);

	primAlg (firstVertex, &minGraph);
	printf("\n*** MINIMIZED ***\n");

	dumpDetails (minGraph);

	return 0;
}
