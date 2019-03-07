#include <stdio.h>
#include "structs.h"
#include "algs.h"
#include <set>
#include <cmath>

int nbTriangles(adjArray *arr) {
  int nb = 0; int i, j, k;
  for(i = 0; i < arr->n; i++) {
    std::set<unsigned int> unei; // Tous les voisins de u
    for(j = arr->cd[i]; j < arr->cd[i+1]; j++) unei.insert(arr->adj[j]);
    for(j = arr->cd[i]; j < arr->cd[i+1]; j++) {
      unsigned int nj = arr->adj[j];
      for(k = arr->cd[nj]; k < arr->cd[nj+1]; k++) {
        if(unei.find(arr->adj[k]) != unei.end()) nb++;
      }
    }
  }
  return nb;
}

int nbTriplets(adjArray *arr) {
  int nb = 0; int i, j, k;
  for(i = 0; i < arr->n; i++) {
    for(j = arr->cd[i]; j < arr->cd[i+1]; j++) {
      unsigned int nj = arr->adj[j];
      for(k = arr->cd[nj]; k < arr->cd[nj+1]; k++) nb++;
    }
  }
  return nb;
}

double transitivity(adjArray *arr, int nbTrip) {
  int nb = nbTriangles(arr);
  return (double) (3 * nb) / (double) nbTrip;
}

double clustering(adjArray *arr) {
  int i, j, k;
  int *degs = (int *)malloc(arr->n*sizeof(int));
  for(i = 0; i < arr->n; i++) degs[i] = arr->cd[i+1]-arr->cd[i];
  for(i = 0; i < arr->m; i++) degs[arr->adj[i]]++;

  double *cc = (double *)malloc(arr->n*sizeof(double));
  for(i = 0; i < arr->n; i++) {
    int nb = 0;
    std::set<unsigned int> unei; // Tous les voisins de u
    for(j = arr->cd[i]; j < arr->cd[i+1]; j++) unei.insert(arr->adj[j]);
    for(j = arr->cd[i]; j < arr->cd[i+1]; j++) {
      unsigned int nj = arr->adj[j];
      for(k = arr->cd[nj]; k < arr->cd[nj+1]; k++) {
        if(unei.find(arr->adj[k]) != unei.end()) nb++;
      }
    }
    if(nb == 0) cc[i] = 0;
    else cc[i] = (double)nb / (double) ((degs[i]*(degs[i]-1))/2);
  }
  double c = 0;
  for(i = 0; i < arr->n; i++) {
    c += cc[i];
  }
  free(cc); free(degs);
  return c / (double) arr->n;
}

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("usage : ./exo4 <input_file>\n");
    return 1;
  }
  char *inputName = argv[1];
  unsigned int nbNodes = 0; unsigned int nbEdges = 0;
  std::map<unsigned int, unsigned int> *map = size(inputName, &nbNodes, &nbEdges);

  adjArray *arr = loadAsAdjArray(inputName, nbNodes, nbEdges, map);
  delete map;

  printf("Loaded the graph as adj array\n");
  int nbTri = nbTriplets(arr);
  sortDescendingDegrees(arr, nbNodes);
  printf("Sorted by descending degrees\n");

  long startTime = clock();
  printf("Nb de triangles: %d\n", nbTriangles(arr));
  printf("Execution time: %ldms\n", (1000*(clock()-startTime))/CLOCKS_PER_SEC);
  printf("Ratio de transitivité: %f\n", transitivity(arr, nbTri));
  printf("Clustering: %f\n", clustering(arr));

  freeAdjArray(arr);
  return 0;
}
