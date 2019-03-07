#include <stdlib.h>
#include <stdio.h>
#include "structs.h"

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("usage : ./exo2 <input_file>\n");
    return 1;
  }
  char *inputName = argv[1];
  unsigned int nbNodes = 0; unsigned int nbEdges = 0;
  std::map<unsigned int, unsigned int> *map = size(inputName, &nbNodes, &nbEdges);

  // Edge list
  edgeList *eList = loadAsEdgeList(inputName, nbEdges);
  printf("Size of edge list : %d\n", sizeofEdgeList(eList));
  freeEdgeList(eList);

  /* adjMatrix *mat = loadAsAdjMatrix(inputName, nbNodes, map); */
  /* printf("Size of adjacency matrix : %d\n", sizeofAdjMatrix(mat)); */
  /* freeAdjMatrix(mat); */

  adjArray *arr = loadAsAdjArray(inputName, nbNodes, nbEdges, map);
  printf("Size of adjacency array: %d\n", sizeofAdjArray(arr));
  freeAdjArray(arr);

  delete map;
  return 0;
}
