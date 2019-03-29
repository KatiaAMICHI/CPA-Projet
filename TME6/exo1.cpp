#include <vector>
#include <algorithm>
#include <set>
#include "structs.h"
#include "heap.h"

std::map<unsigned int, unsigned int> *reverseMap(std::map<unsigned int, unsigned int> *map) {
  std::map<unsigned int, unsigned int>  *revMap = new std::map<unsigned int, unsigned int>();
  for(std::pair<unsigned int, unsigned int> p: *map) {
    (*revMap)[p.second] = p.first;
  }
  return revMap;
}

std::vector<std::pair<int, int>> *coreOrdering(adjArray *arr) {
  int *degs = (int *) malloc(arr->n*sizeof(int));
  for(int i = 0; i < arr->n; i++) degs[i] = arr->cd[i+1] - arr->cd[i];

  std::vector<std::pair<int, int>> *coresAndOrder = new std::vector<std::pair<int, int>>();
  for(int i = 0; i < arr->n; i++) {
    std::pair<int, int> p;
    coresAndOrder->push_back(p);
  }

  Heap V(arr->n, degs);
  for(unsigned int i = 0; i < arr->n; i++) V.insert(i);
  int i = arr->n-1; int c = 0;
  while(not V.getSize() == 0) {
    unsigned int node = V.remove();
    if(degs[node] > c) c = degs[node];
    (*coresAndOrder)[node].first = c;
    (*coresAndOrder)[node].second = i;

    for(int j = arr->cd[node]; j < arr->cd[node+1]; j++) {
      degs[arr->adj[j]]--;
      V.percolateUp(arr->adj[j]);
    }

    i = i - 1;
  }

  free(degs);
  return coresAndOrder;
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

void writeCores(char *fn, std::vector<std::pair<unsigned int, double>> cores) {
  FILE *f = fopen(fn, "w");
  for(std::pair<int, double> p: cores) {
    fprintf(f, "%d %.15g\n", p.first, p.second); //%.17g pour plus de précision d'affichage
  }
  fclose(f);
}

int main(int argc, char **argv) {
  if(argc != 3) {
    printf("usage : ./exo1.exe <input_file> <output_file>\n");
    return 1;
  }
  char *inName = argv[1];
  char *outName = argv[2];

  unsigned int nbNodes = 0; unsigned int nbEdges = 0;
  std::map<unsigned int, unsigned int> *map = size(inName, &nbNodes, &nbEdges);
  std::map<unsigned int, unsigned int> *revMap = reverseMap(map);

  adjArray *arr = loadAsAdjArray(inName, nbNodes, nbEdges, map);
  printf("Loaded graph\n");

  std::vector<std::pair<int, int>> *coresAndOrder = coreOrdering(arr);
  printf("Calculated cores\n");

  int maxC = (*coresAndOrder)[0].first;
  for(int i = 0; i < arr->n; i++) {
    if((*coresAndOrder)[i].first > maxC) maxC = (*coresAndOrder)[i].first;
  }
  printf("Core value: %d\n", maxC);

  std::vector<std::pair<unsigned int, double>> coreMap(arr->n);
  for(int i = 0; i < arr->n; i++) {
    coreMap[i] = std::pair<unsigned int, double>((*revMap)[i], (*coresAndOrder)[i].first);
  }

  std::vector<unsigned int> ordering(arr->n);
  for(int i = 0; i < arr->n; i++) {
    ordering[(*coresAndOrder)[i].second] = i;
  }
  std::set<unsigned int> *densest = densestPrefix(arr, ordering);

  writeCores(outName, coreMap);
  printf("Written cores\n");

  freeAdjArray(arr);
  delete map; delete revMap;
  delete coresAndOrder; delete densest;
  return 0;
}
