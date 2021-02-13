#include <iostream>
#include "App.h"

using namespace std;

int main(int argc, char *argv[])
{
    char *inputFilename, *outputFilename;
    if (argv[1] == NULL)
    {
        cout << "No input filename is entered. Exiting...\n";
        exit(1);
    }
    inputFilename = argv[1];
    App *app;
    if(argv[2]!=NULL){
        outputFilename = argv[2];
        app = new App(inputFilename, outputFilename);
    }
    else
    {
        app = new App(inputFilename);
    }
    app->GetTagsFromInputFile();
    
    delete app;

    return 0;
}