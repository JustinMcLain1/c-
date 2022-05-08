#ifndef QUESTION5_H
#define QUESTION5_H
#include <vector>
#include <list>

class graph 
{
    private:
        int size;
        std::vector< std::list<int> > adj_list;
        std::vector< char > labels;
        void depth_first( int ) const;
    public:
        graph( const char* filename );
        ~graph();
        int get_size() const;
        void traverse( ) const;
        void print ( ) const;
};

//deconstructor for the graph class
graph::~graph(){}
//returns the size of the graph (number of verticies)
int graph::get_size() const
{
  return size;
}

#endif 
