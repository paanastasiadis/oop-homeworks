/**
 * @file AVL.cpp
 * @author Panagiotis Anastasiadis 2134
 * @brief  Homework 4
 *
 * @copyright Copyright (c) 2020
 *
 */

#include "../include/AVL.hpp"
#include <fstream>
#include <iostream>
using namespace std;

AVL::Node::Node(const string &e, Node *parent, Node *left, Node *right) {
  this->element = e;
  this->parent = parent;
  this->left = left;
  this->right = right;
  this->height = 0;
  this->height_diff = 0;
}
AVL::Node *AVL::Node::getParent(void) const { return this->parent; }

AVL::Node *AVL::Node::getLeft(void) const { return this->left; }

AVL::Node *AVL::Node::getRight(void) const { return this->right; }

string AVL::Node::getElement(void) const { return this->element; }

int AVL::Node::getHeight(void) const { return this->height; }

void AVL::Node::setLeft(Node *left_ptr) { this->left = left_ptr; }

void AVL::Node::setRight(Node *right_ptr) { this->right = right_ptr; }

void AVL::Node::setParent(Node *parent_ptr) { this->parent = parent_ptr; }

void AVL::Node::setElement(string e) { this->element = e; }

bool AVL::Node::isLeft(void) const {
  if (this->getParent()->getLeft() == this) {
    return true;
  }
  return false;
}

bool AVL::Node::isRight(void) const {
  if (this->getParent()->getRight() == this) {
    return true;
  }
  return false;
}

int AVL::Node::leftChildHeight(void) const {
  Node *left_child = this->getLeft();
  if (left_child == NULL) {
    return 0;
  }

  return left_child->getHeight();
}

int AVL::Node::rightChildHeight(void) const {
  Node *right_child = this->getRight();

  if (right_child == NULL) {
    return 0;
  }

  return right_child->getHeight();
}
int AVL::Node::getHeightDiff(void) const { return this->height_diff; }

int AVL::Node::updateHeight(void) {

  // set the height of the node by accessing
  this->setHeight(this);

  return (this->getHeight());
}

int AVL::Node::setHeight(AVL::Node *child_node) {
  int h = 0;
  if (child_node != NULL) {

    // find height of left and right children by accessing the subtrees
    // recursively
    int l_height = setHeight(child_node->left);
    int r_height = setHeight(child_node->right);

    // update height difference of node's subtrees
    child_node->height_diff = l_height - r_height;

    // set node height
    int max_height = max(l_height, r_height);
    h = max_height + 1;
    child_node->height = h;
  }
  return h;
}

bool AVL::Node::isBalanced(void) {
  int diff = this->getHeightDiff();

  // check for unbalance by calculating the height difference
  // of the two subtrees
  if (diff > 1 || diff < -1) {
    return false;
  }
  return true;
}

AVL::Iterator::Iterator(AVL::Node *root) {
  if (root != NULL) {
    node_stack.push(root);
  }
  point_node = root;
}

AVL::Iterator &AVL::Iterator::operator++() {

  if (!node_stack.empty()) {
    AVL::Node *node = node_stack.top();
    node_stack.pop(); // pop the top node in  stack
    AVL::Node *r_child = node->getRight();
    AVL::Node *l_child = node->getLeft();

    if (r_child != NULL) {
      node_stack.push(r_child);
      point_node = r_child;
    }
    if (l_child != NULL) {
      node_stack.push(l_child);
      point_node = l_child;
    }

    if (node_stack.empty()) {
      point_node = NULL;
    } else if (l_child == NULL && r_child == NULL) {
      point_node =
          node_stack.top(); // update the iterator to point to the next node
    }
  }

  return *this;
}

AVL::Iterator AVL::Iterator::operator++(int a) {

  // dummy value a
  if (a) {
    a = 0;
  }

  AVL::Node *node = NULL;

  if (!node_stack.empty()) {
    node = node_stack.top();
    node_stack.pop();
    AVL::Node *r_child = node->getRight();
    AVL::Node *l_child = node->getLeft();

    if (r_child != NULL) {
      node_stack.push(r_child);
      point_node = r_child;
    }
    if (l_child != NULL) {
      node_stack.push(l_child);
      point_node = l_child;
    }

    if (node_stack.empty()) {
      point_node = NULL;
    } else if (l_child == NULL && r_child == NULL) {
      point_node = node_stack.top();
    }
  }

  return AVL::Iterator(node);
}

string AVL::Iterator::operator*() { return point_node->getElement(); }

AVL::Node *AVL::Iterator::getNode() { return point_node; }

bool AVL::Iterator::operator!=(Iterator it) {
  if (this->point_node != it.point_node) {
    return true;
  } else {
    return false;
  }
}

bool AVL::Iterator::operator==(Iterator it) {
  if (this->point_node == it.point_node) {
    return true;
  } else {
    return false;
  }
}
AVL::AVL() {
  this->root = NULL;
  this->size = 0;
}

AVL::AVL(const AVL &avl_tree) {
  this->root = NULL;
  this->size = 0;

  for (AVL::Iterator it = avl_tree.begin(); it != avl_tree.end(); ++it) {
    add(*it);
  }
}

bool AVL::add(string e) {

  AVL::Node *node;
  node = insertRecursively(this->root, NULL, e);

  if (node == NULL) {
    return false;
  } else {
    this->size++;
    return true;
  }
}

bool AVL::contains(string e) {
  AVL::Node *node;
  node = findRecursively(this->root, e);

  if (node == NULL) {
    return false;
  } else {
    return true;
  }
}

bool AVL::rmv(string e) {
  AVL::Node *node;
  node = deleteRecursively(this->root, e);

  if (node == NULL && node != root) {
    return false;
  } else {
    this->size--;
    return true;
  }
}
AVL::Node *AVL::findRecursively(AVL::Node *curr_node, const string &value) {
  if (curr_node == NULL) {
    return NULL; // value not found, return null
  } else if (value.compare(curr_node->getElement()) == 0) {
    // value found, return the corresponding node
    return curr_node;
  } else if (value < curr_node->getElement()) {
    curr_node = findRecursively(curr_node->getLeft(), value);
  } else if (value > curr_node->getElement()) {
    curr_node = findRecursively(curr_node->getRight(), value);
  }
  return curr_node;
}

AVL::Node *AVL::insertRecursively(AVL::Node *curr_node, AVL::Node *parent_node,
                                  const string &value) {

  // create a new node for the new value
  if (curr_node == NULL) {
    curr_node = new AVL::Node(value, parent_node, NULL, NULL);

    // when node is root, update the root pointer, too
    if (this->root == NULL) {
      this->root = curr_node;
    }
    // set the node height
    curr_node->updateHeight();

    return curr_node;
  } else if (value.compare(curr_node->getElement()) == 0) {
    // value already exists, return null
    return NULL;
  } else if (value < curr_node->getElement()) {

    AVL::Node *node = insertRecursively(curr_node->getLeft(), curr_node, value);

    if (node == NULL) {
      return NULL;
    }
    curr_node->setLeft(node);
    node->setParent(curr_node);

    // update height of nodes subtrees after the insertion
    curr_node->updateHeight();
    if (!curr_node->isBalanced()) {

      // check if node to be balanced is root in order to update the root
      // pointer
      if (root == curr_node) {
        root = this->setBalance(curr_node);
        curr_node = root;
      } else {
        curr_node = this->setBalance(curr_node);
      }
    }
  } else if (value > curr_node->getElement()) {

    AVL::Node *node =
        insertRecursively(curr_node->getRight(), curr_node, value);

    if (node == NULL) {
      return NULL;
    }
    curr_node->setRight(node);
    node->setParent(curr_node);

    curr_node->updateHeight();
    if (!curr_node->isBalanced()) {
      if (root == curr_node) {
        root = this->setBalance(curr_node);
        curr_node = root;
      } else {
        curr_node = this->setBalance(curr_node);
      }
    }
  }

  return curr_node;
}

AVL::Node *AVL::deleteRecursively(AVL::Node *curr_node, const string &value) {
  if (curr_node == NULL) {
    return NULL;

  } else if (value < curr_node->getElement()) {
    AVL::Node *node = deleteRecursively(curr_node->getLeft(), value);

    curr_node->setLeft(node);

    if (node != NULL) {
      node->setParent(curr_node);
    }

  } else if (value > curr_node->getElement()) {
    AVL::Node *node = deleteRecursively(curr_node->getRight(), value);

    curr_node->setRight(node);

    if (node != NULL) {
      node->setParent(curr_node);
    }

  } else if (value.compare(curr_node->getElement()) == 0) {

    AVL::Node *node_to_delete;

    if (curr_node->getLeft() == NULL &&
        curr_node->getRight() == NULL) { // no children case
      node_to_delete = curr_node;
      if (curr_node == root) {
        root = NULL;
      }

      curr_node = NULL;
      delete node_to_delete;

    } else if (curr_node->getLeft() == NULL) { // right child only case
      node_to_delete = curr_node;
      if (curr_node == root) {
        root = curr_node->getRight();
      }
      AVL::Node *parent = curr_node->getParent();
      curr_node = curr_node->getRight();
      curr_node->setParent(parent);
      delete node_to_delete;

    } else if (curr_node->getRight() == NULL) { // left child only case
      node_to_delete = curr_node;
      if (curr_node == root) {
        root = curr_node->getLeft();
      }
      AVL::Node *parent = curr_node->getParent();
      curr_node = curr_node->getLeft();
      curr_node->setParent(parent);

      delete node_to_delete;

    } else { // both children exist, delete the leftest node in right subtree
      node_to_delete = deleteLeftestInRight(curr_node->getRight());
      curr_node->setElement(node_to_delete->getElement());

      AVL::Node *node = deleteRecursively(curr_node->getRight(),
                                          node_to_delete->getElement());

      curr_node->setRight(node);
      if (node != NULL) {
        node->setParent(curr_node);
      }
    }
  }

  if (curr_node == NULL) {
    return NULL;
  }

  curr_node->updateHeight();

  // check the balance after deletion
  if (!curr_node->isBalanced()) {
    if (root == curr_node) {
      root = this->setBalance(curr_node);
      curr_node = root;
    } else {
      curr_node = this->setBalance(curr_node);
    }
  }
  return curr_node;
}

AVL::Node *AVL::deleteLeftestInRight(AVL::Node *node) {

  if (node == NULL) {

    return NULL;

  } else if (node->getLeft() == NULL) {
    return node;
  } else {
    return (deleteLeftestInRight(node->getLeft()));
  }
}

AVL::Node *AVL::setBalance(AVL::Node *n) {
  int diff = n->getHeightDiff();
  if (diff > 1 && n->getLeft()->getHeightDiff() >= 0) {
    n = rotateLeftLeft(n);
  } else if (diff > 1 && n->getLeft()->getHeightDiff() < 0) {
    n = rotateLeftRight(n);
  } else if (diff < -1 && n->getRight()->getHeightDiff() <= 0) {
    n = rotateRightRight(n);
  } else if (diff < -1 && n->getRight()->getHeightDiff() > 0) {
    n = rotateRightLeft(n);
  }
  return n;
}

/**
 * Right - Right Rotation
 */
AVL::Node *AVL::rotateRightRight(AVL::Node *n) {

  AVL::Node *tmp;
  tmp = n->getRight();

  AVL::Node *rotated_child = tmp->getLeft();
  if (rotated_child != NULL) {
    rotated_child->setParent(n);
  }

  n->setRight(rotated_child);
  tmp->setLeft(n);

  n->setParent(tmp);

  return tmp;
}

/**
 * Left - Left Rotation
 */
AVL::Node *AVL::rotateLeftLeft(AVL::Node *n) {
  AVL::Node *tmp;
  tmp = n->getLeft();

  AVL::Node *rotated_child = tmp->getRight();
  if (rotated_child != NULL) {
    rotated_child->setParent(n);
  }

  n->setLeft(rotated_child);
  tmp->setRight(n);

  n->setParent(tmp);
  return tmp;
}

/**
 * Left - Right Rotation
 */
AVL::Node *AVL::rotateLeftRight(AVL::Node *n) {
  AVL::Node *tmp;
  tmp = n->getLeft();
  AVL::Node *rotated_child = rotateRightRight(tmp);
  if (rotated_child != NULL) {
    rotated_child->setParent(n);
  }
  n->setLeft(rotated_child);
  return rotateLeftLeft(n);
}

/**
 * Right - Left Rotation
 */
AVL::Node *AVL::rotateRightLeft(AVL::Node *n) {

  AVL::Node *tmp;
  tmp = n->getRight();
  AVL::Node *rotated_child = rotateLeftLeft(tmp);
  if (rotated_child != NULL) {
    rotated_child->setParent(n);
  }
  n->setRight(rotated_child);
  return rotateRightRight(n);
}

void AVL::pre_order(std::ostream &out) const {

  for (AVL::Iterator it = this->begin(); it != this->end(); ++it) {
    out << *it << " ";
  }
}

void AVL::print2DotFile(char *filename) {

  ofstream fout(filename);
  fout << "graph Trie {" << endl;

  for (AVL::Iterator it = this->begin(); it != this->end(); ++it) {
    if (*it == this->root->getElement()) {
      fout << *it;
      fout << " [label = \"";
      fout << *it;
      fout << "\", shape=egg, ";
      fout << "color=red, style=filled]" << endl;
    } else {
      fout << it.getNode()->getParent()->getElement();
      fout << " -- ";
      fout << *it << endl;
      fout << *it;
      fout << " [label = \"";
      fout << *it;
      fout << "\", shape=egg, ";
      fout << "color=red, style=filled]" << endl;
    }
  }
  fout << "}" << endl;
}

std::ostream &operator<<(std::ostream &out, const AVL &tree) {

  tree.pre_order(out);

  return out;
}

AVL &AVL::operator=(const AVL &avl) {
  if (this->root != NULL) {

    while (this->size != 0) {
      this->rmv(root->getElement());
    }
  }

  for (AVL::Iterator it = avl.begin(); it != avl.end(); ++it) {
    this->add(*it);
  }

  return *this;
}

AVL AVL::operator+(const AVL &avl) {
  AVL avl_sum;

  for (AVL::Iterator it = this->begin(); it != this->end(); ++it) {
    avl_sum.add(*it);
  }

  for (AVL::Iterator it = avl.begin(); it != avl.end(); ++it) {
    avl_sum.add(*it);
  }

  return avl_sum;
}

AVL AVL::operator+(const string &e) {
  AVL new_avl;

  for (AVL::Iterator it = this->begin(); it != this->end(); ++it) {
    new_avl.add(*it);
  }
  new_avl.add(e);
  return new_avl;
}
AVL AVL::operator-(const string &e) {
  AVL new_avl;

  for (AVL::Iterator it = this->begin(); it != this->end(); ++it) {
    new_avl.add(*it);
  }
  new_avl.rmv(e);
  return new_avl;
}

AVL &AVL::operator+=(const AVL &avl) {
  for (AVL::Iterator it = avl.begin(); it != avl.end(); ++it) {
    this->add(*it);
  }
  return *this;
}

AVL &AVL::operator-=(const string &e) {
  this->rmv(e);
  return *this;
}

AVL &AVL::operator+=(const string &e) {
  this->add(e);
  return *this;
}
AVL::Iterator AVL::begin() const { return Iterator(this->root); }

AVL::Iterator AVL::end() const { return Iterator(NULL); }