#include "encoder.h"

Encoder::Encoder(std::map<std::string,int>& symbolTable): table(symbolTable){
    
}

std::string Encoder::encodeInstruction(const std::string buffer[],const std::map<std::string,int>& table,const int pc){   
    //showSymbolTable(); 
    if(buffer[1]=="add" || buffer[1]=="nand"){
        return encodeRType(buffer[2],buffer[3],buffer[4],buffer[1]);
    }else if(buffer[1]=="lw" || buffer[1]=="sw" || buffer[1]=="beq"){
        return encodeIType(buffer[2],buffer[3],buffer[4],buffer[1],pc);
    }else if(buffer[1]=="jalr"){
        return encodeJType(buffer[2],buffer[3]);
    }else if(buffer[1]=="halt" || buffer[1]=="noop"){
        return encodeOType(buffer[1]);
    }else if(buffer[1]==".fill"){
        return encodeDotFill(buffer[2]);
    }else{
        throw std::runtime_error("INVALID INSTRUCTION : Instrunction ' "+ buffer[1]+" ' found. ");
    }
    return "";
}

std::string Encoder::encodeRType(const std::string regA,const std::string regB,const std::string desReg, const std::string instruction){
    std::string result = "0000000";
    if(instruction=="add"){
        result+="000";
    }else if(instruction=="nand"){
        result+="001";
    }else{

    }
    // register validation , range int 0 - 7 inclusive.
    validateRegister(regA);
    validateRegister(regB);
    validateRegister(desReg);
    
    result+= std::bitset<3>(std::stoi(regA)).to_string();
    result+= std::bitset<3>(std::stoi(regB)).to_string();
    result+= "0000000000000";
    result+= std::bitset<3>(std::stoi(desReg)).to_string();
    return result;
}

std::string Encoder::encodeIType(const std::string regA,const std::string regB,const std::string offset,const std::string instruction,const int pc){
    std::string result = "0000000";
    if(instruction=="lw"){
        result+="010";
    }else if(instruction=="sw"){
        result+="011";
    }else if(instruction=="beq"){
        result+="100";
        // register validation , range int 0 - 7 inclusive.
        validateRegister(regA);
        validateRegister(regB);

        result+= std::bitset<3>(std::stoi(regA)).to_string();
        result+= std::bitset<3>(std::stoi(regB)).to_string();
        if(isNumber(offset)){
            int offsetInt = std::stoi(offset);
            if(offsetInt < -32768 ||  offsetInt > 32767){
                throw std::runtime_error("INVALID OFFSET : Offset must be in range [ -32768 , 32767 ]. ");
            }
            result+= std::bitset<16>(std::stoi(offset)).to_string();
        }else{
            auto it = table.find(offset);
            if(it == table.end()){
                throw std::runtime_error("UNDEFINED SYMBOL : Undefined Symbol ' "+offset+" ' not found. ");
            }
            result+= std::bitset<16>(table[offset]-pc).to_string();
        }
        return result;
    }else{

    }

    // register validation , range int 0 - 7 inclusive.
    validateRegister(regA);
    validateRegister(regB);

    result+= std::bitset<3>(std::stoi(regA)).to_string();
    result+= std::bitset<3>(std::stoi(regB)).to_string();
    if(isNumber(offset)){
        int offsetInt = std::stoi(offset);
        //offset validation , range int [ -32768 , 32767 ].
        if(offsetInt < -32768 ||  offsetInt > 32767){
            throw std::runtime_error("INVALID OFFSET : Offset must be in range [ -32768 , 32767 ]. ");
        }
        result+= std::bitset<16>(std::stoi(offset)).to_string();
    }else{
        auto it = table.find(offset);
        if(it == table.end()){
            throw std::runtime_error("UNDEFINED SYMBOL : Undefined Symbol ' "+offset+" ' not found.");
        }
        result+= std::bitset<16>(table[offset]).to_string();
    }
    return result;
}

std::string Encoder::encodeJType(const std::string regA,const std::string regB){
    std::string result = "0000000";
    result+="101";
    // register validation , range int 0 - 7 inclusive.
    validateRegister(regA);
    validateRegister(regB);
    result+= std::bitset<3>(std::stoi(regA)).to_string();
    result+= std::bitset<3>(std::stoi(regB)).to_string();
    result+= "0000000000000000";

    return result;
}

std::string Encoder::encodeOType(const std::string instruction){
    std::string result = "0000000";
    if(instruction=="halt"){
        result+="110";
    }else if(instruction=="noop"){
        result+="111";
    }
    result+="0000000000000000000000";

    return result;
}

std::string Encoder::encodeDotFill(std::string value){
    if(isNumber(value)){
        int valueInt = std::stoi(value);
        if(valueInt < -32768 || valueInt > 32767){
            throw std::runtime_error("INVALID VALUE : .fill must be in range [ -32768 , 32767 ]. ");
        }
        return std::bitset<32>(std::stoi(value)).to_string();
    }else{
        auto it = table.find(value);
        if(it == table.end()){
            throw std::runtime_error("UNDEFINED SYMBOL : Undefined Symbol ' "+value+" ' not found. ");
        }
        return std::bitset<32>(table[value]).to_string();
    }
}

// helper functions

bool Encoder::isNumber(const std::string str){
    try {
        size_t pos;
        std::stoi(str, &pos);
        return pos == str.length();
    } catch (...) {
        return false;
    }
}

void Encoder::showSymbolTable(){
    for(const auto& item : table){
        std::cout<< item.first<< " -> " << item.second << std::endl;
    }
}

bool Encoder::validateRegister(std::string regStr){

    if(!isNumber(regStr)){
        throw std::runtime_error("INVALID REGISTER : Register must be an integer. ");
    }
    if(std::stoi(regStr) < 0 || std::stoi(regStr) > 7){
        throw std::runtime_error("INVALID REGISTER : Register must be in range [0 , 7]. ");
    }
    return true;
}
