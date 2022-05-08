/********************************************************************
CSCI 480 - Assignment 3 - Spring 2022

Programmer: Justin McLain
ZID  Z1910087	   
Section:   PE1
Date Due:  3/21/2022

Purpose: Simulate CPU scheduling in an OS.
*********************************************************************/

#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "Assign3.h"

struct Process;
struct SpecialPriorityOprtr;

struct Process
{
    /* use to represent logical processes
    that deal with CPU I/O bursts
    and statistics 
     */

    Process(char*,char*);
    Process();
    void ClockTicker();
    
    std::string ProcessName;
    int Priority;
	int ProcessID;
	int ArrivalTime;
	int StartTime;
    static int PIDNum;
    char History[ARRAY_SIZE][2];
    short int Sub;
    short int CPUTimer;
    short int IOTimer;
    short int CPUTotal;
    short int ITotal;
    short int OTotal;
    short int CPUCount;
    short int ICount;
    short int OCount;
    int LastSeen;
    int TimeWaiting;
};

// used to sort operations 
struct SpecialPriorityOprtr
{
    bool operator()(const Process& lhs, const Process& rhs) const{
        if(lhs.Priority == rhs.Priority){
            return (lhs.LastSeen) > (rhs.LastSeen);
        }
        else {
            return lhs.Priority < rhs.Priority;
        }
            
    }
};

#endif