#include "htable.h"

#define N1 10 // num of decimal digits
#define N2 26 // num of upper-case letters

// Hash function, you may NOT modify this function
int HT::hash(const string &s)
{
  int n = (N2 * N1) * (s[0] - 'A') + N1 * (s[1] - 'A') + (s[2] - '0');

  return n % hsize;
}

// You will complete the code below ...
// dynanically increases size of hash table after new entry
HT::HT(const unsigned &hs)
{
  hTable.resize(hs);
  pTable.resize(hs);
  psize = 0;
  hsize = TBL_SZ;
  for (unsigned int i = 0; i != hs; i++)
  {
    hTable[i].front().key = OPEN;
  }
}

/* destructor of the hash table first frees the memory for 
all list containers and then frees the memory for the vector containers for the hash table and the pointer table. */
HT::~HT()
{
  //releases the hTable resources
  hTable.clear();
  pTable.clear();
}

/* his public function inserts the record of the item e:(key, desc, num) in the hash table.*/
void HT::insert(const Entry &e)
{
  int index = hash(e.key);

  unsigned int i;

  for (i = 0; i < hsize; i++)
  {
    if (hTable[(index + i) % hsize].front().key == OPEN)
    {
      // use open addressing: linear probing for collision resolution
      cout << " entry = " << (index + i) % hsize << endl;
      hTable[(index + i) % hsize].front() = e;
      pTable[psize] = &hTable[(index + i) % hsize].front();
      psize++;
      break;
    }
    if (hTable[(index + i) % hsize].front().key == e.key)
    {
      cout << " not inserted - duplicate key!!!" << endl;
      break;
    }
  }
}

//searches the hash table for record with key using find if
void HT::search(const string &s)
{
  int index = hash(s);

  unsigned int i;
  if (find_if(hTable.begin(), hTable.end(), index) != hTable.end())
  {
    for (i = 0; i < hsize; i++)
    {
      if (hTable[(index + i) % hsize].front().key == s)
      {
        cout << " ==> number: " << setw(4) << hTable[(index + i) % hsize].front().num << " - item: " << hTable[(index + i) % hsize].front().desc << endl;
        break;
      }
    }
    if (hTable[(index + i) % hsize].front().key != s)
    {
      cout << " not in table!!" << endl;
    }
  }
}
/* prints the subscript and the contents of all (and only) the active records in the hash table. */
void HT::hTable_print()
{
  bool lastEmp = false;
  for (unsigned int i = 0; i < hsize; i++)
  {
    if (hTable[i].front().key != OPEN)
    {
      if (lastEmp)
        cout << endl;
      cout << setw(4) << i << ": " << hTable[i].front().key << " - "
           << setw(5) << hTable[i].front().num << "   - " << hTable[i].front().desc << endl;
      lastEmp = false;
    }
    else
      lastEmp = true;
  }
  cout << endl;
}
/*  function prints the contents of all (and only) the active records in the hash table */
void HT::pTable_print()
{ //sorts pointer
  unsigned int i, j;
  Entry *bucket;
  for (i = 1; i < hsize; i++)
  {
    bucket = pTable[i];
    for (j = i; (j > 0) && (pTable[j - 1]->key > bucket->key); j--)
      pTable[j] = pTable[j - 1];
    pTable[j] = bucket;
  }
  //prints the table
  for (unsigned int i = 0; i < psize; i++)
  {
    cout << " " << pTable[i]->key
         << " - " << right << setw(4) << pTable[i]->num
         << " - " << left << pTable[i]->desc << endl;
  }
  cout << endl;
}
// compare table and sort
bool cmp(Entry *p, Entry *q)
{
  if (p->key < q->key)
  {
    return true;
  }
  else
  {
    return false;
  }
}
