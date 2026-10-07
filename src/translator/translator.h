#ifndef TRANSLATOR_H
#define TRANSLATOR_H

#include "dispensor/dispensor.h"
#include "encoder/encoder.h"
#include<sstream>
#include<map>


class Translator{
    private:
        Dispensor dispensor;
        std::map<std::string,int> symbolTable;
        Encoder encoder;
        std::string mcBuffer;
        std::string inputFilePath;
        std::string outputFilePath;
        std::string buffer[6]; 
    public:
        Translator(const std::string& inputFileName, const std::string& outputFilePath);
        std::string translate();
        void createSymbolTable();
        void resetBuffer();
        void showSymbolTable();
        void exportAsMachineCodeFile(const std::string& inputFilePath,const std::string& outputFilePath);
        int getCurrentLine();
        bool validateSymbol(std::string symbol);

};

#endif

