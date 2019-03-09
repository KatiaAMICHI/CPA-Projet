#include <vector>
#include <algorithm>
#include "structs.h"
#include <string.h>

std::map<unsigned int, unsigned int> *reverseMap(std::map<unsigned int, unsigned int> *map) {
  std::map<unsigned int, unsigned int>  *revMap = new std::map<unsigned int, unsigned int>();
  for(std::pair<unsigned int, unsigned int> p: *map) {
    (*revMap)[p.second] = p.first;
  }
  return revMap;
}

void matVectProd(char *fn, std::map<unsigned int, unsigned int> *map, int *degs, int n, double *P) {
  double *B = (double *)malloc(n*sizeof(double));
  for(int i = 0; i < n; i++) B[i] = 0;
  FILE *f = fopen(fn, "r");
  char line[128];
  while(fgets(line, sizeof line, f) != NULL) {
    if(line[0] == '#') continue;
    unsigned int nl = (*map)[atoi(strtok(line, " \t"))];
    unsigned int nr = (*map)[atoi(strtok(NULL, " \t"))];
    B[nr] += P[nl]/(double)degs[nl];
  }
  fclose(f);

  for(int i = 0; i < n; i++) P[i] = B[i];
  free(B);
}

void normalize2(double *P, int n) {
  double norme = 0;
  for(int i = 0; i < n; i++) norme += P[i];
  for(int i = 0; i < n; i++) {
    P[i] += (1-norme)/(double)n;
  }
}

double *pagerank(char *fn, std::map<unsigned int, unsigned int> *map, int *degs, double alpha, int n, int t) {
  double *ranks = (double *)malloc(n*sizeof(double));
  for(int i = 0; i < n; i++) {
    ranks[i] = (double)1/(double)n;
  }

  for(int i = 0; i < t; i++) {
    printf("Itération %d\n", i);
    matVectProd(fn, map, degs, n, ranks);
    for(int j = 0; j < n; j++) {
      ranks[j] = (1 - alpha)*ranks[j] + alpha * ((double)1/(double)n);
    }
    normalize2(ranks, n);
  }

  return ranks;
}

void writeRanks(char *fn, std::vector<std::pair<unsigned int, double>> ranks) {
  FILE *f = fopen(fn, "w");
  for(std::pair<int, double> p: ranks) {
    fprintf(f, "%d %.15g\n", p.first, p.second); //%.17g pour plus de précision d'affichage
  }
  fclose(f);
}

int main(int argc, char **argv) {
  if(argc != 3) {
    printf("usage : ./pagerank.exe <input_file> <output_file>\n");
    return 1;
  }
  char *inName = argv[1];
  char *outName = argv[2];

  unsigned int nbNodes = 0; unsigned int nbEdges = 0;
  std::map<unsigned int, unsigned int> *map = size(inName, &nbNodes, &nbEdges);
  std::map<unsigned int, unsigned int> *revMap = reverseMap(map);

  int *degs = degrees(inName, nbNodes, map);

  double *ranks = pagerank(inName, map, degs, 0.2, nbNodes, 20);
  printf("Calculated ranks\n");

  std::vector<std::pair<unsigned int, double>> rankMap(nbNodes);
  for(int i = 0; i < nbNodes; i++) {
    rankMap[i] = std::pair<unsigned int, double>((*revMap)[i], ranks[i]);
  }

  writeRanks(outName, rankMap);

  // Best and worst pages
  std::sort(rankMap.begin(), rankMap.end(),
            [] (std::pair<int, double> p1, std::pair<int, double> p2) -> int {
              return p1.second > p2.second;
            });

  printf("Top 5:\n");
  for(int i = 0; i < 5; i++) {
    printf("%d\n", rankMap[i].first);
  }

  printf("Flop 5:\n");
  for(int i = 0; i < 5; i++) {
    printf("%d\n", rankMap[nbNodes-(i+1)].first);
  }

  free(ranks);
  delete map; delete revMap;
  return 0;
}
