#include "translator.h"
#include<fstream>
Translator::Translator(
    const std::string& inputFilePath,
    const std::string& outputFilePath
)
    :   dispensor(inputFilePath),
        inputFilePath(inputFilePath),
        outputFilePath(outputFilePath),
        symbolTable(),
        mcBuffer(""),
        encoder(symbolTable)
    {
    createSymbolTable();
}

std::string Translator::translate(){
    while(dispensor.hasNext()){
        std::string instruction = dispensor.nextInstruction();
        std::stringstream ss(instruction);
        std::string token;
        int i=0;
        while(getline(ss, token, '\t')){
            buffer[i++] = token;
        }

        std::string res = encoder.encodeInstruction(buffer,symbolTable,dispensor.getCurrentLine());
        int32_t machineCode = static_cast<int32_t>(static_cast<uint32_t>(std::bitset<32>(res).to_ulong()));
        mcBuffer += std::to_string(machineCode) + '\n';
        resetBuffer();
    }
    exportAsMachineCodeFile(inputFilePath,outputFilePath);
    return "Translation completed. Total instruction count: " + std::to_string(dispensor.getCurrentLine());
}

void Translator::resetBuffer(){
	for(int i=0;i<6;i++)buffer[i]="";
}

void Translator::createSymbolTable(){
    while(dispensor.hasNext()){
        std::string instruction = dispensor.nextInstruction();
        std::stringstream ss(instruction);
        std::string token;
        int i=0;
        while(getline(ss, token, '\t')){
            buffer[i++] = token;
        }
        if(buffer[0]!=""){
            symbolTable[buffer[0]] = dispensor.getCurrentLine()-1;
            //std::cout<< "Symbol Table Entry: " << buffer[0] << " -> Address = " << symbolTable[buffer[0]] << std::endl;
        }
        resetBuffer();
    }
    dispensor.reset();
}

void Translator::showSymbolTable(){
    for(const auto& item : symbolTable){
        std::cout<< item.first<< " -> " << item.second << std::endl;
    }
}

void Translator::exportAsMachineCodeFile(const std::string& inputFilePath, const std::string& outputFilePath){
    size_t lastSlash = inputFilePath.find_last_of("/\\");
    std::string fileName = inputFilePath.substr(lastSlash + 1);
    size_t dot = fileName.find_last_of(".");
    std::string baseName = fileName.substr(0, dot);
    std::string outputPath =  outputFilePath + "/" + baseName + ".mc";

    std::ofstream machineCodeFile(outputPath);
    if(!machineCodeFile.is_open()){
        throw std::runtime_error("cannot open output file: "+outputPath);
    }
    machineCodeFile << mcBuffer;
    std::cout<<mcBuffer;
    machineCodeFile.close();
}   