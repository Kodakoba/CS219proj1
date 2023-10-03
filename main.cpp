/*
>By Lloyd "Koda" C
> 10/2/23 - idk the year 0
>purpose: visUAL but bad
do math from a file, with more operations coming soon(tm)
ADD 0xHEX
and detect overflow conditions
*/
#include <iostream>
#include <fstream>
#include <sstream>
//#include <format> //can't include because ubuntu is dogshit


uint32_t addBS(int,int,int*);


int main(int argc, char* argv[]){

    //int bullshit[100]; // what in the duck
    std::string opergrand; //totally
    uint32_t op1, op2, result; //both operands coming up
    //int filePtr = 0;
    int overflow = 0; //this is hell
    //int numberOfEnt = 0;


    if (argc > 1)
    {
      std::ifstream stream(argv[1]);
      std::string current_line = "";
    while(std::getline(stream, current_line)){
            std::stringstream string_stream(current_line);
            std::string current_instruction = "";
            string_stream >> current_instruction >> std::hex >> op1 >> std::hex >> op2;
            if(current_instruction == "ADD") {
            uint64_t result = static_cast<uint64_t>(op1) + static_cast<uint64_t>(op2);
            std::cout << current_instruction << "  "<< std::hex << op1 << "  " << std::hex << op2 << "  : " << std::hex << result << std::endl;
            if(result > UINT32_MAX) {
                std::cout << "ADD Instruction Overflow. result is greater than " << UINT32_MAX << std::endl;
                continue;
            }
            
           // numberOfEnt++;
        }
    }
    }
    else{
            std::cout << "file invalid, check parameters. try using assembler listop" << std::endl;
            return 1;
    }


   // std::cout << result << std::endl;
    return 0;
}

uint32_t addBS (int opc1, int opc2, int* res){
    return 1;
} //i love unused functions
