#include<string>
#include<iostream>
#include<fstream>
#include <sstream>
#include "doubleLink.h"
using namespace std;


int main()
{
   int choice;
   List list;
   do
   {
       menu();
       cin >> choice;
       switch (choice)
       {
       case 1: {
           cout << "Enter the Value : ";
           int data; cin >> data;
           list.appendNodeFront(data);
           break;
       }
       case 2: {
           cout << "Enter the Value : ";
           int data; cin >> data;
           list.appendNodeBack(data);
       }
       case 3: {
           cout << "Enter the Value after : ";
           int data,in; cin >> data;
           cout << "Enter the inserted Value : ";
           cin >> in;
           list.insertAfter(data,in);
           break;
       }
       case 4: {
           cout << "Enter the Value to delete : ";
           int data; cin >> data;
           if (list.DeleteNode(data))
           {
               cout << "data Deleted\n";
           }
           else {
               cout << "data Not found\n";
           }
           break;
       }
       case 5: {
           cout << "Enter the Value to search : ";
           int data; cin >> data;
           if (list.searchNode(data))
           {
               cout << "data fount\n";
           }
           else {
               cout << "data Not found\n";
           }
           break;
       }
       case 6: {
           list.displayNodeFarword();
           break;
       }
       case 7: {
           list.displayNodeReverse();
           break;
       }
       case 8: {
           if (list.DeleteFirst())
           {
               cout << "Deleted\n";
           }
           else
           {
               cout << "Not Found\n";
           }
           break;
       }
       case 9: {
           if (list.DeleteLast())
           {
               cout << "Deleted\n";
           }
           else
           {
               cout << "Not Found\n";
           }
           break;
       }
       default:
           if (choice != 10)
           {
               cout << "You Enter the Wrong Choice\n\tPlz Enter the Right Choice \n";
           }
           break;
       }
   } while (choice != 10);
   return 0;
}

