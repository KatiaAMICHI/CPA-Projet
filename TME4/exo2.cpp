#include "stdio.h"
#include <vector>
#include <map>
#include <numeric>
#include <algorithm>
#include <ctime>
#include "structs.h"

std::map<unsigned int, unsigned int> *reverseMap(std::map<unsigned int, unsigned int> *map) {
  std::map<unsigned int, unsigned int>  *revMap = new std::map<unsigned int, unsigned int>();
  for(std::pair<unsigned int, unsigned int> p: *map) {
    (*revMap)[p.second] = p.first;
  }
  return revMap;
}

int *labelsProp(adjArray *arr) {
  int *labels = (int *)malloc(arr->n*sizeof(int));
  int i;
  for(i = 0; i < arr->n; i++) {
    labels[i] = i;
  }

  std::srand ( unsigned ( std::time(0) ) );
  bool hasChanged = true;
  while(hasChanged) {
    hasChanged = false;
    std::vector<unsigned int> perm(arr->n);
    std::iota(perm.begin(), perm.end(), 0);
    std::random_shuffle(perm.begin(), perm.end(), [] (int i) { return std::rand()%i; });
    for(unsigned int no: perm) {
      std::map<int, int> adjLabs;
      int j;
      for(j = arr->cd[no]; j < arr->cd[no+1]; j++) {
        adjLabs[labels[arr->adj[j]]]+=1;
      }

      int maxL = labels[no]; int maxC = 0;
      for(std::pair<int, int> p: adjLabs) {
        if(p.second > maxC) {
          maxL = p.first; maxC = p.second;
        }
      }
      if(maxL != labels[no]) {
        hasChanged = true;
        labels[no] = maxL;
      }
    }
  }
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

std::map<int, int> *sizesOfCommunities(int *labels, int n) {
  std::map<int, int> *sizes = new std::map<int, int>();
  int i;
  for(i = 0; i < n; i++) {
    (*sizes)[labels[i]]+=1;
  }
  return sizes;
}

std::map<int, int> *distribution(std::map<int, int> *map) {
  std::map<int, int> *dist = new std::map<int, int>();
  for(std::pair<int, int> p: *map) {
    (*dist)[p.second]+=1;
  }
  return dist;
}

void writeSizesDist(char *fn, std::map<int, int> *sizes) {
  FILE *f = fopen(fn, "w");
  for(std::pair<int, int> p: *sizes) {
    fprintf(f, "%d %d\n", p.first, p.second);
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
  int *labels = labelsProp(arr);
  printf("Calculated labels\n");
  writeLabels(outName, labels, revMap, nbNodes);
  printf("Written labels\n");

  // Get the size of communities
  // std::map<int, int> *sizes = sizesOfCommunities(labels, nbNodes);
  // std::map<int, int> *sizesDist = distribution(sizes);
  // writeSizesDist(outName, sizesDist);

  //Get the dist of the number of coms
  // std::map<int, int> *nbComs = new std::map<int, int>();
  // for(int i = 0; i < 100; i++) {
  //   int *labels = labelsProp(arr);
  //   std::map<int, int> *sizes = sizesOfCommunities(labels, nbNodes);
  //   (*nbComs)[sizes->size()]+=1;
  //   free(labels); delete sizes;
  //   printf("Iteration %d\n", i);
  // }
  // writeSizesDist(outName, nbComs);

  freeAdjArray(arr); free(labels);
  // delete sizes; delete sizesDist;
  delete map; delete revMap;
  return 0;
}
