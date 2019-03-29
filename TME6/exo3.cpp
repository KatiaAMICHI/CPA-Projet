#include <vector>
#include <algorithm>
#include <set>
#include "structs.h"

std::map<unsigned int, unsigned int> *reverseMap(std::map<unsigned int, unsigned int> *map) {
  std::map<unsigned int, unsigned int>  *revMap = new std::map<unsigned int, unsigned int>();
  for(std::pair<unsigned int, unsigned int> p: *map) {
    (*revMap)[p.second] = p.first;
  }
  return revMap;
}

double *computeDensity(adjArray *arr, int t) {
  double *r = (double *) malloc(arr->n*sizeof(double));
  for(int i = 0; i < arr->n; i++) r[i] = 0;

  for(int k = 0; k < t; k++) {
    for(int i = 0; i < arr->n; i++) {
      for(int j = arr->cd[i]; j < arr->cd[i+1]; j++) {
        int nj = arr->adj[j];
        if(r[i] <= r[nj]) r[i]++;
        else r[nj]++;
      }
    }
  }

  for(int k = 0; k < arr->n; k++) r[k] = r[k]/(double)(2*t); // 2*t car on passe 2 fois sur chaque arete a cause de la forme du adjArray
  return r;
}

std::set<unsigned int> *densestPrefix(adjArray *arr, std::vector<unsigned int> ordering) {
  unsigned int* revOrdering = (unsigned int *)malloc(arr->n*sizeof(unsigned int));
  for(int i = 0; i < arr->n; i++) {
    revOrdering[ordering[i]] = i;
  }

  double bestDens = 0;
  int bestN = 0; int bestM = 0;
  std::set<unsigned int> currentSet;
  int currentM = 0;

  for(int i = 0; i < arr->n; i++) {
    unsigned int node = ordering[i];
    currentSet.insert(node);
    for(int j = arr->cd[node]; j < arr->cd[node+1]; j++) {
      if(revOrdering[arr->adj[j]] < i) currentM++;
    }
    double currentDens = (double)currentM/(double)(i+1);
    if(currentDens >= bestDens) {
      bestDens = currentDens;
      bestN = i+1; bestM = currentM;
    }
  }

  std::set<unsigned int> *densest = new std::set<unsigned int>();
  for(int i = 0; i < bestN; i++) {
    (*densest).insert(ordering[i]);
  }

  printf("Densest prefix avg degree density: %f\n", bestDens);
  printf("Densest prefix avg edge density: %f\n", (double)bestM/(double)((double)(bestN*(bestN-1))/(double)2));
  printf("Densest prefix size: %d\n", bestN);

  free(revOrdering);
  return densest;
}

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("usage : ./exo3.exe <input_file>\n");
    return 1;
  }
  char *inName = argv[1];

  unsigned int nbNodes = 0; unsigned int nbEdges = 0;
  std::map<unsigned int, unsigned int> *map = size(inName, &nbNodes, &nbEdges);

  adjArray *arr = loadAsAdjArray(inName, nbNodes, nbEdges, map);
  printf("Loaded graph\n");

  double *dens = computeDensity(arr, 100);
  printf("Calculated node density\n");

  std::vector<unsigned int> orderedNodes;
  for(int i = 0; i < arr->n; i++) orderedNodes.push_back(i);

  std::sort(orderedNodes.begin(), orderedNodes.end(),
            [dens] (unsigned int i1, unsigned int i2) -> int {
              return dens[i1] > dens[i2];
            });

  printf("Highest density score: %f\n", dens[orderedNodes[0]]);

  densestPrefix(arr, orderedNodes);

  freeAdjArray(arr);
  delete map;
  return 0;
}
