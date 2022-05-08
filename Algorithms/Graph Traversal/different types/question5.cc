#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include "question5.h"
using namespace std;


int main(int argc, char** argv) 
{
    if ( argc < 2 ) 
	{
        cerr << "args: input-file-name\n";
        return 1;
    }    
    graph g(argv[1]);
    g.print();    
    g.traverse();
    return 0;
}

graph::graph(const char* filename){

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

int * num;
string s; 
void graph::depth_first(int v) const
{
        num[v]++;
        cout << labels[v] << " ";
        for(int i : adj_list[v])
                if(num[i] == 0){
                        s = s + '(' + labels[v] + ',' + labels[i] + ')';
                        depth_first(i);
                }
}

void graph::traverse() const
{
        s = "";
        cout << "\n--------traverse of graph--------" << endl;
        num = new int[size];
        for(int i = 0 ; i< size; i++)
                num[i] = 0;
        for(int i = 0; i < size; i++)
                if(num[i] == 0)
                        depth_first(i);
        cout << endl << s << endl;
        delete num;
        cout << "----------end traverse-----------" << endl;
}

void graph::print() const
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