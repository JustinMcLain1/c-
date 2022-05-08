/* 	Justin McLain
	Z1910087
	CSCI 340- PE1
	
	I certify that this is my own work and where appropriate an extension 
	of the starter code provided for the assignment.
*/
#include "hashmain.h"
#include "htable.h"


// Hashing with separate chaining and indirect sorting

int main() {
  char  c;  // operation code
  Entry e;  // holds input data
  HT    h;  // hash table

  // print program header message
  cout << "\t*** Hashing with separate chaining ***\n\n";

  while (cin >> c) {
    switch (c) {
      case 'A':  // insert record in hash table
	read_rec1(e);
	prnt_rec1(e);
	h.insert(e);
	break;

      case 'S':  // search for record in hash table
	read_rec2(e);
	cout << "search: key " << e.key;
	h.search(e.key);
	break;

      case 'P':  // print all active entries of hash table
	cin.ignore(BUF_SZ, ':');
	cout << "\nhTable_print:\n";
	cout << "entry  key    number    description\n";
	h.hTable_print();
	break;

      default:  // illegal command
	cout << "error: invalid entry\n";
	break;
    }
  }

  // print hash table (in sorted order)
  cout << "\nsort_print:\n   sorted hash table:\n";
  cout << "   key    number    description\n";
  h.pTable_print();

  // print program termination message
  cout << "\n\t*** end of program termination ***\n";

  return 0;
}

// routine to get record from stdin
void read_rec1(Entry& e) {
  char buf1[BUF_SZ + 1];

  cin.ignore(BUF_SZ, ':');
  cin.get(buf1, ID_SZ + 1, ':');
  cin.ignore(BUF_SZ, ':');

  e.key = string(buf1);
  cin >> e.num;
  cin.ignore(BUF_SZ, ':');
  cin.get(buf1, ITEM_SZ + 1, ':');
  cin.ignore(BUF_SZ, ':');
  e.desc = string(buf1);
}

// routine to get record from hash table
void read_rec2(Entry& e) {
  char buf2[BUF_SZ + 1];

  cin.ignore(BUF_SZ, ':');
  cin.get(buf2, ID_SZ + 1, ':');
  cin.ignore(BUF_SZ, ':');
  e.key = string(buf2);
}

// routine to print record from stdin on stdout
void prnt_rec1(const Entry& e) {
  cout << "insert: :" << e.key << ':';
  cout.flags(ios ::right);
  cout << setw(ID_SZ + 1) << e.num << ':';
  cout.flags(ios ::left);
  cout << setw(ITEM_SZ) << e.desc.c_str() << ':';
  cout.flags(ios ::right);
}
