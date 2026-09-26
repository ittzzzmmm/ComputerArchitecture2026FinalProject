#include "dispensor.h"

Dispensor::Dispensor(const std::string& file){
    assemblyFile.open(file);
}
bool Dispensor::hasNext(){
    return !assemblyFile.eof();
}

std::string Dispensor::nextInstruction(){
    std::string instruction;
    if (std::getline(assemblyFile, instruction)) {
        return instruction;
    }
    return "";
}
