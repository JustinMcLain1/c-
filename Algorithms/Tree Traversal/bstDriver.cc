/* 	Justin McLain
	Z1910087
	CSCI 340- PE1
	
	I certify that this is my own work and where appropriate an extension 
	of the starter code provided for the assignment.
*/
#include <iostream>
#include "bstDriver.h"
#include "binarysearchtree.h"


using namespace std;
// This program generates bunch of rand ints in given range
// and stores them in vector, and it also inserts them in
// bin search tree. Then, removes all leaves in tree and
// repeat this process until tree becomes empty.

int main() {
  vector<int>   v(N);  // holds rand ints
  BinarySearchTree<int> t;     // bin search tree ( BST )

  // generate rand ints
  generate(v.begin(), v.end(), RND());

  // print contents of vector
  sz = v.size();
  COUT_SZ;
  for_each(v.begin(), v.end(), print<int>);
  cout << endl;

  // insert ints in vector into BST
  for (unsigned i = 0; i < v.size(); i++) t.Insert(v[i]);

  // remove leaves of BST until it becomes empty
  bool flag = true;  // to check if BST empty
  while (flag) {
    // print contents of BST
    sz = t.getSize();
    COUT_SZ;
    t.Inorder(print<int>);
    cout << endl;

    // remove all leaves of BST
    flag = false;
    for (unsigned i = 0; i < v.size(); i++)
      if (t.Remove(v[i])) flag = true;
  }

  return 0;
}
