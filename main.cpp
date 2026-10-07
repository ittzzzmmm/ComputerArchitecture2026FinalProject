#include<iostream>
#include<string>
#include<stdexcept>
#include "src/translator/translator.h"

int main(int argc,char *argv[]){
    Translator* translator = nullptr;
    try
    {   //invalid_input handler
        if(argc!=3){
            throw std::invalid_argument("Arguments count must equal to 3");
        }
        std::string inputFilePath = argv[1];
        std::string outputFilePath = argv[2];
        Translator t(inputFilePath,outputFilePath);
        translator = &t;
        std::string res = translator->translate();
        
        std::cout<<res<<std::endl;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what();
        if(translator != nullptr){
            std::cerr <<"At line # " << translator->getCurrentLine() <<  '\n';
        }
        return 1;
    }
    return 0;
}