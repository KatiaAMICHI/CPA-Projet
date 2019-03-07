#include <stdio.h>
#include <map>
#include "structs.h"
#include "algs.h"

double fractionInMainComponent(adjArray *arr, int n) {
  int *marks = bfs(arr, 0, n);
  int i;
  int inMainComp = 0;
  for(i = 0; i < n; i++) {
    if(marks[i] != -1) inMainComp++;
  }
  free(marks);
  return ((double)inMainComp)/((double)n);
}

int diameter(adjArray *arr, int n) {
  int s = 0; int maxDiam = 0; int i;
  for(i = 0; i < 10; i++) {
    int *marks = bfs(arr, s, n);
    int furthest = s; int maxDist = 0;
    int j;
    for(j = 0; j < n; j++) {
      if(marks[j] > maxDist) {
        maxDist = marks[j];
        furthest = j;
      }
    }
    s = furthest ;
    if(maxDist > maxDiam) maxDiam = maxDist;
    free(marks);
  }
  return maxDiam+1; // +1 parce que sinon on ne prends pas en compte le noeud de départ
}

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("usage : ./exo3 <input_file>\n");
    return 1;
  }
  char *inputName = argv[1];
  unsigned int nbNodes = 0; unsigned int nbEdges = 0;
  std::map<unsigned int, unsigned int> *map = size(inputName, &nbNodes, &nbEdges);

  adjArray *arr = loadAsAdjArray(inputName, nbNodes, nbEdges, map);
  delete map;
  printf("Loaded the graph as adj array\n");
  printf("Fraction dans la composante principale : %f\n", fractionInMainComponent(arr, nbNodes));
  printf("Borne inf du diametre: %d\n", diameter(arr, nbNodes));

  freeAdjArray(arr);
  return 0;
}
