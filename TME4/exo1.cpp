#include "stdio.h"
#include <vector>
#include "structs.h"

#define N 400

edgeList *generateGraph(double p, double q) {
  edgeList *l = (edgeList *)malloc(sizeof(edgeList));
  std::vector<edge> vect;
  int i,j;
  for(i = 0; i < N; i++) {
    for(j = i+1; j < N; j++) {
      if(i/100 == j/100) { // Meme cluster
        if((double)rand()/(double)RAND_MAX < p) {
          edge e; e.u = i; e.v = j;
          vect.push_back(e);
        }
      } else {
        if((double)rand()/(double)RAND_MAX < q) {
          edge e; e.u = i; e.v = j;
          vect.push_back(e);
        }
      }
    }
  }
  l->m = vect.size();
  l->list = (edge *)malloc(l->m*sizeof(edgeList));
  for(i = 0; i < l->m; i++) {
    l->list[i] = vect[i];
  }
  return l;
}

void writeGraph(edgeList *edges, char *fn) {
  FILE *f = fopen(fn, "w");
  int i;
  for(i = 0; i < edges->m; i++) {
    fprintf(f, "%d %d\n", edges->list[i].u, edges->list[i].v);
  }
  fclose(f);
}

int main(int argc, char **argv) {
  if(argc != 2) {
    printf("usage : ./exo1.exe <output_file>\n");
    return 1;
  }
  char *outName = argv[1];

  //edgeList *edges = generateGraph(0.01,0.01);
  //edgeList *edges = generateGraph(0.01,0.001);
  edgeList *edges = generateGraph(0.5,0.001);
  writeGraph(edges, outName);
  freeEdgeList(edges);
  return 0;
}
