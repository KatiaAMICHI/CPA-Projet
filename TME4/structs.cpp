#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "structs.h"

unsigned int sizeofEdgeList(edgeList *e) {
  return sizeof(unsigned int) + e->m*sizeof(edge);
}

void freeEdgeList(edgeList *e) {
  free(e->list);
  free(e);
}

unsigned int sizeofAdjMatrix(adjMatrix *mat) {
  return sizeof(unsigned int) + mat->n*mat->n*sizeof(int);
}

void freeAdjMatrix(adjMatrix *mat) {
  unsigned int i;
  for(i = 0; i < mat->n; i++) free(mat->matrix[i]);
  free(mat->matrix);
  free(mat);
}

unsigned int sizeofAdjArray(adjArray *arr) {
  return 2*sizeof(unsigned int) + arr->n*sizeof(unsigned int) + arr->m*sizeof(unsigned int);
}

void freeAdjArray(adjArray *arr) {
  free(arr->cd);
  free(arr->adj);
  free(arr);
}

std::map<unsigned int, unsigned int> *size(char *fn, unsigned int *nbNodes, unsigned int *nbEdges) {
  FILE *f = fopen(fn, "r");
  std::map<unsigned int, unsigned int> *nodesM = new std::map<unsigned int, unsigned int>;
  char line[128];
  *nbNodes = 0;
  while(fgets(line, sizeof line, f) != NULL) {
    if(line[0] == '#') continue;
    (*nbEdges)++;
    unsigned int nl = atoi(strtok(line, " \t"));
    unsigned int nr = atoi(strtok(NULL, " \t"));
    if(nodesM->find(nl) == nodesM->end()) {
      (*nodesM)[nl] = *nbNodes;
      (*nbNodes)++;
    }
    if(nodesM->find(nr) == nodesM->end()) {
      (*nodesM)[nr] = *nbNodes;
      (*nbNodes)++;
    }
  }
  fclose(f);
  return nodesM;
}

edgeList *loadAsEdgeList(char *fn, int m) {
  edgeList *eList = (edgeList*)malloc(sizeof(edgeList));
  eList->m = m;
  eList->list = (edge *)malloc(eList->m*sizeof(edge));

  FILE *f = fopen(fn, "r");
  char line[128];
  int i = 0;
  while(fgets(line, sizeof line, f) != NULL) {
    if(line[0] == '#') continue;
    eList->list[i].u = atoi(strtok(line, " \t"));
    eList->list[i].v = atoi(strtok(NULL, " \t"));
    i++;
  }
  fclose(f);
  return eList;
}

adjMatrix *loadAsAdjMatrix(char *fn, int n, std::map<unsigned int, unsigned int> *map) {
  adjMatrix *mat = (adjMatrix*)malloc(sizeof(adjMatrix));
  mat->n = n;
  mat->matrix = (int**)malloc(mat->n*sizeof(unsigned int*));
  unsigned int i, j;
  for(i = 0; i < mat->n; i++) {
    mat->matrix[i] = (int*)malloc(mat->n*sizeof(unsigned int));
    for(j = 0; j < mat->n; j++) {
      mat->matrix[i][j] = 0;
    }
  }

  FILE *f = fopen(fn, "r");
  char line[128];
  while(fgets(line, sizeof line, f) != NULL) {
    if(line[0] == '#') continue;
    unsigned int nl = atoi(strtok(line, " \t"));
    unsigned int nr = atoi(strtok(NULL, " \t"));
    mat->matrix[(*map)[nl]][(*map)[nr]] = 1;
    mat->matrix[(*map)[nr]][(*map)[nl]] = 1;
  }
  fclose(f);
  return mat;
}

int *degrees(char *fn, unsigned int n, std::map<unsigned int, unsigned int> *m) {
  int *degs = (int *)malloc(sizeof(int)*n);
  unsigned int i;
  for(i = 0; i < n; i++) degs[i] = 0;
  FILE *f = fopen(fn, "r");
  char line[128];
  while(fgets(line, sizeof line, f) != NULL) {
    if(line[0] == '#') continue;
    unsigned int nl = atoi(strtok(line, " \t"));
    unsigned int nr = atoi(strtok(NULL, " \t"));
    degs[(*m)[nl]]++; degs[(*m)[nr]]++;
  }
  fclose(f);
  return degs;
}

adjArray *loadAsAdjArray(char *fn, int n, int m, std::map<unsigned int, unsigned int> *map) {
  adjArray *arr = (adjArray*)malloc(sizeof(adjArray));
  arr->n = n; arr->m = m;
  int *degs = degrees(fn, n, map);
  arr->cd = (unsigned int *)malloc((n+1)*sizeof(unsigned int)); arr->cd[0] = 0;
  int i;
  for(i = 0; i < n; i++) arr->cd[i+1] = degs[i]+arr->cd[i];
  arr->adj = (unsigned int*)malloc(arr->cd[n]*sizeof(unsigned int));
  for(i = 0; i < arr->cd[n]; i++) arr->adj[i] = 0;

  int *indexes = (int *) malloc(n*sizeof(int));
  for(i = 0; i < n; i++) indexes[i] = 0;

  FILE *f = fopen(fn, "r");
  char line[128];
  while(fgets(line, sizeof line, f) != NULL) {
    if(line[0] == '#') continue;
    unsigned int nl = (*map)[atoi(strtok(line, " \t"))];
    unsigned int nr = (*map)[atoi(strtok(NULL, " \t"))];
    arr->adj[arr->cd[nl]+indexes[nl]] = nr;
    arr->adj[arr->cd[nr]+indexes[nr]] = nl;
    indexes[nl]++; indexes[nr]++;
  }

  fclose(f);
  free(degs); free(indexes);
  return arr;
}
