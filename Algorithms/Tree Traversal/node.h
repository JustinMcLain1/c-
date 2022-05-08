#ifndef NODE_H_
#define NODE_H_

// definition of node in bin tree

template <typename T>
class BinaryTree;  // forward declaration

template <typename T>
class BinarySearchTree;  // forward declaration

template <typename T>
class Node {
  friend class BinaryTree<T>;   // binTree is friend
  friend class BinarySearchTree<T>;  // binSTree is friend

 public:
  Node(const T& = T(), Node<T>* = nullptr,
       Node<T>* = nullptr);  // default constructor

 private:
  T       data;          // data component
  Node<T>*left, *right;  // left and right links
};

// default constructor
template <typename T>
Node<T>::Node(const T& x, Node<T>* l, Node<T>* r) : data(x), left(l), right(r) {}

#endif  // NODE_H_
