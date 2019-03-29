// https://gist.github.com/truncs/1810804
// (but slightly modified to fit our purposes)

#include <vector>

class Heap {
  std::vector<unsigned int> list;
  unsigned int *place;
  int *compValue;

  void bubbleUp();
  void bubbleDown();
  void swap(int child, int parent);
  int getLeftChild(int parent);
  int getRightChild(int parent);
  int getParent(int child);
public:
  Heap(int n, int *compValue);
  void insert(unsigned int);
  unsigned int remove();
  int getSize();
  void percolateUp(unsigned int elem);
};
