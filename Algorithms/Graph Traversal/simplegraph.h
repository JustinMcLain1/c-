#ifndef SIMPLEGRAPH_H
#define SIMPLEGRAPH_H
#include <list>
#include <vector>

class Graph {
 private:
  int                          size;
  std::vector<std::list<int> > adj_list;
  std::vector<char>            labels;
  void                         Depthfirst(int);

 public:
  Graph(const char* filename);
  ~Graph();
  int  Getsize() const;
  void Traverse();
  void Print() const;
};

//deconstructor for the graph class
Graph::~Graph(){}
//returns the size of the graph (number of verticies)
int Graph::Getsize() const
{
  return size;
}
#endif // SIMPLEGRAPH_H
