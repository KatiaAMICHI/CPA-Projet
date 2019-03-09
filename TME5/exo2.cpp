#include "structs.h"
#include <string.h>

std::map<unsigned int, unsigned int> *reverseMap(std::map<unsigned int, unsigned int> *map) {
  std::map<unsigned int, unsigned int>  *revMap = new std::map<unsigned int, unsigned int>();
  for(std::pair<unsigned int, unsigned int> p: *map) {
    (*revMap)[p.second] = p.first;
  }
  return revMap;
}

void degrees(char *fn, unsigned int n, std::map<unsigned int, unsigned int> *m, int *inDegs, int *outDegs) {
  unsigned int i;
  for(i = 0; i < n; i++) {
    inDegs[i] = 0;
    outDegs[i] = 0;
  }
  FILE *f = fopen(fn, "r");
  char line[128];
  while(fgets(line, sizeof line, f) != NULL) {
    if(line[0] == '#') continue;
    unsigned int nl = atoi(strtok(line, " \t"));
    unsigned int nr = atoi(strtok(NULL, " \t"));
    outDegs[(*m)[nl]]++; inDegs[(*m)[nr]]++;
  }
  fclose(f);
}

void writeDegrees(char *fn, int n, std::map<unsigned int, unsigned int> *m, int *degs) {
  FILE *f = fopen(fn, "w");
  for(int i = 0; i < n; i++) {
    fprintf(f, "%d %d\n", (*m)[i], degs[i]); //%.17g pour plus de précision d'affichage
  }
  fclose(f);
}

int main(int argc, char **argv) {
  if(argc != 4) {
    printf("usage : ./pagerank.exe <input_file> <in-degs_file> <out-degs_file>\n");
    return 1;
  }
  char *inName = argv[1];
  char *inDegName = argv[2];
  char *outDegName = argv[3];

  unsigned int nbNodes = 0; unsigned int nbEdges = 0;
  std::map<unsigned int, unsigned int> *map = size(inName, &nbNodes, &nbEdges);
  std::map<unsigned int, unsigned int> *revMap = reverseMap(map);

  int *inDegs = (int *)malloc(nbNodes*(sizeof(int)));
  int *outDegs = (int *)malloc(nbNodes*(sizeof(int)));
  degrees(inName, nbNodes, map, inDegs, outDegs);

  printf("Calculated degrees\n");

  writeDegrees(inDegName, nbNodes, revMap, inDegs);
  writeDegrees(outDegName, nbNodes, revMap, outDegs);

  free(inDegs); free(outDegs);
  delete map; delete revMap;
  return 0;
}
