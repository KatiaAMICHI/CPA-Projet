#include <vector>
#include <list>
#include <algorithm>
#include <set>
#include "structs.h"
#include "assert.h"

std::map<unsigned int, unsigned int> *reverseMap(std::map<unsigned int, unsigned int> *map) {
  std::map<unsigned int, unsigned int>  *revMap = new std::map<unsigned int, unsigned int>();
  for(std::pair<unsigned int, unsigned int> p: *map) {
    (*revMap)[p.second] = p.first;
  }
  return revMap;
}

void sortDescendingDegrees(adjArray *arr) {
  int *degs = (int *)malloc(arr->n*sizeof(int));
  int i;
  for(i = 0; i < arr->n; i++) degs[i] = arr->cd[i+1]-arr->cd[i];

  int maxD = 0;
  for(i = 0; i < arr->n; i++) if(degs[i] > maxD) maxD = degs[i];
  std::list<unsigned int> **buckets = (std::list<unsigned int>**) malloc(maxD*sizeof(std::list<unsigned int>));
  for(i = 0; i < maxD; i++) buckets[i] = new std::list<unsigned int>();
  for(i = 0; i < arr->n; i++) {
    buckets[degs[i]-1]->push_front(i);
  }

  unsigned int *correspondance = (unsigned int*) malloc(arr->n*sizeof(unsigned int)); // non trie -> trie
  unsigned int *sortedNodes = (unsigned int*) malloc(arr->n*sizeof(unsigned int)); // trie -> non trie
  int j = 0;
  for(i = maxD - 1; i >= 0; i--) {
    for(unsigned int n: *(buckets[i])) {
      correspondance[n] = j;
      sortedNodes[j] = n;
      j++;
    }
  }
  for(i = 0; i < maxD; i++) delete(buckets[i]);
  free(buckets);

  std::list<unsigned int> **voisins = (std::list<unsigned int>**) malloc(arr->n*sizeof(std::list<unsigned int>*));
  for(i = 0; i < arr->n; i++) {
    unsigned int coi = sortedNodes[i];
    voisins[i] = new std::list<unsigned int>();
    for(j = arr->cd[coi]; j < arr->cd[coi+1]; j++) {
      if(degs[arr->adj[j]] > degs[coi] ||
         (degs[arr->adj[j]] == degs[coi] && arr->adj[j] > coi)) {
        voisins[i]->push_front(correspondance[arr->adj[j]]);
      }
    }
  }

  arr->cd[0] = 0;
  for(i = 1; i <= arr->n; i++) arr->cd[i] = arr->cd[i-1] + voisins[i-1]->size();

  free(arr->adj);
  arr->adj = (unsigned int *)malloc(arr->cd[arr->n]*sizeof(unsigned int));
  j = 0;
  for(i = 0; i < arr->n; i++) {
    for(unsigned int no: *(voisins[i])) {
      arr->adj[j] = no;
      j++;
    }
  }

  free(degs);
  free(sortedNodes); free(correspondance);
  for(i = 0; i < arr->n; i++) delete voisins[i];
  free(voisins);
}

double *computeTriangleDensity(adjArray *arr, int t) {
  double *r = (double *) malloc(arr->n*sizeof(double));
  for(int i = 0; i < arr->n; i++) r[i] = 0;

  for(int l = 0; l < t; l++) {
    printf("Iteration %d\n", l);
    for(int i = 0; i < arr->n; i++) {
      std::set<unsigned int> unei; // Tous les voisins de u
      for(int j = arr->cd[i]; j < arr->cd[i+1]; j++) unei.insert(arr->adj[j]);
      for(int j = arr->cd[i]; j < arr->cd[i+1]; j++) {
        unsigned int nj = arr->adj[j];
        for(int k = arr->cd[nj]; k < arr->cd[nj+1]; k++) {
          unsigned int nk = arr->adj[k];
          if(unei.find(nk) != unei.end()) {
            if(r[i] <= r[nj] && r[i] <= r[nk]) r[i]++;
            else if(r[nj] <= r[i] && r[nj] <= r[nk]) r[nj]++;
            else r[nk]++;
          }
        }
      }
    }
  }

  return r;
}

std::set<unsigned int> *densestPrefix(adjArray *arr, std::vector<unsigned int> ordering) {
  unsigned int* revOrdering = (unsigned int *)malloc(arr->n*sizeof(unsigned int));
  for(int i = 0; i < arr->n; i++) revOrdering[i] = -1;
  for(int i = 0; i < arr->n; i++) {
    assert(revOrdering[ordering[i]] == -1);
    revOrdering[ordering[i]] = i;
  }

  double bestDens = 0;
  int bestN = 0; int bestM = 0;
  std::set<unsigned int> currentSet;
  int currentM = 0;

  for(int i = 0; i < arr->n; i++) {
    unsigned int node = ordering[i];
    currentSet.insert(node);

    std::set<unsigned int> unei; // Tous les voisins de u qu'on observe
    for(int j = arr->cd[node]; j < arr->cd[node+1]; j++) {
      if(revOrdering[arr->adj[j]] >= i) continue;
      unei.insert(arr->adj[j]);
    }
    for(int j = arr->cd[node]; j < arr->cd[node+1]; j++) {
      unsigned int nj = arr->adj[j];
      if(revOrdering[nj] >= i) continue;
      for(int k = arr->cd[nj]; k < arr->cd[nj+1]; k++) {
        unsigned int nk = arr->adj[k];
        if(revOrdering[nk] >= i) continue;
        if(unei.find(nk) != unei.end()) {
          currentM++;
        }
      }
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

  // Calculate number of edges
  int m = 0;
  for(unsigned int node: *densest) {
    for(int j = arr->cd[node]; j < arr->cd[node+1]; j++) {
      int nj = arr->adj[j];
      if(densest->find(nj) != densest->end()) m++;
    }
  }

  printf("Densest prefix avg triangle density: %f\n", bestDens);
  printf("Densest prefix avg edge density: %f\n", (double)m/(double)((double)(bestN*(bestN-1))/(double)2));
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

  sortDescendingDegrees(arr);

  double *dens = computeTriangleDensity(arr, 100);
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
