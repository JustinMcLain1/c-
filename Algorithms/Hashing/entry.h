#ifndef H_ENTRY
#define H_ENTRY
#include <iomanip>
#include <iostream>
#include <string>
using namespace std;

#define ID_SZ   3   // size of key
#define ITEM_SZ 24  // max size for item description
#define TBL_SZ  31  // default size for hash table
#define OPEN "---" //defines free spot in hash table

// entry in hash table

struct Entry {
  string   key;   // key
  string   desc;  // description
  unsigned num;   // no of copies

  // constructor
  Entry(const string& k = "", 
        const string& d = "", 
        const unsigned& n = 0) : key(k), desc(d), num(n) {}
};


#endif
