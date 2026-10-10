// runSimulator - execution loop + decoding

/* instruction-level simulator */

#include "simulator.h"
#include <bitset>
#include <iostream>

using namespace std;

void printState(stateType *statePtr)
{
    int i;
    printf("\n@@@\nstate:\n");
    printf("\tpc %d\n", statePtr->pc);
    printf("\tmemory:\n");
    for (i = 0; i < statePtr->numMemory; i++)
    {
        printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
    }
    printf("\tregisters:\n");
    for (i = 0; i < NUMREGS; i++)
    {
        printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
    }
    printf("end state\n");
}

void runSimulator(stateType *statePtr)
{
    bool isHalted = false;

    // instructions counter
    int counter = 0;

    // initial pc
    statePtr->pc = 0;

    // initial registers
    for (int i = 0; i < NUMREGS; i++)
    {
        statePtr->reg[i] = 0;
    }

    while (!isHalted)
    {
        printState(statePtr);

        // 1. fetch state.mem
        int instruction = statePtr->mem[statePtr->pc];

        // 2. decode
        // Bits 24-22 opcode
        int opcode = (instruction >> 22) & 7;

        // 3. call (execute by bew)

        switch (opcode)
        {
        // add R-type
        case 0:
        {
            int regA = (instruction >> 19) & 7;
            int regB = (instruction >> 16) & 7;
            int destReg = instruction & 7;

            add(regA, regB, destReg, statePtr);
        }
        break;

        // nand R-type
        case 1:
        {
            int regA = (instruction >> 19) & 7;
            int regB = (instruction >> 16) & 7;
            int destReg = instruction & 7;

            nand(regA, regB, destReg, statePtr);
        }
        break;

        // lw I-type
        case 2:
        {
            int regA = (instruction >> 19) & 7;
            int regB = (instruction >> 16) & 7;
            int offsetField = instruction & 65535;

            lw(regA, offsetField, regB, statePtr);
        }
        break;

        // sw I-type
        case 3:
        {
            int regA = (instruction >> 19) & 7;
            int regB = (instruction >> 16) & 7;
            int offsetField = instruction & 65535;

            sw(regA, offsetField, regB, statePtr);
        }
        break;

        // beq I-type
        case 4:
        {
            int regA = (instruction >> 19) & 7;
            int regB = (instruction >> 16) & 7;
            int offsetField = instruction & 65535;

            beq(regA, offsetField, regB, statePtr);
        }
        break;

        // jalr J-Type
        case 5:
        {
            int regA = (instruction >> 19) & 7;
            int regB = (instruction >> 16) & 7;

            jalr(regA, regB, statePtr);
        }
        break;

        // halt O-type
        case 6:
        {
            isHalted = true;
            statePtr->pc++;
        }
        break;

        // noop O-type
        case 7:
        {
            statePtr->pc++;
        }
        break;
        }

        // 4. update counter (pc,reg) done in each function
        counter++;
    }

    if (isHalted)
    {
        std::cout << "machine halted\n";
        std::cout << "total of " << counter << " instructions executed\n";
        std::cout << "final state of machine:\n";

        printState(statePtr);
    }
}

void add(int regA, int regB, int destReg, stateType *statePtr)
{
    if (destReg != 0)
    {
        statePtr->reg[destReg] = statePtr->reg[regA] + statePtr->reg[regB];
    }
    statePtr->pc++;
}

void nand(int regA, int regB, int destReg, stateType *statePtr)
{
    if (destReg != 0)
    {
        statePtr->reg[destReg] = ~(statePtr->reg[regA] & statePtr->reg[regB]);
    }
    statePtr->pc++;
}

void lw(int regA, int offsetField, int regB, stateType *statePtr)
{
    if (regB != 0)
    {
        int offset = convertNum(offsetField);
        int address = statePtr->reg[regA] + offset;
        if (address < 0 || address >= NUMMEMORY)
        {
            cerr << "Error: Memory address out of bounds: " << address << endl;
            exit(1);
        }
        statePtr->reg[regB] = statePtr->mem[address];
    }
    statePtr->pc++;
}

void sw(int regA, int offsetField, int regB, stateType *statePtr)
{
    int offset = convertNum(offsetField);
    int address = statePtr->reg[regA] + offset;
    if (address < 0 || address >= NUMMEMORY)
    {
        cerr << "Error: Memory address out of bounds: " << address << endl;
        exit(1);
    }
    statePtr->mem[address] = statePtr->reg[regB];
    statePtr->pc++;
}

void beq(int regA, int offsetField, int regB, stateType *statePtr)
{
    if (statePtr->reg[regA] == statePtr->reg[regB])
    {
        int offset = convertNum(offsetField);
        int address = statePtr->pc + offset + 1;
        if (address < 0 || address >= NUMMEMORY)
        {
            cerr << "Error: Branch address out of bounds: " << address << endl;
            exit(1);
        }
        statePtr->pc = address;
    }
    else
    {
        statePtr->pc++;
    }
}

void jalr(int regA, int regB, stateType *statePtr)
{
    if (regB != 0)
    {
        statePtr->reg[regB] = statePtr->pc + 1;
    }
    if (regA == regB)
    {
        statePtr->pc++;
    }
    else
    {
        if (regA < 0 || regA >= NUMREGS)
        {
            cerr << "Error: Invalid register index for jalr: " << regA << endl;
            exit(1);
        }
        statePtr->pc = statePtr->reg[regA];
    }
}

int convertNum(int num)
{
    /* convert a 16-bit number into a 32-bit integer */
    if (num & (1 << 15))
    {
        num -= (1 << 16);
    }
    return (num);
}
