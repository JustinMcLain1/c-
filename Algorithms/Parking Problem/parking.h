#include <iostream>
#include <string>
#include <deque>
#include <stack>
#include <fstream>

// parking limit (you can change it)
#define LIMIT 5
  
using namespace std;

typedef struct CAR {
        int id; // assigned ID
        string lp; // plate number
        int moves; // no of moves
        
        CAR(int id, string lp) {
                this->id = id;
                this->lp = lp;
                this->moves = 0;
        }
}CAR;

// process line
void get_input_vals(string &line, char &act, string &lp) {
        act = line[0];
        lp.append(line.substr(2, line.length() - 3));
}
// arrival
void arrival(CAR &car, deque<CAR> &cars) {
        cout << "Car " << car.id << " with license plate " << car.lp << " has arrived.\n";
        if(cars.size() == LIMIT) {
                cout << "But garage is FULL!\n";
        } else {
                cars.push_back(car);
        }
}

// departure
void departure(string &lp, deque<CAR> &cars, stack<CAR> &track) {
        bool found = false;
        while(!cars.empty()) {
                CAR car = cars.front();
                cars.pop_front();
                if(car.lp == lp) {
                        // need to remove
                        cout << "Car " << car.id << " with license " << car.lp << " has departed.\n";
                        cout << "Car was moved " << car.moves << " times in the garage.\n";
                        found = true;
                        break;
                } else {
                        // it will be moved to garage later on
                        track.push(car);
                }
        }
        if(!found) {
                // car not found
                cout << "NO car with license plate: " << lp << " found in garage.\n";
        }
}