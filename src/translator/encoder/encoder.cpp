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
        result+= std::bitset<3>(std::stoi(regA)).to_string();
        result+= std::bitset<3>(std::stoi(regB)).to_string();
        if(isNumber(offset)){
            result+= std::bitset<16>(std::stoi(offset)).to_string();
        }else{
            result+= std::bitset<16>(table[offset]-pc).to_string();
        }
        return result;
    }else{

    }

    result+= std::bitset<3>(std::stoi(regA)).to_string();
    result+= std::bitset<3>(std::stoi(regB)).to_string();
    if(isNumber(offset)){
        result+= std::bitset<16>(std::stoi(offset)).to_string();
    }else{
        result+= std::bitset<16>(table[offset]).to_string();
    }
    return result;
}

std::string Encoder::encodeJType(const std::string regA,const std::string regB){
    std::string result = "0000000";
    result+="101";
    result+= std::bitset<3>(std::stoi(regA)).to_string();
    result+= std::bitset<3>(std::stoi(regB)).to_string();

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
        return std::bitset<32>(std::stoi(value)).to_string();
    }else{
        return std::bitset<32>(table[value]).to_string();
    }
}

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
// std::string Encoder::decimalToBinaryStr(const std::string decimal,const unsigned long long bitLength){
//     int value = std::stoi(decimal);
//     return std::bitset<3>(value).to_string();
// }

bool Encoder::validateRegister(std::string regStr){
    if(!isNumber(regStr)){
        throw std::runtime_error("INVALID REGISTER : Register must be an integer. ");
    }
    if(std::stoi(regStr) < 0 || std::stoi(regStr) > 7){
        throw std::runtime_error("INVALID REGISTER : Register must be in range [0 , 7]. ");
    }
    return true;
}
