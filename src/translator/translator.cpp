#include "translator.h"
#include<fstream>
#include<regex>
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
        if (!instruction.empty() && isspace(instruction[0])){
            buffer[i++] = "";
        }
        while(ss >> token){
            buffer[i++]=token;
            if(i==5)break;
        }

        std::string res = encoder.encodeInstruction(buffer,dispensor.getCurrentLine());
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
        std::string symbol;
        if(!instruction.empty() && isspace(instruction[0])){ // not empty, start with whitespace
            continue;
        }
        ss >> symbol;
        validateSymbol(symbol);
        symbolTable[symbol] = dispensor.getCurrentLine()-1;
        //std::cout<< "Symbol Table Entry: " << buffer[0] << " -> Address = " << symbolTable[buffer[0]] << std::endl;
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

bool Translator::validateSymbol(std::string symbol){
    if(symbolTable.count(symbol)>0){
        throw std::runtime_error("INVALID LABEL : duplicated label ' "+symbol+" ' found. At line # "+std::to_string(dispensor.getCurrentLine())+'\n');
    }
    std::regex symbolRegex("^[A-Za-z][A-Za-z0-9]{0,5}$");// first, alphabet. 5 left , alphabel or number 
    if(symbol.size()>6){
        throw std::runtime_error("INVALID LABEL : Label must have not more than 6 characters. At line # "+std::to_string(dispensor.getCurrentLine())+'\n');
    }else if(!std::regex_match(symbol,symbolRegex)){
        throw std::runtime_error("INVALID LABEL : Label format is not match. At line # "+std::to_string(dispensor.getCurrentLine())+'\n');
    }

    return true;
}

int Translator::getCurrentLine(){
    return dispensor.getCurrentLine();
}