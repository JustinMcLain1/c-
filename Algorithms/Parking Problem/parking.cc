/* 	Justin McLain
	Z1910087
	CSCI 340- PE1
	
	I certify that this is my own work and where appropriate an extension 
	of the starter code provided for the assignment.
*/

#include <iostream>
#include <deque>
#include <stack>
#include <fstream>
#include "parking.h"
using namespace std;

int main() {



   // read lines one by one and process it
        ifstream input;
        input.open("parking.in");
        
        deque<CAR> cars; // track cars in queue
        stack<CAR> track; // track temporary removed cars
        
        string line;
        int id = 1;
        while(input) {
                getline(input, line);
                char act;
                string lp;
                get_input_vals(line, act, lp);
                
                if(act == 'A') {
                        // arrival
                        CAR car(id++, lp);
                        car.moves++; // track moves
                        arrival(car, cars);
                } else if(act == 'D') {
                        // departure
                        departure(lp, cars, track);
                        
                        // push back to garage again from front
                        while(!track.empty()) {
                                CAR car = track.top();
                                car.moves++;
                                cars.push_front(car);
                                track.pop();
                        }
                } else {
                        // invalid action
                        cout << "Invalid action: " << act << "\n";
                }
        }
        
        input.close();
        
        return 0;
}

