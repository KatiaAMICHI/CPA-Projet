#include "map"

#ifndef _STRUCTS_H_
#define _STRUCTS_H_

// Adjacency array
typedef struct {
  unsigned int n;
  unsigned int m;
  unsigned int *cd;
  unsigned int *adj;
} adjArray;

unsigned int sizeofAdjArray(adjArray*);
void freeAdjArray(adjArray*);

std::map<unsigned int, unsigned int> *size(char*, unsigned int*, unsigned int*);
adjArray *loadAsAdjArray(char*, int, int, std::map<unsigned int, unsigned int>*);

#endif
