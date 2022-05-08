/* 	Justin McLain
	Z1910087
	CSCI 340- PE1
	
	I certify that this is my own work and where appropriate an extension 
	of the starter code provided for the assignment.
*/
#include "simplegraph.h"

#include <fstream>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

int main(int argc, char** argv) {
  if (argc < 2) {
    cerr << "args: input-file-name\n";
    return 1;
  }

  Graph g(argv[1]);

  g.Print();

  g.Traverse();

  return 0;
}

/* This is the constructor. It reads data in from the input 
file of the graph given in adjacency matrix representation and builds the graph with a adjacency list representation. */
Graph::Graph(const char* filename){

        ifstream input;
        input.open(filename);
        input >> size;
        char temp[100];
        input.getline(temp,1);
        input.getline(temp, 100);
        {
                string line(temp);
                string::iterator end = remove_if(line.begin(), line.end(), static_cast<int(*)(int)> (isspace));
                line.erase(end, line.end());
                for(int i = 0; i < line.size(); i++)
                        labels.push_back('A' + i);
        }

        while(input){
                list<int> values;
                input.getline(temp, 100);
                string line(temp);
                if(line.size() == 0) break;
                line.erase(remove_if(line.begin(),line.end(), static_cast<int(*)(int)>(isspace)), line.end());
                for(int i = 1; i < line.size(); i++)
                {
                        if(line[i] == '1')
                                values.push_back(i-1);
                }
                adj_list.push_back(values);
        }
}
 /* This function prints the adjacency list for the graph. */
void Graph::Print() const
{
        int i = 0;
        cout << "Number of verticies in graph: " << size << endl;
        cout << "\n----------START GRAPH---------" << endl;
        for(list<int> l : adj_list){
                cout << (char) ('A' + i++) << ": ";
                for(int c : l)
                        cout << labels[c] << ", ";
                cout << endl;
        }
        cout << "-----------END GRAPH----------" << endl;
}

int * num;
string s; 
/* This private function is used to traverse a graph in the depth-first traversal/search algorithm */
void Graph::Depthfirst(int v)
{
        num[v]++;
        cout << labels[v] << " ";
        for(int i : adj_list[v])
                if(num[i] == 0){
                        s = s + '(' + labels[v] + ',' + labels[i] + ')';
                        Depthfirst(i);
                }
}
/* This public function is used to traverse a graph and invokes the above Depthfirst method. */
void Graph::Traverse()
{
        s = "";
        cout << "\n--------traverse of graph--------" << endl;
        num = new int[size];
        for(int i = 0 ; i< size; i++)
                num[i] = 0;
        for(int i = 0; i < size; i++)
                if(num[i] == 0)
                        Depthfirst(i);
        cout << endl << s << endl;
        delete num;
        cout << "----------end traverse-----------" << endl;
}
