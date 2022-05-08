
//***************************************************************************
//
//  Justin McLain
//  Z1910087
//  CSCI 463 - PE1
//
//  I certify that this is my own work and where appropriate an extension
//  of the starter code provided for the assignment.
//
//***************************************************************************

#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
#include <getopt.h>
#include <string>
#include <sstream>
using namespace std;

constexpr int rows = 1000;     /// < the number of rows in the work matrix
constexpr int columns = 100;   /// < the number of cols in the work matrix

std::mutex stdout_lock;        /// < for serializing access to stdout

std::mutex counter_lock;       /// < for dynamic balancing only
volatile int counter = rows;   /// < for dynamic balancing only

std::vector<int> tcount;       /// < count of rows summed for each thread
std::vector<uint64_t> sum;     /// < the calculated sum from each thread

int work[rows][columns];       /// < the matrix to be summed

void sum_static(int tid, int num_threads);
void sum_dynamic(int tid);

static void usage()
{
    cerr << "Usage : reduce [-d] [-t number of threads to use]" << std::endl;
    cerr << "-d use dyanmic load balancing (static is default)" << std::endl;
    cerr << "-t specify number of threads to use (default is 2 threads)" << std::endl;

    exit(1);
}

int sumTotRows();
int nextRows();
uint64_t sum_row(int rowCount);
uint64_t grossSumCalc();

/**
 * @brief main to determine threads based on input
 * for both -d (dynamic load balancing)
 * and -t num for number of threads to use
 * @param argc determines if input value 
 * @param argv determines the switch case based on value input 
 * @return total output for threads sum tcount and total work and 
 * gross sum and outputs usage statement if incorrect
 ********************************************************************/
int main(int argc, char **argv)
{

    bool dynamFlag = false;       ///flag case for dynamic value
    int opt;                      /// int value for getopt input
    unsigned int num_threads = 2; /// default number of threads

    unsigned int concur_threads = thread::hardware_concurrency(); //determines number of threads with -t

    while ((opt = getopt(argc, argv, "dt:")) != -1)
    {
        switch (opt)
        {
        case 'd':
        {
            dynamFlag = true;
        }
        break;
        case 't':
        {
            std::istringstream iss(optarg);
            iss >> num_threads;
            if (concur_threads < num_threads)
            {
                num_threads = concur_threads;
            }
            break;
        }
        default:
        {
            usage();
        }
        break;
        }
    }
    if (optind < argc)
    {
        usage();
    }


    sum.assign(num_threads, 0);
    tcount.assign(num_threads, 0);

    std::cout << to_string(concur_threads) + " concurrent threads supported." << std::endl; 

    srand(0x1234);
    for (int i = 0; i < rows; i++) {
            for (int j = 0; j < columns; j++) {
                work[i][j] = rand();
            }
    }

    vector<thread*> TheThreads;

    if (dynamFlag)
    {

        for (unsigned int i = 0; i < num_threads; i++)
        {
            TheThreads.push_back(new thread(sum_dynamic, i));
        }
    }
    else
    {
        for (unsigned int i = 0; i < num_threads; i++)
        {
            TheThreads.push_back(new thread(sum_static, i, num_threads)); //add to vector thread
        }
    }

  int totalWorking = 0;
  uint64_t grossSum = 0;
  
     for (unsigned int i = 0; i < num_threads; i++)
    {  
            TheThreads.at(i)->join();
            delete TheThreads.at(i);
            totalWorking += tcount.at(i);
            grossSum += sum.at(i);
    }


  cout<< "main() exiting, total_work = "<< sumTotRows() << " gross_sum = "<< grossSumCalc() << std::endl;
return 0;
}

/**
 * @brief sums the rows to be reported to output
 * 
 * @param rowCount compares to value i which add every
 * time its less than columns
 * @return sumTheRow which reports the value of total rows
 ********************************************************************/
uint64_t sum_row(int rowCount) {

        uint64_t sumTheRow = 0;

        for (int i = 0; i < columns; i++)
        {
            sumTheRow += work[rowCount][i];
        }
        return sumTheRow;
}

/**
 * @brief determines which rows will be proccessed by each thread
 * 
 * @param tid determines the first row of each thread then advances
 * @param num_threads determines the next row to process
 ********************************************************************/

void sum_static(int tid, int num_threads)
{

  stdout_lock.lock();
  cout<<"Thread "<< tid <<" Starting" <<endl;
  stdout_lock.unlock();

  for (int i = tid; i < rows; i += num_threads) {
    
            sum[tid] += sum_row(i);
            tcount[tid]++;
    }

  stdout_lock.lock(); //got rid of helper function for output
  cout<<"Thread "<< tid <<" ending tcount = "<< tcount[tid] <<" sum ="<<sum[tid]<<endl;
  stdout_lock.unlock();

  
}

/**
 * @brief sums the rows of a matrix using dynamic load balancing
 * to determine which rows will be processed by each thread
 * @note each thread must use a mutex lock to access the global and
 * volatile counter variable
 * @param tid the thread ID to determine the first row of each thread
 ********************************************************************/
void sum_dynamic(int tid)
{   

    stdout_lock.lock();   ///balancing
    cout<<"Thread "<< tid <<" Starting" <<endl;
    stdout_lock.unlock();

    counter_lock.lock();     ///counter lock 
    int nextValRow = --counter; /// got rid of nextRow function only 
    counter_lock.unlock();    /// only called nextRow once found unncessesary

    while(nextValRow >= 0)
    {
            sum[tid] += sum_row(nextValRow);
            tcount[tid]++;

            counter_lock.lock();
            nextValRow = --counter;
            counter_lock.unlock();
    }

  stdout_lock.lock();
  cout<<"Thread "<< tid <<" ending tcount = "<< tcount[tid] <<" sum ="<<sum[tid]<<endl;
  stdout_lock.unlock();

}

/**
 * @brief calculates the total_work value
 * 
 * @return totals which is the value of tcount 
 ********************************************************************/
int sumTotRows() {

    int totals = 0;
    for (long unsigned int i = 0; i < tcount.size(); i++){
            totals += tcount[i];
    }
    return totals;
}

/**
 * @brief calculates the value of gross sum by iterating through 
 * sum.size and adding to i reporting back to grossVal
 * @return grossVal in uint64_t 
 ********************************************************************/
uint64_t grossSumCalc(){
    uint64_t grossVal = 0;
    for (long unsigned int i = 0; i < sum.size(); i++)
    {
         grossVal += sum[i];
    }
    return grossVal;
}
