#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <sstream>
#include <stack>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>

using namespace std;
using namespace std::chrono;

/////////////////// Player ///////////////////
class publisher {
public:
  string name;
  float na_sales;
  float eu_sales;
  float others_sales;
};

/////////////////// Node ///////////////////
class Node {
public:
  publisher key;
  int color; // "Red"=1 or "Black"=0
  Node *parent, *left, *right;

  Node(publisher key) {
    this->key = key;
    this->color = 1; // "RED"
    this->parent = NULL;
    this->left = NULL;
    this->right = NULL;
  }
  ~Node() {}
};

/////////////////// RB-Tree ///////////////////
class RB_tree {
private:
  Node *root;

public:
  publisher *best_seller[3];
  stack<string> tree_deep_stack;

  Node *get_root();
  Node *search(string publisher_name);
  Node *RB_insert(Node *root, Node *ptr);
  void insertValue(vector<string>);
  void RB_left_rotate(Node *);
  void RB_right_rotate(Node *);
  void RB_insert_fixup(Node *);
  void preorder();
  void find_best_seller();

  RB_tree() {
    this->root = NULL;
    this->best_seller[0] = NULL;
    this->best_seller[1] = NULL;
    this->best_seller[2] = NULL;
  }
  ~RB_tree() {}
};

void print_best_sellers(int year, publisher *temp_publisher[3]) {
  cout.precision(5);
  cout << "End of the " + to_string(year) + " Year" << endl;
  cout << "Best seller in North America: " + temp_publisher[0]->name + " - "
       << temp_publisher[0]->na_sales << " million" << endl;
  cout << "Best seller in Europe: " + temp_publisher[1]->name + " - "
       << temp_publisher[1]->eu_sales << " million" << endl;
  cout << "Best seller rest of the World: " + temp_publisher[2]->name + " - "
       << temp_publisher[2]->others_sales << " million" << endl;
}

Node *RB_tree::get_root() { return this->root; }

Node *RB_tree::search(string publisher_name) {
  Node *current = root;
  while (current != NULL) {
    if (publisher_name == current->key.name) {
      return current;
    }
    if (publisher_name < current->key.name) {
      current = current->left;
    } else {
      current = current->right;
    }
  }
  return NULL;
}

Node *RB_tree::RB_insert(Node *root, Node *ptr) {
  // Base case: if root is null, return new node
  if (root == NULL) {
    return ptr;
  }

  // If publisher already exists, update sales data
  if (root->key.name == ptr->key.name) {
    root->key.na_sales += ptr->key.na_sales;
    root->key.eu_sales += ptr->key.eu_sales;
    root->key.others_sales += ptr->key.others_sales;
    delete ptr;
    return root;
  }

  // Recursively insert based on publisher name
  if (ptr->key.name < root->key.name) {
    root->left = RB_insert(root->left, ptr);
    root->left->parent = root;
  } else {
    root->right = RB_insert(root->right, ptr);
    root->right->parent = root;
  }

  return root;
}

void RB_tree::insertValue(vector<string> data) {
  // Create new publisher
  publisher new_publisher;
  new_publisher.name = data[3];               // Publisher name
  new_publisher.na_sales = stof(data[4]);     // NA sales
  new_publisher.eu_sales = stof(data[5]);     // EU sales
  new_publisher.others_sales = stof(data[6]); // Other sales

  // Create new node
  Node *ptr = new Node(new_publisher);

  // Insert into tree
  this->root = RB_insert(this->root, ptr);

  // Fix Red-Black properties
  if (this->root == ptr) {
    ptr->color = 0; // Root should be black
  } else {
    RB_insert_fixup(ptr);
  }
}

void RB_tree::RB_left_rotate(Node *x) {
  if (!x || !x->right)
    return;

  Node *y = x->right;
  x->right = y->left;

  if (y->left != NULL) {
    y->left->parent = x;
  }

  y->parent = x->parent;

  if (x->parent == NULL) {
    this->root = y;
  } else if (x == x->parent->left) {
    x->parent->left = y;
  } else {
    x->parent->right = y;
  }

  y->left = x;
  x->parent = y;
}

void RB_tree::RB_right_rotate(Node *y) {
  if (!y || !y->left)
    return;

  Node *x = y->left;
  y->left = x->right;

  if (x->right != NULL) {
    x->right->parent = y;
  }

  x->parent = y->parent;

  if (y->parent == NULL) {
    this->root = x;
  } else if (y == y->parent->right) {
    y->parent->right = x;
  } else {
    y->parent->left = x;
  }

  x->right = y;
  y->parent = x;
}

void RB_tree::RB_insert_fixup(Node *k) {
  Node *u;
  while (k->parent != NULL && k->parent->color == 1) {
    if (k->parent == k->parent->parent->right) {
      u = k->parent->parent->left;
      if (u != NULL && u->color == 1) {
        u->color = 0;
        k->parent->color = 0;
        k->parent->parent->color = 1;
        k = k->parent->parent;
      } else {
        if (k == k->parent->left) {
          k = k->parent;
          RB_right_rotate(k);
        }
        k->parent->color = 0;
        k->parent->parent->color = 1;
        RB_left_rotate(k->parent->parent);
      }
    } else {
      u = k->parent->parent->right;
      if (u != NULL && u->color == 1) {
        u->color = 0;
        k->parent->color = 0;
        k->parent->parent->color = 1;
        k = k->parent->parent;
      } else {
        if (k == k->parent->right) {
          k = k->parent;
          RB_left_rotate(k);
        }
        k->parent->color = 0;
        k->parent->parent->color = 1;
        RB_right_rotate(k->parent->parent);
      }
    }
    if (k == root)
      break;
  }
  root->color = 0;
}

void RB_tree::preorder() {
  stack<pair<Node *, int>> s;
  if (root)
    s.push({root, 0});

  while (!s.empty()) {
    Node *current = s.top().first;
    int depth = s.top().second;
    s.pop();

    // Print node info
    for (int i = 0; i < depth; i++)
      cout << "  ";
    cout << current->key.name << " (" << (current->color ? "RED" : "BLACK")
         << ")" << endl;

    // Push right child first (so left will be processed first)
    if (current->right)
      s.push({current->right, depth + 1});
    if (current->left)
      s.push({current->left, depth + 1});
  }
}

void RB_tree::find_best_seller() {
  if (!root)
    return;

  // Initialize best sellers with root data
  best_seller[0] = &root->key; // NA
  best_seller[1] = &root->key; // EU
  best_seller[2] = &root->key; // Others

  stack<Node *> s;
  Node *current = root;

  while (current || !s.empty()) {
    // Reach the leftmost Node of the current Node
    while (current) {
      s.push(current);
      current = current->left;
    }

    current = s.top();
    s.pop();

    // Compare sales
    if (current->key.na_sales > best_seller[0]->na_sales)
      best_seller[0] = &current->key;
    if (current->key.eu_sales > best_seller[1]->eu_sales)
      best_seller[1] = &current->key;
    if (current->key.others_sales > best_seller[2]->others_sales)
      best_seller[2] = &current->key;

    current = current->right;
  }
}

RB_tree generate_RBT_tree_from_csv(string file_name) {
  RB_tree temp_RBtree;
  ifstream file(file_name);
  string line;

  // Skip header
  getline(file, line);

  // Read data
  while (getline(file, line)) {
    vector<string> row;
    stringstream ss(line);
    string cell;

    // Parse CSV line
    while (getline(ss, cell, ',')) {
      row.push_back(cell);
    }

    // Insert into tree
    if (row.size() >= 7) { // Ensure we have all required fields
      temp_RBtree.insertValue(row);
    }
  }

  return temp_RBtree;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cout << "Usage: " << argv[0] << " <csv_file>" << endl;
    return EXIT_FAILURE;
  }

  string fname = argv[1];

  // Measure insertion time
  auto start = high_resolution_clock::now();
  RB_tree RBtree = generate_RBT_tree_from_csv(fname);
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  cout << "Time taken to build RB tree: " << duration.count() << " microseconds"
       << endl;

  // Perform random searches
  vector<string> publishers;
  ifstream file(fname);
  string line;
  getline(file, line); // Skip header
  while (getline(file, line)) {
    stringstream ss(line);
    string cell;
    vector<string> row;
    while (getline(ss, cell, ',')) {
      row.push_back(cell);
    }
    if (find(publishers.begin(), publishers.end(), row[3]) ==
        publishers.end()) {
      publishers.push_back(row[3]);
    }
  }

  // Perform 50 random searches
  random_device rd;
  mt19937 gen(rd());
  uniform_int_distribution<> dis(0, publishers.size() - 1);

  long long total_search_time = 0;
  for (int i = 0; i < 50; i++) {
    string search_publisher = publishers[dis(gen)];
    auto search_start = high_resolution_clock::now();
    RBtree.search(search_publisher);
    auto search_stop = high_resolution_clock::now();
    total_search_time +=
        duration_cast<nanoseconds>(search_stop - search_start).count();
  }
  cout << "Average search time: " << total_search_time / 50 << " nanoseconds"
       << endl;

  // Find and print best sellers for each decade
  vector<int> decades = {1990, 2000, 2010, 2020};
  for (int year : decades) {
    RBtree.find_best_seller();
    print_best_sellers(year, RBtree.best_seller);
  }

  // Print tree structure
  cout << "\nRB-Tree Structure (Preorder):" << endl;
  RBtree.preorder();

  return EXIT_SUCCESS;
}
