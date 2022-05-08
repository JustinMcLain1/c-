/********************************************************************
CSCI 480 - Assignment 3 - Spring 2022

Programmer: Justin McLain
ZID  Z1910087	   
Section:   PE1
Date Due:  3/21/2022

Purpose: Simulate CPU scheduling in an OS.
*********************************************************************/

#include <cstring>
#include <string>
#include "Process.h"
#include "Assign3.h"


//default constructor
//generates black process 
//no parameters
Process::Process() {}

int Process::PIDNum = 101; //generate PID

//generates process from two input lines
//line1 is ProcessName, Priority, ArrivalTime
//line2 is Repeating ARRAY_SIZE
Process::Process(char* line1, char* line2) {
    char * hold = strtok(line1," ");
    ProcessName = hold;

    hold = strtok(nullptr," ");
    ArrivalTime = atoi(hold);

    hold = strtok(line2," "); //fill History w/ values

    for (int i = 0; i < ARRAY_SIZE && hold != nullptr; i++){
        History[i][0] = hold[0];

        hold = strtok(nullptr," ");
        History[i][1] = atoi(hold);

        hold = strtok(nullptr," ");
    }

    //set all defaults to zero and
    //Obtain pid & set pidnum for next object
    Sub = 0;
	CPUTimer = 0;
	IOTimer = 0;
	CPUTotal = 0;
	ITotal = 0;
	OTotal = 0;
	CPUCount = 0;
	ICount = 0;
	OCount = 0;
    ProcessID = PIDNum;
	PIDNum++;
	LastSeen = 0;
	TimeWaiting = 0;
}

// ClockTicker and process
//decrements and increments timer based on history positon
// doesnt have parameters or return anything
void Process::ClockTicker(){
    switch(History[Sub][0])
    {
        case 'C':
            CPUTimer--;
            CPUTotal++;
            break;
        case 'I':
            IOTimer--;
            ITotal++;
            break;
        case 'O':
            IOTimer--;
            OTotal++;
            break;
    }
}