#ifndef APP_H
#define APP_H

#include <fstream>
#include <string>
#include <unordered_map>
#include "FibonacciHeap.h"
#include <iostream>

using namespace std;

class App
{
private:
    string inFile;  // input file name
    string outFile; // output file name
    ifstream ifs;   // input file stream
    ofstream ofs;   // output file stream
    unordered_map<string, FHNode*> hashTable; // hash table (imported from STL)
    FibonacciHeap fHeap;    // Fibonacci Heap object

    // processes each line of input file
    void processLine(string &line);
    // adds the tagname and count to hashtable and fibonacci heap
    void addToHashTable(string tagName, int tagCount);
    // prints maximum hashtag names to preferred output (console/outstream)
    void printToOutput(int num);

public:
    // constructor for console based output
    App(char *inputFileName);
    // constructor for file based output
    App(char *inputFilename, char *outputFilename);
    // Destructor
    ~App();

    // Retrieves tag from given input file using input file stream
    void GetTagsFromInputFile();
};

#endif // !APP_H