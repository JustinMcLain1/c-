/********************************************************************
CSCI 480 - Assignment 3 - Spring 2022

Programmer: Justin McLain
ZID  Z1910087	   
Section:   PE1
Date Due:  3/21/2022

Purpose: Simulate CPU scheduling in an OS.
*********************************************************************/

#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <queue>
#include <iomanip>

#include "Process.h"
#include "Assign3.h"

bool finished();
int  countInSystem();
void PStatus();
void Terminate(Process*);


typedef std::priority_queue<Process, std::vector<Process>, SpecialPriorityOprtr> ProcessPriorityQueue;

std::queue<Process> Entry;

ProcessPriorityQueue Ready;
ProcessPriorityQueue Input;
ProcessPriorityQueue Output;

Process* Active = nullptr;
Process* IActive = nullptr;
Process* OActive = nullptr;

Process PHolder;
Process APHolder;
Process IPHolder;
Process OPHolder;


int Timer = 0;
int NoTermProcess = 0;


//main
//returns 0 on success 
//returns -1 on file failed to open

int main(int argc, char **argv)
{

//Input fileVal this opens file
const char* fileVal = argv[1];
	//attempt to open file
	std::ifstream fileIn;
	fileIn.open(fileVal);
	
	//exit if fileIn error
	if (!fileIn) {
		std::cerr << "unable to open input file";
		exit(-1);
	}
	
	//read two lines at once
	char cline1[256];
	char cline2[256];
	
	//nullify the lines 
	memset(cline1, '\0', 256);
	memset(cline2, '\0', 256);
	
	//read loop
	while (fileIn && fileIn.peek() != -1) {
		
        //carriage return w/ newline 
        //beginning indicates might be at end of file
		if (fileIn.peek() == '\n' || fileIn.peek() == '\r'){
			fileIn.ignore();
        }
		else{
			//read two lines
			fileIn.getline(cline1, 256);
			fileIn.getline(cline2, 256);
			
			//Generate a new process from the two read lines and push it onto Entry
			Entry.push(Process(cline1, cline2));
			
			//Null out the two lines
			memset(cline1, '\0', 256);
			memset(cline2, '\0', 256);
		}
	}
	
	//close
	fileIn.close();

	Timer = 0;
	bool checkWaitProcExists = true;
	
	std::cout << "Simulation of Priority Scheduling\n" << std::endl;
	
	//main loop process
	do
	{
		//retrieve waiting process
		while (countInSystem() < IN_USE && checkWaitProcExists) {
			//hold entering process, pop it off entry
			PHolder = Entry.front();
			Entry.pop();
			
			//printing
			std::cout << "Process " 
				 << Entry.front().ProcessID 
				 << " moved from Entry Queue into the Ready Queue at time "
				 << Timer
				 << "\n" << std::endl;
			
			//set default starter time, and start wait time
			PHolder.LastSeen = Timer;
			PHolder.StartTime = Timer;
			
			//push to ready queue
			Ready.push(PHolder);
			
			//make sure to not enter "STOPHERE" if the process hasnt arrived
			checkWaitProcExists = !(Entry.front().ArrivalTime > Timer ||
								   Entry.front().ProcessName == "STOPHERE");
		}
		
		//check active pointer & Ready queue
		if (Active == nullptr && !Ready.empty()){
			//set active pointer and get process from ready to pop
			APHolder = Ready.top();
			Active = &APHolder;
			Ready.pop();
			
			//calculate Total time waiting
			Active->TimeWaiting += (Timer - Active->LastSeen);
			//start CPUTimer
			Active->CPUTimer = (short) Active->History[Active->Sub][1];
		}
		
		//if active process add a clock tick
		if (Active != nullptr){
			//execute tick on clock by location of History[Sub][0]
			Active->ClockTicker();
			
			//is the CPU burst finished?
			if (Active->CPUTimer == 0) {
				//increment burst count
				Active->CPUCount++;
				//increment subscript
				Active->Sub++;
				
				//whats job of next process
				switch(Active->History[Active->Sub][0]){
					case 'I':
						Input.push(*Active);
						break;
					case 'O':
						Output.push(*Active);
						break;
					default:
						Terminate(Active);
				}
				//update LastSeen with time waiting
				Active->LastSeen = Timer;
				//remove process from Ready & clear active pointer
				Active = nullptr;
			}
		}
		else{
            //for idleing
			std::cout << "\nAt time " 
				 << Timer
				 << ", Active is 0, so we have idle time for a while\n"
                 << std::endl;
		}
		
		//get a I process if found
		if (IActive == nullptr && !Input.empty()){
			//get process from input to pop while setting IActive pointer
			IPHolder = Input.top();
			IActive = &IPHolder;
			Input.pop();
			
			//calculate waiting time
			IActive->TimeWaiting += (Timer - IActive->LastSeen);
            //start IOTimer
			IActive->IOTimer = (short) IActive->History[IActive->Sub][1];
		}
		
		if (IActive != nullptr){
			//exec clock tick for history[sub][0]
			IActive->ClockTicker();
			//done with I burst?
			if (IActive->IOTimer == 0){
				//increment burst counter
				IActive->ICount++;
				//increment history sub
				IActive->Sub++;
				//update wait timer
				IActive->LastSeen = Timer;
				//push back to ready
				Ready.push(*IActive);
				//free input again
				IActive = nullptr;
			}
		}
		
		if (OActive == nullptr && !Output.empty()){
			//get process from Output send it to pop and set OActive pointer
			OPHolder = Output.top();
			OActive = &OPHolder;
			Output.pop();
			//calculate the TimeWaiting value
			OActive->TimeWaiting += (Timer - OActive->LastSeen);
			//start countdown for IOTimer
			OActive->IOTimer = (short) OActive->History[OActive->Sub][1];
		}
		
		if (OActive != nullptr){
			//exec clock tick for history[sub][0]
			OActive->ClockTicker();
			//done with O burst?
			if (OActive->IOTimer == 0){
				//increment burst counter
				OActive->OCount++;
				//increment history sub
				OActive->Sub++;
				//update wait timer
				OActive->LastSeen = Timer;
				//push back to ready
				Ready.push(*OActive);
				//free input again
				OActive = nullptr;
			}
		}
		
		
		//output status if timer == multiple of HOW_OFTEN
		if ((Timer % HOW_OFTEN) == 0){
			PStatus();
        }
		Timer++;
		//dont enter "STOPHERE"
		checkWaitProcExists = !(Entry.front().ArrivalTime > Timer ||
							   Entry.front().ProcessName == "STOPHERE");
		
	} while (!finished());
	
	//last output
	std::cout << "\n\nThe run has ended\n"
		 << "The Final Value of the Timer was: "
		 << Timer << std::endl
		 << "Number of terminated Processes = "
		 << NoTermProcess << std::endl
		 << "Final ";
	//last print
	PStatus();
	return 0;
}

//finished
//checks to see if the program is done executing or not
//returns true if there are no more processes left
//or if MAX_TIME has been reached
//otherwise it returns false

bool finished() 
{
	return (Timer == MAX_TIME || 
			(Ready.empty() && 
			 Input.empty() && 
			 Output.empty() && 
			 Active == nullptr && 
			 IActive == nullptr && 
			 OActive == nullptr));
}

//countInSystem
//counts how many processes are in the system
//returns the number of processes in the system

int countInSystem()
{
	int sysCount = (Ready.size() + Input.size() + Output.size());
	if (Active != nullptr){
		sysCount++;
    }
	if (IActive != nullptr){
		sysCount++;
    }
	if (OActive != nullptr){
		sysCount++;
    }
	return sysCount;
}

//PStatus
//Prints the status of pointers and queues
//at the current moment
//void so it returns nothing

void PStatus() 
{
	//hold 
	std::queue<Process> tempQueue;
	ProcessPriorityQueue tempPriorQueue;
	
	std::cout << "Status at time " << Timer << std::endl;
	std::cout << "Active is at " << ((Active == nullptr)? 0 : Active->ProcessID) << std::endl;
	std::cout << "IActive is at " << ((IActive == nullptr)? 0 : IActive->ProcessID) << std::endl;
	std::cout << "OActive is at " << ((OActive == nullptr)? 0 : OActive->ProcessID) << std::endl;
	
	
	std::cout << "Contents of the Entry Queue:" << std::endl;
	
	//print the value of everything in Empty
	while (!Entry.empty()){

		if (Entry.front().ProcessName != "STOPHERE"){
			std::cout << Entry.front().ProcessID << std::endl;
        }
		
		tempQueue.push(Entry.front());
		Entry.pop();
	}
	
	//places values into Entry
	Entry = tempQueue;
	
	//clear out the temporary queue
	while (!tempQueue.empty()){
		tempQueue.pop();
    }
	
	std::cout << "Contents of the Ready Queue:" << std::endl;
	
	//print everything within ready with the value of PID and Priority
	if (Ready.empty()){
		std::cout << "(Empty)";
    }
	else{
		while (!Ready.empty()){
			std::cout << Ready.top().ProcessID <<"\t";
			tempPriorQueue.push(Ready.top());
			Ready.pop();
		}
		
		//places values into Ready
		Ready = tempPriorQueue;
		
		//empty out the temporary priority queue
		while (!tempPriorQueue.empty()){
			tempPriorQueue.pop();
        }
	}
	std::cout << std::endl;
	
	std::cout << "Contents of the Input Queue:" << std::endl;
	
	//print everything within Input with the value of PID and Priority
	if (Input.empty()){
		std::cout << "(Empty)";
    }
	else{
		while (!Input.empty()){
			std::cout << Input.top().ProcessID
				 << "("
				 << Input.top().Priority
				 << ")   ";
			
			tempPriorQueue.push(Input.top());
			Input.pop();
		}
		
		//places values into Input
		Input = tempPriorQueue;
		//empty out the temporary priority queue
		while (!tempPriorQueue.empty()){
			tempPriorQueue.pop();
        }
	}
	std::cout << std::endl;
	
	std::cout << "Contents of the Output Queue:" << std::endl;
	
	//print everything within Output with the value of PID and Priority
	if (Output.empty()){
		std::cout << "(Empty)";
    }
	else{
		while (!Output.empty()){
			std::cout << Output.top().ProcessID
				 << "("
				 << Output.top().Priority
				 << ")   ";
			
			tempPriorQueue.push(Output.top());
			Output.pop();
		}
		
		//place values into Output
		Output = tempPriorQueue;
		//empty out the temporary priority queue
		while (!tempPriorQueue.empty()){
			tempPriorQueue.pop();
        }
	}
	std::cout << "\n" << std::endl; 
}


//Terminate
//Prints out processes values and increments Terminate counter
//has a value of x to point to a process
//void so doesnt return anything

void Terminate(Process* x)
{
	std::cout << "\nA Process has terminated.\n";
	std::cout << std::setw(22) << std::left << "Process ID:";
	std::cout << x->ProcessID << std::endl;
	std::cout << std::setw(22) << std::left << "Name:";
	std::cout << x->ProcessName << std::endl;
	std::cout << std::setw(22) << std::left << "Priority:";
	std::cout << x->Priority << std::endl;
	std::cout << std::setw(22) << std::left << "Started at time:";
	std::cout << x->StartTime << " and ended at time " << Timer << std::endl;
	std::cout << std::setw(22) << std::left << "Total CPU time:";
	std::cout << x->CPUTotal << " in " << x->CPUCount << " CPU Burst(s)" << std::endl;
	std::cout << std::setw(22) << std::left << "Total Input time:";
	std::cout << x->ITotal << " in " << x->ICount << " Input Burst(s)" << std::endl;
	std::cout << std::setw(22) << std::left << "Total Output time:";
	std::cout << x->OTotal << " in " << x->OCount << " Output Burst(s)" << std::endl;
	std::cout << std::setw(22) << std::left << "Time spend waiting:";
	std::cout << x->TimeWaiting << "\n\n";
	
	NoTermProcess++;
}
