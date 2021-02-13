#include "FibonacciHeap.h"
#include <iostream>

/*
    Fibonacci Heap methods
*/

FibonacciHeap::FibonacciHeap()
{
    // constructor
    numNodes = 0;
    maxNode = NULL;
}

FibonacciHeap::~FibonacciHeap()
{ // destructor
}

FHNode *FibonacciHeap::createNode(int count, string tagName)
{
    // creates a new node similar to one element fibonacci heap
    FHNode *node = new FHNode();
    node->count = count;
    node->tagName = tagName;
    node->parent = NULL;
    node->child = NULL;
    node->degree = 0;
    node->left = node;
    node->right = node;
    node->childCut = false;
    return node;
}

void FibonacciHeap::meldNodes(FHNode *na, FHNode *nb)
{
    // meld two nodes side by side
    FHNode *naR = na->right;
    FHNode *nbL = nb->left;
    na->right = nb;
    nb->left = na;
    naR->left = nbL;
    nbL->right = naR;
    // return na;
}

FHNode *FibonacciHeap::pairwiseCombine(FHNode *na, FHNode *nb)
{
    if (na == NULL || nb == NULL)
        return na;

    // considering na > nb
    // interchange if nb is greater than na
    if (nb->count > na->count)
    {
        FHNode *temp = na;
        na = nb;
        nb = temp;
    }

    // takes out the second node without changing the heap structure
    nb->parent = na;
    na->degree = na->degree + 1;
    nb->childCut = false;
    nb->left = nb;
    nb->right = nb;
    if (na->child == NULL)
    {
        na->child = nb;
    }
    else
    {
        meldNodes(na->child, nb);
    }
    maxNode = GetMax();
    return na;
}

vector<FHNode *> FibonacciHeap::collectRoots(FHNode *node)
{
    // returns vector of nodes on same level by looping to its neighbour nodes
    vector<FHNode *> result;
    FHNode *temp = node;
    do
    {
        result.push_back(temp);
        temp = temp->left;
    } while (temp != node);
    return result;
}

FHNode *FibonacciHeap::addToNodeMap(FHNode *node)
{
    // keeps on looping until the node finds its place in map and entered. When done, pairwise combine happens
    while (true)
    {
        map<int, FHNode *>::iterator iter;
        iter = nodeMap.find(node->degree);
        if (iter == nodeMap.end())
        {
            if (maxNode->count < node->count)
            {
                maxNode = node;
            }
            nodeMap.insert(pair<int, FHNode *>(node->degree, node));
            return node;
        }
        else if (node != iter->second)
        {
            FHNode *temp = iter->second;
            nodeMap.erase(iter);
            node = pairwiseCombine(temp, node);
        }
    }
    return NULL;
}

FHNode *FibonacciHeap::Insert(int count, string tagName)
{
    // inserts new node into the heap
    FHNode *newNode = createNode(count, tagName);
    if (maxNode == NULL)
    {
        maxNode = newNode;
    }
    else
    {
        meldNodes(maxNode, newNode);
        if (maxNode->count < count)
            maxNode = newNode;
    }
    numNodes++;
    return newNode;
}

void FibonacciHeap::Meld(FHNode *na, FHNode *nb)
{
    // public extension of private meld function
    meldNodes(na, nb);
}

FHNode *FibonacciHeap::GetMax()
{
    // returns node of maximum count
    if (maxNode != NULL)
    {
        return maxNode;
    }
    return NULL;
}

void FibonacciHeap::SetMax(FHNode *node)
{ // sets node to maxnode
    maxNode = node;
}

FHNode *FibonacciHeap::RemoveMax()
{
    // no node in heap
    if (maxNode == NULL)
        return maxNode;

    // max node is only element in heap
    if (maxNode->left == maxNode && maxNode == NULL)
    {
        FHNode *temp = maxNode;
        maxNode = NULL;
        --numNodes;
        return temp;
    }
    if (maxNode->degree != 0)
    {
        FHNode *temp1 = maxNode->child;
        maxNode->child = NULL;
        maxNode->degree = 0;
        FHNode *temp2 = temp1;
        do
        {
            // removing parent value from children
            temp1->parent = NULL;
            temp1 = temp1->left;
        } while (temp1 != temp2);
        meldNodes(maxNode, temp1);
        if (maxNode->count < temp1->count)
            maxNode = temp1;
    }
    // removing maxNode from heap now
    FHNode *result = maxNode;
    FHNode *temp1 = maxNode->right;
    maxNode->left->right = maxNode->right;
    maxNode->right->left = maxNode->left;
    FHNode *temp2 = temp1;
    maxNode = temp1;
    vector<FHNode *> list = collectRoots(temp2);
    nodeMap.clear();
    for (int i = 0; i < list.size(); i++)
    {
        list[i]->left = NULL;
        list[i]->right = NULL;
        addToNodeMap(list[i]);
    }
    map<int, FHNode *>::iterator iter = nodeMap.begin();
    maxNode = (*iter).second; // doubt
    maxNode->left = maxNode;
    maxNode->right = maxNode;
    while (iter != nodeMap.end())
    {
        (*iter).second->left = (*iter).second;
        (*iter).second->right = (*iter).second;
        meldNodes(maxNode, (*iter).second);
        if (maxNode->count < (*iter).second->count)
        {
            maxNode = (*iter).second;
        }
        ++iter;
    }
    --numNodes;
    return result;
}

void FibonacciHeap::IncreaseKey(FHNode *node, int count)
{
    // increases count value
    if (node == NULL)
        return;
    if (node->parent == NULL)
    {
        node->count += count;
        if (node->count > maxNode->count)
            maxNode = node;
        return;
    }
    if (node->parent != NULL)
    {
        node->count = node->count + count;
        if (node->count < node->parent->count)
            return;

        else
        {
            do
            {
                if (node->left == node)
                {
                    // if node is the only child in the parent
                    node->parent->child = NULL;
                }
                else
                {
                    //if node has many other siblings
                    node->parent->child = node->left;
                    node->left->right = node->right;
                    node->right->left = node->left;
                    node->left = node;
                    node->right = node;
                }
                //node's parent degree has to be reduced as one element has been taken out of the loop
                node->parent->degree -= 1;
                FHNode *temp = node->parent;
                node->parent = NULL;
                meldNodes(maxNode, node);
                if (maxNode->count < node->count)
                {
                    maxNode = node;
                }
                node = temp;
            } while (node != NULL && node->parent != NULL && node->childCut != false);
            if (node != NULL && node->parent != NULL)
            {
                //make the last node of loop true
                node->childCut = true;
            }
        }
    }
}