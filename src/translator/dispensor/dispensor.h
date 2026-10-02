#ifndef DISPENSOR_H
#define DISPENSOR_H

#include<fstream>
#include<string>
   
class Dispensor{
    private: 
        //instance
        std::ifstream assemblyFile;
    public:
        //constructor function
        Dispensor(const std::string& filePath);
        bool hasNext();
        std::string nextInstruction();
        void reset();
};

#endif