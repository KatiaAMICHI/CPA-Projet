#include <list>
#include <queue>
#include "algs.h"

void sortDescendingDegrees(adjArray *arr, unsigned int n) {
  int *degs = (int *)malloc(n*sizeof(int));
  int i;
  for(i = 0; i < n; i++) degs[i] = arr->cd[i+1]-arr->cd[i];

  int maxD = 0;
  for(i = 0; i < n; i++) if(degs[i] > maxD) maxD = degs[i];
  std::list<unsigned int> **buckets = (std::list<unsigned int>**) malloc(maxD*sizeof(std::list<unsigned int>));
  for(i = 0; i < maxD; i++) buckets[i] = new std::list<unsigned int>();
  for(i = 0; i < n; i++) {
    buckets[degs[i]-1]->push_front(i);
  }

  unsigned int *correspondance = (unsigned int*) malloc(n*sizeof(unsigned int)); // non trie -> trie
  unsigned int *sortedNodes = (unsigned int*) malloc(n*sizeof(unsigned int)); // trie -> non trie
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

  std::list<unsigned int> **voisins = (std::list<unsigned int>**) malloc(n*sizeof(std::list<unsigned int>*));
  for(i = 0; i < n; i++) {
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
  for(i = 1; i <= n; i++) arr->cd[i] = arr->cd[i-1] + voisins[i-1]->size();

  free(arr->adj);
  arr->adj = (unsigned int *)malloc(arr->cd[n]*sizeof(unsigned int));
  j = 0;
  for(i = 0; i < n; i++) {
    for(unsigned int no: *(voisins[i])) {
      arr->adj[j] = no;
      j++;
    }
  }

  free(degs);
  free(sortedNodes); free(correspondance);
  for(i = 0; i < n; i++) delete voisins[i];
  free(voisins);
}

int *bfs(adjArray *arr, unsigned int s, unsigned int n) {
  int *marks = (int *)malloc(n * sizeof(int));
  int i;
  for(i = 0; i < n; i++) marks[i] = -1;

  std::queue<unsigned int> fifo;
  fifo.push(s);
  marks[s] = 0;
  while(!fifo.empty()) {
    unsigned int u = fifo.front(); fifo.pop();
    unsigned int j;
    for(j = arr->cd[u]; j < arr->cd[u+1]; j++) {
      int v = arr->adj[j];
      if(marks[v] == -1) {
        fifo.push(v);
        marks[v] = marks[u]+1;
      }
    }
  }
  return marks;
}
