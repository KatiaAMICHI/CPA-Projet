#include "stdio.h"
#include <vector>
#include <map>
#include <set>
#include <numeric>
#include <algorithm>
#include <queue>
#include <ctime>
#include <limits.h>
#include "structs.h"

std::map<unsigned int, unsigned int> *reverseMap(std::map<unsigned int, unsigned int> *map) {
  std::map<unsigned int, unsigned int>  *revMap = new std::map<unsigned int, unsigned int>();
  for(std::pair<unsigned int, unsigned int> p: *map) {
    (*revMap)[p.second] = p.first;
  }
  return revMap;
}

void community_bfs(adjArray *arr, int *marks, unsigned int s, unsigned int n) {
  int i;
  std::queue<unsigned int> fifo;
  fifo.push(s);
  marks[s] = s;
  while(!fifo.empty()) {
    unsigned int u = fifo.front(); fifo.pop();
    unsigned int j;
    for(j = arr->cd[u]; j < arr->cd[u+1]; j++) {
      int v = arr->adj[j];
      if(v == -1) continue;
      if(marks[v] == -1) {
        fifo.push(v);
        marks[v] = s;
      }
    }
  }
}

int *divide(adjArray *arr) {
  // Strength score of a link: number of common neighbours
  int *strength = (int*)malloc(arr->cd[arr->n]*sizeof(int));
  for(int i = 0; i < arr->cd[arr->n]; i++) strength[i] = 0;
  for(int i = 0; i < arr->n; i++) {
    std::set<unsigned int> voisinsI;
    for(int j = arr->cd[i]; j < arr->cd[i+1]; j++) voisinsI.insert(arr->adj[j]);
    for(int j = arr->cd[i]; j < arr->cd[i+1]; j++) {
      unsigned int nj = arr->adj[j];
      for(int k = arr->cd[nj]; k < arr->cd[nj+1]; k++) {
        if(voisinsI.find(arr->adj[k]) != voisinsI.end()) strength[j]++;
      }
    }
  }

  int maxStrength = 0;
  std::vector<std::pair<int, int>> strengthS(arr->cd[arr->n]);
  for(int i = 0; i < arr->cd[arr->n]; i++) {
    strengthS[i].first = i;
    strengthS[i].second = strength[i];
    if(strength[i] > maxStrength) maxStrength = strength[i];
  }

  std::random_shuffle(strengthS.begin(), strengthS.end(), [] (int i) { return std::rand()%i; });

  std::sort(strengthS.begin(), strengthS.end(),
            [] (std::pair<int, int> p1, std::pair<int, int> p2) -> int {
              return p1.second < p2.second;
            });

  // Update them
  for(int i = 0; i < arr->cd[arr->n]; i++) {
    if(strengthS[i].second > maxStrength/10) break; // We can tweak that
    arr->adj[strengthS[i].first] = -1;
  }

  // Apply labels
  int *labels = (int *)malloc(arr->n*sizeof(int));
  for(int i = 0; i < arr->n; i++) {
    labels[i] = -1;
  }

  int nonLabeled = 0;
  while(nonLabeled != -1) {
    community_bfs(arr, labels, nonLabeled, arr->n);

    nonLabeled = -1;
    for(int i = 0; i < arr->n; i++) {
      if(labels[i] == -1) nonLabeled = i;
    }
  }

  free(strength);
  return labels;
}

void writeLabels(char *fn, int *labels, std::map<unsigned int, unsigned int> *revMap, int n) {
  FILE *f = fopen(fn, "w");
  int i;
  for(i = 0; i < n; i++) {
    fprintf(f, "%d %d\n", (*revMap)[i], labels[i]);
  }
  fclose(f);
}

int main(int argc, char **argv) {
  if(argc != 3) {
    printf("usage : ./exo2.exe <input_file> <output_file>\n");
    return 1;
  }
  char *inName = argv[1];
  char *outName = argv[2];

  unsigned int nbNodes = 0; unsigned int nbEdges = 0;
  std::map<unsigned int, unsigned int> *map = size(inName, &nbNodes, &nbEdges);
  std::map<unsigned int, unsigned int> *revMap = reverseMap(map);

  adjArray *arr = loadAsAdjArray(inName, nbNodes, nbEdges, map);
  printf("Loaded graph\n");

  // Get the different labels
  int *labels = divide(arr);
  printf("Calculated labels\n");
  writeLabels(outName, labels, revMap, nbNodes);
  printf("Written labels\n");

  freeAdjArray(arr); free(labels);
  delete map; delete revMap;
  return 0;
}
