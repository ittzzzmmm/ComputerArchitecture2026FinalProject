#include "dispensor.h"

Dispensor::Dispensor(const std::string& filePath){
    assemblyFile.open(filePath);
    if(!assemblyFile.is_open()){
        throw std::runtime_error("Cannot open assembly file : "+ filePath);
    }
}
bool Dispensor::hasNext(){
    return !assemblyFile.eof();
}

std::string Dispensor::nextInstruction(){
    std::string instruction;
    if (std::getline(assemblyFile, instruction)) {
        currentLine++;
        return instruction;
    }
    return "";
}

int Dispensor::getCurrentLine() {
    return currentLine;
}


void Dispensor::reset(){
    assemblyFile.clear(); // Clear EOF flag
    assemblyFile.seekg(0); // Move pointer to beginning of file
    currentLine = 0; // Reset current line number
}
