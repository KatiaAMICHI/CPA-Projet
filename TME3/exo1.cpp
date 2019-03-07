#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "map"

// Ex1
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

// Ex3
int *degrees(char *fn, unsigned int n, std::map<unsigned int, unsigned int> *m) {
  int *degs = (int*) malloc(sizeof(int)*n);
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

  char *outputName = (char*) malloc(strlen(fn)+9); strcpy(outputName, fn);
  strcat(outputName, ".degrees");
  f = fopen(outputName, "w");
  for(std::pair<unsigned int, unsigned int> p : *m) fprintf(f, "%d %d\n", std::get<0>(p), degs[(*m)[std::get<0>(p)]]);
  fclose(f);

  free(outputName);
  return degs;
}

// Ex4
unsigned long quantity(char *fn, int *degs, std::map<unsigned int, unsigned int> *m) {
  unsigned long q = 0;
  FILE *f = fopen(fn, "r");
  char line[128];
  while(fgets(line, sizeof line, f) != NULL) {
    if(line[0] == '#') continue;
    unsigned int nl = atoi(strtok(line, " \t"));
    unsigned int nr = atoi(strtok(NULL, " \t"));
    q += degs[(*m)[nl]] * degs[(*m)[nr]];
  }
  fclose(f);
  return q;
}

// Ex5
void degreesDist(char *fn, int *degs, unsigned int n) {
  int maxDeg = 0;
  unsigned int i;
  for(i = 0; i < n; i++) {
    if(degs[i] > maxDeg) maxDeg = degs[i];
  }

  int *degsDist = (int*)malloc(sizeof(int)*(maxDeg+1));
  for(i = 0; i < maxDeg+1; i++) degsDist[i] = 0;
  for(i = 0; i < n; i++) degsDist[degs[i]]++;

  char *outputName = (char*)malloc(strlen(fn)+14); strcpy(outputName, fn);
  strcat(outputName, ".degrees_dist");
  FILE *f = fopen(outputName, "w");
  for(i = 0; i < maxDeg+1; i++) fprintf(f, "%d %d\n", i, degsDist[i]);
  fclose(f);

  free(outputName);
  free(degsDist);
}

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("usage : ./exo1 <input_file>\n");
    return 1;
  }
  char *inputName = argv[1];

  // Ex2
  long startTime = clock();
  unsigned int nbNodes = 0; unsigned int nbEdges = 0;
  std::map<unsigned int, unsigned int> *m = size(inputName, &nbNodes, &nbEdges);
  printf("Nb de noeuds: %d, Nb d'aretes: %d\n", nbNodes, nbEdges);

  // Ex3
  int *degs = degrees(inputName, nbNodes, m);

  // Ex4
  printf("Qg = %ld\n", quantity(inputName, degs, m));
  printf("Execution time: %ldms\n", (1000*(clock()-startTime))/CLOCKS_PER_SEC);

  // Ex5
  degreesDist(inputName, degs, nbNodes);

  free(degs);
  delete m;
  return 0;
}
