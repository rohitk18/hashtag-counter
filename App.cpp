#include <sstream>
#include "App.h"

App::App(char *inputFilename) : inFile(inputFilename)
{
    // constructor with console output
    ifs.open(inFile, ios::in);
}

App::App(char *inputFilename, char *outputFilename) : inFile(inputFilename), outFile(outputFilename)
{
    // constructor with file output stream
    ifs.open(inFile, ios::in);
    ofs.open(outFile, ios::out);
}

App::~App()
{
    // closes if output stream is opened
    if (ofs.is_open())
        ofs.close();
}

void App::GetTagsFromInputFile()
{
    // checks if input stream is opened or not
    if (!ifs.is_open())
    {
        if (inFile.empty())
        {
            cout << "No input file received. Exiting...\n";
            exit(1);
        }
        else
        {
            ifs.open(inFile, ios::in);
        }
    }
    string line;
    // stops until it finds stop word
    while (line.compare("stop") && line.compare("STOP"))
    {
        processLine(line);
        getline(ifs, line);
    }
}

void App::processLine(string &line)
{
    if (line.empty())
        return;
    // adds key to hashtable if # is first char
    if (line.front() == '#')
    {
        // line parsing
        int spaceChar = line.find(" ");
        string tagName = line.substr(1, spaceChar - 1);
        int tagCount;
        istringstream(line.substr(spaceChar + 1)) >> tagCount;
        addToHashTable(tagName, tagCount);
    }
    // prints output
    else if (line.front() >= 0 || line.front() <= 39)
    {
        int num;
        istringstream(line) >> num;
        printToOutput(num);
    }
}

void App::addToHashTable(string tagName, int tagCount)
{
    // stores key and node in hash table; inserts or increases key if it exists
    unordered_map<string, FHNode *>::const_iterator citer = hashTable.find(tagName);
    if (citer != hashTable.end())
    {
        fHeap.IncreaseKey(citer->second, tagCount);
    }
    else
    {
        FHNode *newNode = fHeap.Insert(tagCount, tagName);
        hashTable.insert(make_pair(tagName, newNode));
    }
}

void App::printToOutput(int num)
{
    // prints the removed nodes from heap to prefered output
    vector<FHNode *> tagNodes;
    tagNodes.clear();
    for (int i = 0; i < num; i++)
    {
        FHNode *n = fHeap.RemoveMax();
        if (n != NULL)
            tagNodes.push_back(n);
    }
    stringstream concTagStream;
    concTagStream.clear();
    unordered_map<string, FHNode *>::const_iterator citer;
    concTagStream << tagNodes[0]->tagName;
    for (int i = 1; i < num; i++)
    {
        concTagStream << "," << tagNodes[i]->tagName;
    }
    if (ofs.is_open())
    {
        ofs << concTagStream.str() << "\n";
    }
    else
    {
        cout << concTagStream.str() << "\n";
    }
    // pushes back the removed nodes to the heap
    for (int i = 0; i < num; i++)
    {
        citer = hashTable.find(tagNodes[i]->tagName);
        if (citer != hashTable.end())
        {
            FHNode *newNode = fHeap.Insert(citer->second->count, citer->first);
            hashTable.erase(citer->first);
            hashTable.insert(make_pair(citer->first, newNode));
        }
    }
    tagNodes.clear();
}