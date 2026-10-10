#include <iostream>
#include <string>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8       /* number of machine registers */
#define MAXLINELENGTH 1000

typedef struct stateStruct
{
    int pc;
    int mem[NUMMEMORY];
    int reg[NUMREGS];
    int numMemory;
} stateType;

void printState(stateType *);

void runSimulator(stateType *);

void add(int regA, int regB, int destReg, stateType *statePtr);

void nand(int regA, int regB, int destReg, stateType *statePtr);

void lw(int regA, int offsetField, int regB, stateType *statePtr);

void sw(int regA, int offsetField, int regB, stateType *statePtr);

void beq(int regA, int offsetField, int regB, stateType *statePtr);

void jalr(int regA, int regB, stateType *statePtr);

int convertNum(int num);