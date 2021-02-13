#ifndef FIBONACCI_HEAP_H
#define FIBONACCI_HEAP_H

#include <string>
#include <vector>
#include <map>

using namespace std;

/* Node of Fibonacci Heap */
struct FHNode
{
    int count;      // count value of tag
    string tagName; // name of tag
    FHNode *parent; 
    FHNode *child;
    FHNode *left;
    FHNode *right;
    int degree; // number of direct children
    bool childCut; // true if child is lost; false on max removal
};

/* Fibonacci Heap Class */
class FibonacciHeap
{
    // implements max fibonacci heap
private:
    FHNode *maxNode;    // pointer to node of maximum value in heap
    int numNodes;       // size of heap
    map<int, FHNode *> nodeMap; // for pairwise combination

    // creates new node
    FHNode *createNode(int count, string tagName);
    // meld two nodes
    void meldNodes(FHNode *na, FHNode *nb);
    // returns list(vector) nodes that have same degree
    vector<FHNode *> collectRoots(FHNode *node);
    // adds nodes to nodeMap used for pairwise combination
    FHNode *addToNodeMap(FHNode *node);
    // performs when max node is removed
    FHNode *pairwiseCombine(FHNode *na, FHNode *nb);

public:
    FibonacciHeap();
    ~FibonacciHeap();

    // inserts new key
    FHNode *Insert(int count, string tagName);
    // meld nodes (just a public function for meldNode private function)
    void Meld(FHNode *na, FHNode *nb);
    // getting maximum
    FHNode *GetMax();
    // setting maximum
    void SetMax(FHNode *node);
    // extracts maximum node
    FHNode *RemoveMax();
    // increase the value
    void IncreaseKey(FHNode *node, int count);
    // get number of nodes
    int GetNumNodes()
    {
        return numNodes;
    }
};

#endif // !FIBONACCI_HEAP_H
