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
#include <iomanip>
#include <unordered_map>
#include <cstdint>
#include <algorithm>

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
	ORRS,

	MOV
};

static std::unordered_map<std::string, Instruction> instruction_map = { //defining params for said baby class
	{ "ADD", Instruction::ADD },
	{ "SUB", Instruction::SUB },

	{ "NOT", Instruction::NOT },
	{ "AND", Instruction::AND },
	{ "ORR", Instruction::ORR },
	{ "XOR", Instruction::XOR },

	{ "LSL", Instruction::LSL },
	{ "LSR", Instruction::LSR },
	{ "ASR", Instruction::ASR },

	// aliases signed shifts to respective unsigned shifts
	{ "LSLS", Instruction::LSL },
	{ "LSRS", Instruction::LSR },
	{ "ASRS", Instruction::ASR },

	{ "ADDS", Instruction::ADDS },
	{ "SUBS", Instruction::SUBS },
	{ "ANDS", Instruction::ANDS },
	{ "XORS", Instruction::XORS },
	{ "NOTS", Instruction::NOTS },
	{ "ORRS", Instruction::ORRS },
	{ "MOV", Instruction::MOV }
};

/*
	"ADD r1, r2" -> Operation(Instruction::ADD, 1, 2)
*/
static const int OPERATION_MAX_ARGUMENTS = 3;
static const int MAX_REGISTERS = 8;

enum ValueType {
	NONE,
	REGISTER,
	NUMBER,
};

struct Operation {
	Instruction instruction = Instruction::INVALID;
	std::string instruction_name = ""; // Really just here to print it out nicely :)

	// Contains either the raw value (-> argTypes[i] == NUMBER), or the register number (-> argTypes[i] == REGISTER)
	int64_t arguments[OPERATION_MAX_ARGUMENTS] = { 0 };
	ValueType argTypes[OPERATION_MAX_ARGUMENTS] = { NONE };
};

std::string tolower(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::tolower(c); }); //makes luaU look shrimple
	return s;
}

std::string toupper(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(), [](unsigned char c) { return std::toupper(c); });
	return s;
}

class MachineState {
public:
	void parseOperation(std::string line) {
		std::istringstream sstream(tolower(line));
		std::string instruction_name = "";
		sstream >> instruction_name; // Parse out the line into an instruction name (to convert to the enum)

        instruction_name = toupper(instruction_name);

		// std::map contains is C++20
        // if i roll a 12 I fail my classes
        if (instruction_map.count(instruction_name) == 0) {
            throw std::out_of_range("No such instruction: " + instruction_name);
        }

		// Set the `instruction` in the operator
		op.instruction = instruction_map.at(instruction_name);
		op.instruction_name = instruction_name;

		// Parse the arguments
		for (int i = 0; i < OPERATION_MAX_ARGUMENTS; i++) {
            std::string regStr = "";
			op.arguments[i] = 0;

			if (sstream >> regStr) { // Extract a string ("R5") into regStr
				int64_t argValue = 0;
				ValueType type = NONE;

				if (regStr[0] == 'R' || regStr[0] == 'r') {
					// Extract the number from the registry string ("R5" -> 5)
					argValue = atoi(regStr.substr(1).c_str());

					type = REGISTER;
				} else if (regStr[0] == '#') {
					// Extract the hex value
					std::stringstream ss(regStr.substr(1)); // bleh, second stringstream
					ss << std::hex;
					ss >> argValue;

					type = NUMBER; //was it too much to ask for consistency in listop?
				}

				op.arguments[i] = argValue;
				op.argTypes[i] = type;
			}
		}
	};


	int64_t getValue(int num) {
		if (op.argTypes[num] == REGISTER) {
			// Value #num is a register number; pull out the value from the register
			if (num >= MAX_REGISTERS) {
				throw std::out_of_range("Register #" + std::to_string(num) + " out of range!");
			}

			return this->registers[op.arguments[num]];
		}

		if (op.argTypes[num] == NUMBER) {
			// Value #num is a raw value; just return that
			return op.arguments[num];
		}

		// The value #num was simply not given; getting it is an error...
		throw std::out_of_range("Value #" + std::to_string(num) + " was not defined!");
	}

	void storeValue(int regNum, int64_t val) {
		if (regNum >= MAX_REGISTERS) {
			throw std::out_of_range("Register #" + std::to_string(regNum) + " out of range!");
		}

		this->registers[regNum] = val;
	}

	int getRegisterNum(int num) {
		if (op.argTypes[num] == REGISTER) {
			// Value #num is a register number; pull out the value from the register
			if (num >= MAX_REGISTERS) {
				throw std::out_of_range("Register #" + std::to_string(num) + " out of range!");
			}

			return op.arguments[num];
		}

		throw std::out_of_range("Value #" + std::to_string(num) + " is not a register!");
	}

	void printRegisters() {
		const int REG_PER_LINE = 4;
		int i = 0;
		for (; i < MAX_REGISTERS; i++) {
			std::cout << "R" << i << ": " << std::hex << std::setfill('0') << std::setw(8) << std::right << registers[i];
			if ((i + 1) % REG_PER_LINE == 0) {
				std::cout << std::endl;
			} else {
				std::cout << " ";
			}
		}

		if (i % REG_PER_LINE != 0) {
		  std::cout << std::endl;
		}
	}
public:
	Operation op = Operation{ .instruction = Instruction::INVALID };
	int64_t registers[MAX_REGISTERS] = { 0 };
};

int calcFlag(char flag, int res) {
	int flagsend = 0;
	uint32_t zeroFr = 0;
	uint32_t bullshit = res;

	if(flag == 'N'){
		if(res > UINT32_MAX / 2){
			flagsend = 1;
		}
		else {
			flagsend = 0;
		}
	}
	/*else if(flag = 'C'){
		if(res < state.getValue(1) || result < state.getValue(2)){ //this depends on the class so not happening here lol
			flagsend = 1;
		}
		else{
			flagsend = 0; 
		}
	}*/
	else if(flag == 'Z'){
		if(bullshit = zeroFr){
			std::cout << "i think zero is overrated" << std::endl;
			flagsend = 1;
		}
		else{
			flagsend = 0;
		}
	}
	else if (flag == 'V'){ //why are you my least favorite flag BECAUSE YOU NEVER FUCKING WORK
		//std::cout << "i think overflows are overrated" << std::endl;
		if(res > INT32_MAX || res < 0) {
			flagsend = 1;
			std::cout << "overflow here" << std::endl;
		}
		else{
			flagsend = 0;
		}
	}
	return flagsend;
}

int main(int argc, char* argv[]) {
	if (argc <= 1) {
		std::cout << "Not enough arguments; no file supplied." << std::endl;
		return 1;
	}

	std::cout << "the blutuf deuaice is weady to paiw" << std::endl;

	static MachineState state = {};


	std::ifstream stream(argv[1]);
	if(!stream.good()) { // good file or not
		std::cout << "Couldn't open file: " << argv[1] << std::endl;
		return 1;
	};

	std::cout << "the blutuf deuaice has connectedey sucshessfulay" << std::endl; //look for btle device
	int neg = 0, zer = 0, car = 0, vov = 0;
	std::string current_line = "";
	while(std::getline(stream, current_line)) {
        state.parseOperation(current_line);

		Instruction current_instruction = state.op.instruction;
		uint32_t op1 = 0, op2 = 0;
		// case hell begins :)
		switch(current_instruction) {
			case Instruction::ADD: {
				// result = reg, op1 = base added to, op2 being what is adding :5head:
				int64_t result  = state.getValue(1) + state.getValue(2);
				if(result > UINT32_MAX) {

					std::cout << "+C" << std::endl;
				}
				state.storeValue(state.getRegisterNum(0), result);
				std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << " = " << std::hex << result << std::endl;

				break;
			}
			case Instruction::SUB: {
				uint64_t result = state.getValue(1) - state.getValue(2);
				state.storeValue(state.getRegisterNum(0), result);
				std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << " = " << std::hex << result << std::endl;
				break;
			}
			case Instruction::NOT: {
				uint32_t result = ~state.getValue(1);
				state.storeValue(state.getRegisterNum(0), result);
				std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << std::endl;
				break;
			}
			case Instruction::AND: {
				uint32_t result = state.getValue(0) & state.getValue(1);
				state.storeValue(state.getRegisterNum(0), result);
				std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << std::endl;
				break;
			}
			case Instruction::ORR: {
				uint32_t result = state.getValue(1) | state.getValue(2);
				state.storeValue(state.getRegisterNum(0), result);
				//std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << " = " << std::hex << result << std::endl;
				std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << std::endl;
				break;
			}
			case Instruction::XOR: {
				uint32_t result = state.getValue(1) ^ state.getValue(2);
				state.storeValue(state.getRegisterNum(0), result);
				//std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << " = " << std::hex << result << std::endl;
				std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << std::endl;
				break;
			}
			case Instruction::LSL: {
				uint32_t result = state.getValue(1) << state.getValue(2);
				state.storeValue(state.getRegisterNum(0), result);
				//std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << " = " << std::hex << result << std::endl;
				std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << std::endl;
				neg = calcFlag('N',result);
				zer = calcFlag('Z',result);
				if(result < state.getValue(1) || result < state.getValue(2)){
					car = 1;
				}
				else{
					car = 0;
				}
				// gonna eat this vov = calcFlag('V',result);
				if(result < state.getValue(1) + state.getValue(2) || result < state.getValue(2) || result > UINT32_MAX){ //what the fuck
					vov = 1;
				}
				else{
					vov = 0;
				}
				break;
			}
			case Instruction::LSR: {
				uint32_t result = state.getValue(1) >> state.getValue(2);
				state.storeValue(state.getRegisterNum(0), result);
				//std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << " = " << std::hex << result << std::endl;
				std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << std::endl;
				neg = calcFlag('N',result);
				zer = calcFlag('Z',result);
				if(result < state.getValue(1) || result < state.getValue(2)){
					car = 1;
				}
				else{
					car = 0;
				}
				//terrible vov = calcFlag('V',result);
				if(result < state.getValue(1) + state.getValue(2) || result > UINT32_MAX){
					vov = 1;
				}
				else{
					vov = 0;
				}
				break;
			}

			// i gave up here ngl
			case Instruction::ASR: {
				int32_t result = state.getValue(1) >> state.getValue(2);
				state.storeValue(state.getRegisterNum(0), result);
				//std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << " = " << std::hex << result << std::endl;
				std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << std::endl;
				neg = calcFlag('N',result);
				zer = calcFlag('Z',result);
				if(result < state.getValue(1) || result < state.getValue(2)){
					car = 1;
				}
				else{
					car = 0;
				}
				vov = calcFlag('V',result);
				break;
			}
			case Instruction::ADDS: {
				int64_t result = state.getValue(1) + state.getValue(2);
				if(result > INT32_MAX) {
					//std::cout << "overflow here" << std::endl;
					vov = 1; //already know it overflowed here so why do it twice
				}
				neg = calcFlag('N',result);
				zer = calcFlag('Z',result);
				if(result < state.getValue(1) || result < state.getValue(2)){
					car = 1;
				}
				else{
					car = 0;
				}
				vov = calcFlag('V',result);//in this case we know it's not going to be 1 because we already checked for an overflow.
				//std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << " = " << std::hex << result << std::endl;
				std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << std::endl;
				state.storeValue(state.getRegisterNum(0), result);
				//state.printRegisters();
				break;
			}
			case Instruction::SUBS: {
				int64_t result = state.getValue(1) - state.getValue(2);
				state.storeValue(state.getRegisterNum(0), result);
				neg = calcFlag('N',result);
				zer = calcFlag('Z',result);
				if(result < state.getValue(1) || result < state.getValue(2)){
					car = 1;
				}
				else{
					car = 0;
				}
				//vov = calcFlag('V',result); //yeah fuck this it isn't working
				if(result < state.getValue(1) + state.getValue(2) || result > UINT32_MAX){
					vov = 1;
				}
				else{
					vov = 0;
				}
				//std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << " = " << std::hex << result << std::endl;
				std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << std::endl;
				break;
			}
			case Instruction::ANDS: {
				int32_t result = state.getValue(1) & state.getValue(2);
				state.storeValue(state.getRegisterNum(0), result);
				//std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << " = " << std::hex << result << std::endl;
				std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << std::endl;
				neg = calcFlag('N',result);
				zer = calcFlag('Z',result);
				if(result < state.getValue(1) || result < state.getValue(2)){
					car = 1;
				}
				else{
					car = 0;
				}
				// vov = calcFlag('V',result); //isn't working
				if(result < state.getValue(1) + state.getValue(2) || result > UINT32_MAX){
					vov = 1;
				}
				else{
					vov = 0;
				}
				break;
			}
			case Instruction::XORS: {
				int32_t result = op1 ^ op2;
				state.storeValue(state.getRegisterNum(0), result);
				//std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << " = " << std::hex << result << std::endl;
				std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << std::endl;
				neg = calcFlag('N',result);
				zer = calcFlag('Z',result);
				if(result < state.getValue(1) || result < state.getValue(2)){
					car = 1;
				}
				else{
					car = 0;
				}
				vov = calcFlag('V',result);
				break;
			}
			case Instruction::NOTS: {
				int32_t result = ~op1;
				state.storeValue(state.getRegisterNum(0), result);
				//std::cout << state.op.instruction_name << "\t"<< std::hex << op1 << " = " << std::hex << result << std::endl;
				std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << std::endl;
				neg = calcFlag('N',result);
				zer = calcFlag('Z',result);
				if(result < state.getValue(1) || result < state.getValue(2)){
					car = 1;
				}
				else{
					car = 0;
				}
				vov = calcFlag('V',result);
				break;
			}
			case Instruction::ORRS: {
				int32_t result = op1 | op2;
				state.storeValue(state.getRegisterNum(0), result);
				//std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << " = " << std::hex << result << std::endl;
				std::cout << state.op.instruction_name << "\t"<< std::hex << state.getValue(1) << ", " << std::hex << state.getValue(2) << std::endl;
				neg = calcFlag('N',result);
				zer = calcFlag('Z',result);
				if(result < state.getValue(1) || result < state.getValue(2)){
					car = 1;
				}
				else{
					car = 0;
				}
				vov = calcFlag('V',result);
				break;
			}
			case Instruction::MOV: {
				state.storeValue(state.getRegisterNum(0), state.getValue(1));
				//std::cout << "Moving " << state.getValue(1) << " into " << state.getRegisterNum(0) << std::endl;
				std::cout << state.op.instruction_name << "\t" << "R"  << std::hex << state.getRegisterNum(0) << ", " << std::hex << state.getValue(1) << std::endl;
				break;
			}

			default: std::cout << "Invalid Instruction: " << state.op.instruction_name << std::endl; break;
		}

		state.printRegisters();
		std::cout << "N:" << neg << " Z:" << zer << " C:" << car << " V:" << vov << " " << std::endl; //every masterpiece has its cheap copy (of printing flags)
	}

	return 0;
}