#include "map"

#ifndef _STRUCTS_H_
#define _STRUCTS_H_

// Edge list
typedef struct {
  unsigned int u;
  unsigned int v;
} edge;

typedef struct {
  unsigned int m;
  edge *list;
} edgeList;

// Adjacency matrix
typedef struct {
  unsigned int n;
  int **matrix;
} adjMatrix;

// Adjacency array
typedef struct {
  unsigned int n;
  unsigned int m;
  unsigned int *cd;
  unsigned int *adj;
} adjArray;

unsigned int sizeofEdgeList(edgeList*);
unsigned int sizeofAdjMatrix(adjMatrix*);
unsigned int sizeofAdjArray(adjArray*);

void freeEdgeList(edgeList*);
void freeAdjMatrix(adjMatrix*);
void freeAdjArray(adjArray*);

std::map<unsigned int, unsigned int> *size(char*, unsigned int*, unsigned int*);
edgeList *loadAsEdgeList(char*, int);
adjMatrix *loadAsAdjMatrix(char*, int, std::map<unsigned int, unsigned int>*);
adjArray *loadAsAdjArray(char*, int, int, std::map<unsigned int, unsigned int>*);

#endif
