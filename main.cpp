#include<iostream>
#include<string>
#include<stdexcept>
#include "src/translator/translator.h"

int main(int argc,char *argv[]){
    try
    {   //invalid_input handler
        if(argc!=3){
            throw std::invalid_argument("Arguments count must equal to 3");
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return 0;
    }
    std::string inputFilePath = argv[1];
    std::string outputFilePath = argv[2];
    Translator translator(inputFilePath,outputFilePath);
    std::string res = translator.translate();
    
    std::cout<<res<<std::endl;
}