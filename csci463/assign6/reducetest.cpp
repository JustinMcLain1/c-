#include <iostream>
#include <mutex>
#include <vector>
#include <thread>
#include <getopt.h>
#include <string>
#include <sstream>

using namespace std;


constexpr int rows = 1000;     ///< the number of rows in the work matrix
constexpr int cols = 100;      ///< the number of cols in the work matrix

std::mutex stdout_lock;        ///< for serializing access to stdout
std::mutex counter_lock;       ///< for dynamic balancing only

volatile int counter = rows;   ///< for dynamic balancing only

std::vector <int > tcount;     ///< count of rows summed for each thread
std::vector <uint64_t> sum;   ///< the calculated sum from each thread

int work[rows][cols];          ///< the matrix to be summed



uint64_t sum_row(int row) {

        uint64_t sumTheRow = 0;

        for (int i = 0; i < cols; i++)
        {
            sumTheRow += work[row][i];
        }
        return sumTheRow;
}


void sum_static(int tid, int num_threads)
{

  stdout_lock.lock();
  cout<<"Thread "<< tid <<" Starting" <<endl;
  stdout_lock.unlock();

  for (int i = tid; i < rows; i += num_threads) {
    
            sum[tid] += sum_row(i);
            tcount[tid]++;
    }

  stdout_lock.lock();
  cout<<"Thread "<< tid <<" ending tcount = "<< tcount[tid] <<" sum ="<<sum[tid]<<endl;
  stdout_lock.unlock();

  
}

void sum_dynamic(int tid)
{   

    stdout_lock.lock();
    cout<<"Thread "<< tid <<" Starting" <<endl;
    stdout_lock.unlock();

    counter_lock.lock();
    int next_row = --counter;
    counter_lock.unlock();

    while(next_row >= 0)
    {
            sum[tid] += sum_row(next_row);
            tcount[tid]++;

            counter_lock.lock();
            next_row = --counter;
            counter_lock.unlock();
    }

  stdout_lock.lock();
  cout<<"Thread "<< tid <<" ending tcount = "<< tcount[tid] <<" sum ="<<sum[tid]<<endl;
  stdout_lock.unlock();

}




/**
* @brief the given usage() function is print an error for message displayed by 
*  a program that utilizes a command-line interface for execution or a missing 
*  file name
*
* @note it gives termination at exit(1), so the program will stop.
* 
*******************************************************************************/

static void usage()
{
	cerr << "Usage: 2-dimensional matrix using either static or dynamic load   balancing\n"
	     <<"       –[-d] Use dynamic load-balancing. (By default, use static load balancing.)\n" 
       <<"       –[-t num] Specifies the number of threads to use. (By default, start two threads.) Use: std::thread::hardware_concurrency()" << endl;
	exit(1);
}

int main(int argc, char **argv)
{
  cout<< thread::hardware_concurrency() <<" concurrent thread supported. "<<endl;


    bool dynamic_sum = false;
    int opt;
    unsigned int num_thread = 2;


    while ((opt = getopt(argc, argv, "dt:")) != -1)
    {
        switch (opt)
        {
        case 'd':
        {
            dynamic_sum = true;
        }
        break;
        case 't':
        {
            std::istringstream iss(optarg);
            iss >> num_thread;
            if (thread::hardware_concurrency() < num_thread)
            {
                num_thread = thread::hardware_concurrency();
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

  

  sum.assign(num_thread,0);
  tcount.assign(num_thread,0);

      srand(0x1234);

    for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                work[i][j] = rand();
            }
    }

   vector<thread*> threads;


  if(dynamic_sum)
  {
    for(unsigned int i = 0; i < num_thread; ++i)
    {
        threads.push_back(new thread(sum_dynamic,i));
    }
  }
  else
  {
        for (unsigned int i = 0; i < num_thread; i++)
        {
            threads.push_back(new thread(sum_static, i, num_thread));
        }
  }

  int total_work = 0;
  uint64_t gross_sum = 0;
  
     for (unsigned int i = 0; i < num_thread; i++)
    {  
            threads.at(i)->join();
            delete threads.at(i);
            total_work += tcount.at(i);
            gross_sum += sum.at(i);
    }


  cout<< "main() exiting, total_work = "<< total_work << " gross_sum = "<< gross_sum <<endl;
return 0;
}