#ifndef DISPENSOR_H
#define DISPENSOR_H

#include<fstream>
#include<string>
   
class Dispensor{
    private: 
        //instance
        std::ifstream assemblyFile;
        int currentLine=0;
    public:
        //constructor function
        Dispensor(const std::string& filePath);
        bool hasNext();
        std::string nextInstruction();
        int getCurrentLine();
        void reset();
};

#endif