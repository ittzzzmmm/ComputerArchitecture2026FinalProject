#ifndef ENCODER_H
#define ENCODER_H

#include <string>
#include<bitset>
#include<map>
#include<iostream>

class Encoder{
    private:
        std::map<std::string,int>& table;
    public:
        Encoder(std::map<std::string,int>& symbolTable);
        std::string encodeInstruction(const std::string buffer[],const std::map<std::string,int>& table,const int pc);
        std::string encodeRType(const std::string regA,std::string regB,std::string desReg,std::string opcode);
        std::string encodeIType(const std::string regA,std::string regB,std::string offset,std::string opcode,const int pc);
        std::string encodeJType(const std::string regA,std::string regB);
        std::string encodeOType(const std::string opcode);
        std::string encodeDotFill(const std::string value);

        //std::string decimalToBinaryStr(const std::string decimal,const unsigned long long bitLength);
        bool isNumber(const std::string str);
        void showSymbolTable();

};

#endif