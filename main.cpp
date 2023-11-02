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
#include <unordered_map> // fuck outta here with that binary tree GARBAGE. TRASH. WHERES YOUR HASH FUNCTION NOW?!?!?

// yeah so i used a map with strings as a meme for quick speedups :)

#define MAP_INSTRUCTION(instruction) { #instruction, Instruction::instruction }
enum Instruction {
	INVALID = -1,

	ADD,
	SUB,

	NOT,
	AND,
	ORR,
	XOR,

	LSL,
	LSR,
	ASR,

	ADDS,
	SUBS,
	ANDS,
	XORS,
	NOTS,
	ORRS
};

static std::unordered_map<std::string, Instruction> instruction_map = {
	MAP_INSTRUCTION(ADD),
	MAP_INSTRUCTION(SUB),

	MAP_INSTRUCTION(NOT),
	MAP_INSTRUCTION(AND),
	MAP_INSTRUCTION(ORR),
	MAP_INSTRUCTION(XOR),

	MAP_INSTRUCTION(LSL),
	MAP_INSTRUCTION(LSR),
	MAP_INSTRUCTION(ASR),

	// aliases signed shifts to respective unsigned shifts
	{"LSLS", Instruction::LSL},
	{"LSRS", Instruction::LSR},
	{"ASRS", Instruction::ASR},

	MAP_INSTRUCTION(ADDS),
	MAP_INSTRUCTION(SUBS),
	MAP_INSTRUCTION(ANDS),
	MAP_INSTRUCTION(XORS),
	MAP_INSTRUCTION(NOTS),
	MAP_INSTRUCTION(ORRS)
};

int main(int argc, char* argv[]){
	if(argc > 1)
	{
		std::ifstream stream(argv[1]);
		if(!stream.good()) { std::cout << "Invalid file, fuckoff" << std::endl; return 1; };

		std::string current_line = "";
		while(std::getline(stream, current_line)) {
			uint32_t op1 = 0;
			uint32_t op2 = 0;

			std::stringstream string_stream(current_line);
			std::string current_instruction_mneumonic = "";
			string_stream >> current_instruction_mneumonic >> std::hex >> op1 >> std::hex >> op2;

			Instruction current_instruction = Instruction::INVALID;
			if(instruction_map.count(current_instruction_mneumonic) != 0)
				current_instruction = instruction_map[current_instruction_mneumonic];

			// case hell begins :)
			switch(current_instruction) {
				case Instruction::ADD: {
					uint64_t result = static_cast<uint64_t>(op1) + static_cast<uint64_t>(op2);
					if(result > UINT32_MAX) {
						std::cout << "Overflow Encountered..." << std::endl;
					}
                	result = static_cast<uint32_t>(result);
					std::cout << current_instruction_mneumonic << "\t"<< std::hex << op1 << ", " << std::hex << op2 << " = " << std::hex << result << std::endl;

					break;
				}
				case Instruction::SUB: {
					uint64_t result = static_cast<uint64_t>(op1) - static_cast<uint64_t>(op2);
					std::cout << current_instruction_mneumonic << "\t"<< std::hex << op1 << ", " << std::hex << op2 << " = " << std::hex << result << std::endl;
					break;
				}
				case Instruction::NOT: {
					uint32_t result = ~op1;
					std::cout << current_instruction_mneumonic << "\t"<< std::hex << op1 << " = " << std::hex << result << std::endl;
					break;
				}
				case Instruction::AND: {
					uint32_t result = op1 & op2;
					std::cout << current_instruction_mneumonic << "\t"<< std::hex << op1 << ", " << std::hex << op2 << " = " << std::hex << result << std::endl;
					break;
				}
				case Instruction::ORR: {
					uint32_t result = op1 | op2;
					std::cout << current_instruction_mneumonic << "\t"<< std::hex << op1 << ", " << std::hex << op2 << " = " << std::hex << result << std::endl;
					break;
				}
				case Instruction::XOR: {
					uint32_t result = op1 ^ op2;
					std::cout << current_instruction_mneumonic << "\t"<< std::hex << op1 << ", " << std::hex << op2 << " = " << std::hex << result << std::endl;
					break;
				}
				case Instruction::LSL: {
					uint32_t result = op1 << op2;
					std::cout << current_instruction_mneumonic << "\t"<< std::hex << op1 << ", " << std::hex << op2 << " = " << std::hex << result << std::endl;
					break;
				}
				case Instruction::LSR: {
					uint32_t result = op1 >> op2;
					std::cout << current_instruction_mneumonic << "\t"<< std::hex << op1 << ", " << std::hex << op2 << " = " << std::hex << result << std::endl;
					break;
				}
				case Instruction::ASR: {
					int32_t result = static_cast<int32_t>(op1) >> static_cast<int32_t>(op2);
					std::cout << current_instruction_mneumonic << "\t"<< std::hex << op1 << ", " << std::hex << op2 << " = " << std::hex << result << std::endl;
					break;
				}
				case Instruction::ADDS: {
					int64_t result = static_cast<int64_t>(op1) + static_cast<int64_t>(op2);
					if(result > INT32_MAX) {
						std::cout << "Overflow Encountered..." << std::endl;
					}
                	result = static_cast<int32_t>(result);
					std::cout << current_instruction_mneumonic << "\t"<< std::hex << op1 << ", " << std::hex << op2 << " = " << std::hex << result << std::endl;
					break;
				}
				case Instruction::SUBS: {
					int64_t result = static_cast<int64_t>(op1) - static_cast<int64_t>(op2);
					std::cout << current_instruction_mneumonic << "\t"<< std::hex << op1 << ", " << std::hex << op2 << " = " << std::hex << result << std::endl;
					break;
				}
				case Instruction::ANDS: {
					int32_t result = static_cast<int32_t>(op1) & static_cast<int32_t>(op2);
					std::cout << current_instruction_mneumonic << "\t"<< std::hex << op1 << ", " << std::hex << op2 << " = " << std::hex << result << std::endl;
					break;
				}
				case Instruction::XORS: {
					int32_t result = static_cast<int32_t>(op1) ^ static_cast<int32_t>(op2);
					std::cout << current_instruction_mneumonic << "\t"<< std::hex << op1 << ", " << std::hex << op2 << " = " << std::hex << result << std::endl;
					break;
				}
				case Instruction::NOTS: {
					int32_t result = ~static_cast<int32_t>(op1);
					std::cout << current_instruction_mneumonic << "\t"<< std::hex << op1 << " = " << std::hex << result << std::endl;
					break;
				}
				case Instruction::ORRS: {
					int32_t result = static_cast<int32_t>(op1) | static_cast<int32_t>(op2);
					std::cout << current_instruction_mneumonic << "\t"<< std::hex << op1 << ", " << std::hex << op2 << " = " << std::hex << result << std::endl;
					break;
				}

				default: std::cout << "Invalid Instruction: " << current_instruction_mneumonic << std::endl; break;
			}
		}
	}
	else {
		std::cout << "file invalid, check parameters. try using assembler listop" << std::endl;
		return 1;
	}


   // std::cout << result << std::endl;
	return 0;
}