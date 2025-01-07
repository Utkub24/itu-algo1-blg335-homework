#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <stack>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <sstream>

using namespace std;
using namespace std::chrono;

/////////////////// Publisher ///////////////////
class publisher {
public:
    string name;
    float na_sales;
    float eu_sales;
    float others_sales;
    int year;
};

/////////////////// Node ///////////////////
class Node {
public:
    publisher key;
    Node *parent, *left, *right;

    Node(publisher key) {
        this->key = key;
        this->parent = NULL;
        this->left = NULL;
        this->right = NULL;
    }

    ~Node() {}
};

void findBestSellersHelper(Node* node, publisher* best_seller[3], int decade) {
    if (node == NULL) return;
    
    bool isInDecade;
    if (decade == 2020) {
        isInDecade = (node->key.year >= 1980 && node->key.year <= 2020);
    } else {
        isInDecade = (node->key.year >= 1980 && node->key.year <= decade + 9);
    }
    
    if (isInDecade) {
        if (best_seller[0] == NULL || 
            (node->key.na_sales > best_seller[0]->na_sales)) {
            best_seller[0] = &(node->key);
        }
        
        if (best_seller[1] == NULL || 
            (node->key.eu_sales > best_seller[1]->eu_sales)) {
            best_seller[1] = &(node->key);
        }
        
        if (best_seller[2] == NULL || 
            (node->key.others_sales > best_seller[2]->others_sales)) {
            best_seller[2] = &(node->key);
        }
    }
    
    findBestSellersHelper(node->left, best_seller, decade);
    findBestSellersHelper(node->right, best_seller, decade);
}

/////////////////// BST-Tree ///////////////////
class BST_tree {
private:
    Node *root;

public:
    publisher *best_seller[3];
    stack<string> tree_deep_stack;
    vector<string> all_publishers;

    Node *get_root() {
        return this->root;
    }

    Node* search(string publisher_name) {
        Node* current = root;
        while (current != NULL) {
            if (current->key.name == publisher_name) {
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

    void collectPublishers(Node* node) {
        if (node == NULL) return;
        
        if (find(all_publishers.begin(), all_publishers.end(), node->key.name) == all_publishers.end()) {
            all_publishers.push_back(node->key.name);
        }
        
        collectPublishers(node->left);
        collectPublishers(node->right);
    }

    Node *BST_insert(Node *root, Node *ptr) {
        if (root == NULL) {
            root = ptr;
            return root;
        }
        
        if (root->key.name == ptr->key.name) {
            root->key.na_sales += ptr->key.na_sales;
            root->key.eu_sales += ptr->key.eu_sales;
            root->key.others_sales += ptr->key.others_sales;
            delete ptr;
            return root;
        }
        
        if (ptr->key.name < root->key.name) {
            root->left = BST_insert(root->left, ptr);
            if (root->left != NULL) {
                root->left->parent = root;
            }
        } else {
            root->right = BST_insert(root->right, ptr);
            if (root->right != NULL) {
                root->right->parent = root;
            }
        }
        
        return root;
    }

    void insertValue(vector<string> data) {
        try {
            publisher new_pub;
            new_pub.name = data[3];
            new_pub.na_sales = stof(data[4]);
            new_pub.eu_sales = stof(data[5]);
            new_pub.others_sales = stof(data[6]);
            new_pub.year = stoi(data[2]);
            
            if (new_pub.name.empty()) {
                cerr << "Warning: Empty publisher name found" << endl;
                return;
            }
            
            if (new_pub.na_sales < 0 || new_pub.eu_sales < 0 || new_pub.others_sales < 0) {
                cerr << "Warning: Negative sales values found for publisher " << new_pub.name << endl;
                return;
            }
            
            Node* new_node = new Node(new_pub);
            this->root = BST_insert(this->root, new_node);
        } catch (const exception& e) {
            cerr << "Error in insertValue: " << e.what() << endl;
        }
    }

    void find_best_seller(int decade) {
        best_seller[0] = NULL;
        best_seller[1] = NULL;
        best_seller[2] = NULL;
        findBestSellersHelper(this->root, best_seller, decade);
    }

    void find_best_seller() {
        find_best_seller(0);
    }

    BST_tree() {
        this->root = NULL;
        this->best_seller[0] = NULL;
        this->best_seller[1] = NULL;
        this->best_seller[2] = NULL;
    }

    ~BST_tree() {}
};


void print_best_sellers(int year, publisher *temp_publisher[3]) {
    cout.precision(5);
    cout << "\n=== End of the " + to_string(year) + " Year ===" << endl;
    
    if (temp_publisher[0] == NULL || temp_publisher[1] == NULL || temp_publisher[2] == NULL) {
        cout << "No data available for this time period." << endl;
        return;
    }
    
    cout << "Best seller in North America: " + temp_publisher[0]->name + " - "
         << temp_publisher[0]->na_sales << " million" << endl;
    cout << "Best seller in Europe: " + temp_publisher[1]->name + " - "
         << temp_publisher[1]->eu_sales << " million" << endl;
    cout << "Best seller rest of the World: " + temp_publisher[2]->name + " - "
         << temp_publisher[2]->others_sales << " million" << endl;
    cout << "----------------------------------------" << endl;
}

BST_tree generate_BST_tree_from_csv(string file_name) {
    BST_tree temp_BSTtree;
    ifstream file(file_name);
    string line;
    
    getline(file, line);
    
    while (getline(file, line)) {
        vector<string> row;
        stringstream ss(line);
        string token;
        
        while (getline(ss, token, ',')) {
            row.push_back(token);
        }
        
        if (row.size() >= 7) {
            temp_BSTtree.insertValue(row);
        }
    }
    
    return temp_BSTtree;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        cout << "Usage: " << argv[0] << " <csv_file>" << endl;
        return EXIT_FAILURE;
    }

    string fname = argv[1];
    
    // Measure tree building time
    auto start = high_resolution_clock::now();
    BST_tree BSTtree = generate_BST_tree_from_csv(fname);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    
    cout << "\nBST Construction Time: " << duration.count() << " microseconds" << endl;
    
    // Collect publishers and perform searches
    BSTtree.collectPublishers(BSTtree.get_root());
    
    // Random search setup
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, BSTtree.all_publishers.size() - 1);
    
    // Perform 50 random searches
    int num_searches = 50;
    vector<long long> search_times;
    
    cout << "\nPerforming " << num_searches << " random searches..." << endl;
    cout << "----------------------------------------" << endl;
    
    for (int i = 0; i < num_searches; i++) {
        string target_publisher = BSTtree.all_publishers[dis(gen)];
        
        auto search_start = high_resolution_clock::now();
        BSTtree.search(target_publisher);
        auto search_end = high_resolution_clock::now();
        
        auto search_time = duration_cast<nanoseconds>(search_end - search_start).count();
        search_times.push_back(search_time);
        
        cout << "Search " << setw(2) << i + 1 << ": ";
        cout << left << setw(30) << target_publisher;
        cout << right << setw(10) << search_time << " ns" << endl;
    }
    
    // Calculate and display average search time
    double avg_search_time = accumulate(search_times.begin(), search_times.end(), 0.0) / num_searches;
    cout << "----------------------------------------" << endl;
    cout << "Average search time: " << fixed << setprecision(2) << avg_search_time << " nanoseconds\n" << endl;
    
    // Find and print best sellers for each decade
    vector<int> decades = {1990, 2000, 2010, 2020};
    for (int decade : decades) {
        BSTtree.find_best_seller(decade);
        print_best_sellers(decade, BSTtree.best_seller);
    }

    return EXIT_SUCCESS;
}
