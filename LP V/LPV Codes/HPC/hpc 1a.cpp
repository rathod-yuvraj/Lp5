#include <iostream>
#include <stdlib.h>
#include <queue>
#include <omp.h> 

using namespace std;

class node {
public:
    node* left;
    node* right;
    int data;
};

// Class to handle BST insertion and parallel BFS
class Breadthfs {
public:
    node* insert(node* root, int data);
    void bfs(node* head);
};

// BST-style insert function
node* Breadthfs::insert(node* root, int data) {
    if (!root) {
        root = new node;
        root->left = root->right = NULL;
        root->data = data;
        return root;
    }

    if (data < root->data)
        root->left = insert(root->left, data);
    else
        root->right = insert(root->right, data);

    return root;
}

// Parallel BFS using OpenMP
void Breadthfs::bfs(node* head) {
    if (!head) return;

    queue<node*> q;
    q.push(head);

    while (!q.empty()) {
        int qSize = q.size();

        #pragma omp parallel for
        for (int i = 0; i < qSize; i++) {
            node* currNode;

            #pragma omp critical
            {
                currNode = q.front();
                q.pop();
                cout << currNode->data << " ";
            }

            #pragma omp critical
            {
                if (currNode->left)
                    q.push(currNode->left);
                if (currNode->right)
                    q.push(currNode->right);
            }
        }
    }
}

int main() {
    node* root = NULL;
    int data;
    char ans;
    Breadthfs tree;

    do {
        cout << "\nEnter data: ";
        cin >> data;

        root = tree.insert(root, data);

        cout << "Do you want to insert another node? (y/n): ";
        cin >> ans;
        
    } while (ans == 'y' || ans == 'Y'); 

    cout << "\nBFS Traversal of the Tree: ";
    tree.bfs(root);
    cout << endl;

    return 0;
}
