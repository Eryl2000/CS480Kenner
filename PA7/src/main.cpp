#include <iostream>
#include <string.h>
#include "engine.h"


int main(int argc, char **argv){
    if(argc != 5){
        printf("Usage\n\n  -v <vertexShaderFilename>\n  -f <fragmentShaderFilename>\n\nSpecify vertex and fragment shader files.\n");
        exit(1);
    }

    //Parse command line arguments for the specified vertex and fragment shader filepaths
    std::string vertexShader;
    std::string fragmentShader;
    std::string objectPath;
    for(int i = 1; i < argc; i += 2){
        if(std::string(argv[i]) == "-v"){
            vertexShader = std::string(argv[i + 1]);
        } else if(std::string(argv[i]) == "-f"){
            fragmentShader = std::string(argv[i + 1]);
        }
    }

    // Start an engine and run it then cleanup after
    Engine *engine = new Engine("Tutorial Window Name", 800, 600);
    if(!engine->Initialize(vertexShader, fragmentShader)){
        printf("The engine failed to start.\n");
        delete engine;
        engine = NULL;
        return 1;
    }
    engine->Run();
    delete engine;
    engine = NULL;
    return 0;
}
