// https://gist.github.com/truncs/1810804
// (but slightly modified to fit our purposes)

#include <iostream>
#include <vector>
#include "heap.h"

using namespace std;

Heap::Heap(int n, int *compValue): compValue(compValue) {
  place = (unsigned int *) malloc(n * sizeof(unsigned int));
}

int Heap::getSize(){
  return list.size();
}

void Heap::swap(int child, int parent) {
  unsigned int temp; unsigned int tempPlace;
  temp = list[child]; tempPlace = place[list[child]];
  place[list[child]] = place[list[parent]]; list[child] = list[parent];
  place[list[parent]] = tempPlace; list[parent] = temp;
}

int Heap::getParent(int child) {
  if (child % 2 == 0)
    return (child /2 ) -1;
  else
    return child/2;

}

int Heap::getLeftChild(int parent){
  return 2*parent +1;
}

int Heap::getRightChild(int parent){
  return 2 * parent + 2;
}

void Heap::insert(unsigned int value) {
  place[value] = list.size();
  list.push_back(value);
  bubbleUp();
}

void Heap::bubbleUp() {
  int child = list.size() - 1;
  int parent = getParent(child);

  while (compValue[list[child]] < compValue[list[parent]] && child >=0 && parent >= 0) {
    swap(child, parent);
    child = parent;
    parent = getParent(child);
  }
}


unsigned int Heap::remove() {
  int child = list.size()  - 1;
  place[list[child]] = -1;
  swap(child, 0);

  unsigned int value = list.back();
  list.pop_back();

  bubbleDown();

  return value;
}


void Heap::bubbleDown() {
  int parent = 0;

  while (1) {
    int left = getLeftChild(parent);
    int right = getRightChild(parent);
    int length = list.size();
    int largest = parent;

    if (left < length && compValue[list[left]] < compValue[list[largest]])
      largest = left;

    if (right < length && compValue[list[right]] < compValue[list[largest]])
      largest = right;

    if (largest != parent) {
      swap(largest, parent);
      parent = largest;
    }
    else
      break;
  }
}

void Heap::percolateUp(unsigned int elem) {
  int index = place[elem];
  if(index == -1) return;
  int parent = getParent(index);

  while (compValue[list[index]] < compValue[list[parent]] && index >=0 && parent >= 0) {
    swap(index, parent);
    index = parent;
    parent = getParent(index);
  }
}
