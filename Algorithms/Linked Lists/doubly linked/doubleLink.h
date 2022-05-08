#include <iostream>

using namespace std;
struct Node
{
public:
   Node(int);
   void setData(int);
   void setNext(Node*);
   void setPrev(Node*);

   int getData();
   Node* getNext();
   Node* getPrev();

private:
   Node* prev, * next;
   int data;
};
Node::Node(int data) {
   setData(data);
   setNext(NULL);
   setPrev(NULL);
}

void Node::setData(int data) {
   this->data = data;
}
void Node::setNext(Node* next) {
   this->next = next;
}
void Node::setPrev(Node* prev) {
   this->prev = prev;
}

int Node::getData() {
   return data;
}
Node* Node::getNext() {
   return next;
}
Node* Node::getPrev() {
   return prev;
}
class List
{
public:
   List();
   void appendNodeBack(int);
   void appendNodeFront(int);
   void insertAfter(int,int);
   bool DeleteNode(int);
   bool DeleteFirst();
   bool DeleteLast();
   bool searchNode(int);
   void displayNodeReverse();
   void displayNodeFarword();

private:
   Node* front, * back;
   int size;
};

List::List() {
   size = 0;
   front = NULL;
   back = NULL;
}
void List::appendNodeBack(int data) {
   Node* myNode = new Node(data);
   if (front == NULL)
   {
       front = myNode;
       back = myNode;
   }
   else
   {
       myNode->setPrev(back);
       back->setNext(myNode);
       back = myNode;
   }
   size++;
}
void List::appendNodeFront(int data) {
   Node* myNode = new Node(data);
   if (front == NULL)
   {
       front = myNode;
       back = myNode;
   }
   else
   {
       myNode->setNext(front);
       front->setPrev(myNode);
       front = myNode;
   }
   size++;
}
void List::insertAfter(int aft, int va) {
   Node* insert = front;
   if (insert==NULL)
   {
       appendNodeFront(va);
   }
   else
   {
       while (insert!=NULL)
       {
           if (insert->getData()==aft)
           {
               Node* insertValue = new Node(va);
               insertValue->setNext(insert->getNext());
               insertValue->setPrev(insert);
               insert->setNext(insertValue);
               insertValue->getNext()->setPrev(insertValue);
           }
           insert = insert->getNext();
       }
   }
   appendNodeBack(aft);
}
bool List::DeleteFirst() {
   Node* delet = front;
   if (front == NULL) {
       return false;
   }
   else {
           Node* temp = front;
           front = front->getNext();
           temp->setNext(NULL);
           temp->setPrev(NULL);
           delete temp;
           size--;
           if (size == 0)
           {
               back = NULL;
           }
           return true;
       }
}
bool List::DeleteLast() {
   Node* delet = front;
   if (front == NULL) {
       return false;
   }
   else {
       delet = back;
       back = back->getPrev();
       back->setNext(NULL);
       delet->setNext(NULL);
       delet->setPrev(NULL);
       size--;
       delete delet;
       if (size == 0)
       {
           front = NULL;
       }
       return true;
       }
}
bool List::DeleteNode(int data) {
   Node* delet = front;
   if (front == NULL) {
       return false;
   }
   else {
       if (delet->getData() == data) {
           DeleteFirst();
       }
       else if (back->getData() == data)
       {
           DeleteLast();
       }
       else {
           delet = delet->getNext();
           while (delet != NULL) {

               if (delet->getData() == data) {
                   Node* temp = delet->getPrev();
                   temp->setNext(delet->getNext());
                   temp = delet->getNext();
                   temp->setPrev(delet->getPrev());
                   delet->setNext(NULL);
                   delet->setPrev(NULL);
                   delete delet;
                   size--;
                   return true;
               }
               delet = delet->getNext();
           }
       }
   }
   return false;
}
void List::displayNodeFarword() {

   Node* display = front;
   if (display == NULL)
   {

       cout << "List is Empty\n";
   }
   else
   {
       cout << "\t\t--( Data Detail )--\n";
       while (display != NULL)
       {
           cout << "\to " << display->getData() << endl;
           display = display->getNext();
       }
   }
}
void List::displayNodeReverse() {

   Node* display = back;
   if (display == NULL)
   {

       cout << "List is Empty\n";
   }
   else
   {
       cout << "\t\t--( Data Detail )--\n";
       while (display != NULL)
       {
           cout << "\to " << display->getData() << endl;
           display = display->getPrev();
       }
   }
}
bool List::searchNode(int value) {

   Node* search = front;
   if (search == NULL)
   {

       cout << "List is Empty\n";
   }
   else
   {
       cout << "\t\t--( Data Detail )--\n";
       while (search != NULL)
       {
           if (search->getData() == value)
               return true;
           search = search->getNext();
       }
   }
   return false;
}
void menu() {
   cout << "1.Inser Value Front\n";
   cout << "2.Inser Value Back\n";
   cout << "3.Inser Value after\n";
   cout << "4.Delete\n";
   cout << "5.search\n";
   cout << "6.Display Reverse\n";
   cout << "7.Display Forword\n";
   cout << "8.Delete First\n";
   cout << "9.Delete Last\n";
   cout << "10.Exit\n";
   cout << "Enter You'r Choice : ";
}