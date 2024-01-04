// Source file for the definitions of the instructions data types
using namespace std;
//The base struct for instruction ADT
struct instruction{
	string type;
	int opcode;
	int reg_index(string& reg);
	int index_num(std::string reg);
	int op(string ins);
	void print(){
		std::cout<<"Opcode: "<<opcode<<"\n";
	}
	virtual void output_to_file(ofstream& file) = 0;
};


 int instruction::op(string ins) // it returns the opcode of the instruction. E.g, for the add instruction, it returns 1
 {
 	std::stringstream ss(ins);
 	std::string temp;
 	ss>>temp;
 	if(temp == "add") return 1;
 	else if(temp == "addi") return 2;
 	else if(temp == "mul") return 3;
 	else if(temp == "div") return 4;
 	else if(temp == "jal") return 11;
 	else if(temp == "jr") return 10;
 	else if(temp == "j") return 9;
 	else if(temp == "lw") return 5;
 	else if(temp == "sw") return 6;
 	else if(temp == "beq") return 7;
 	else if(temp == "slt") return 8;
 	else {
 		std::cout<<"Error! Invalid operation detected\n";
 		return -1;
	 }
 }

//returns the number of the reegister. E.g, for register a0, it a0, it returns the number 0
// returns -1 for an invalid register 
int instruction::index_num(string reg)
{
	char* num = &reg[1];
	int palo = std::atoi(num);
	return palo;
}

int instruction::reg_index(string& reg){ //Computes the index of the register
	if(reg.size() !=2) {
		std::cout<<"Error! Invalid register name.\n";
		return -1;
	}
	else{
		const char* ch = reg.c_str();
		if(ch[0] == 't'){
			return index_num(reg) < 4 ? 4+index_num(reg): -1;
		}
		else if(ch[0] == 's'){
			return index_num(reg) < 4 ? 8+index_num(reg): -1;
		}
		else if(ch[0] == 'v'){
			return index_num(reg) < 2 ? 2+index_num(reg): -1;
		}
			
		else if(ch[0] == 'a'){
			
			return index_num(reg) < 2 ? index_num(reg): -1;
		}
		else if(ch[0] == 'f'){
			return index_num(reg) < 13 ? 12+index_num(reg): -1;
		}
		else if(ch[0] == 'r'&&ch[1]=='a'){
			return 15;
		}
		else{
			return -1;
		}
	}
}

//The memory reference instruction ADT
struct mem_type: instruction{
	int immediate;
	int rd;
	int rs;
	mem_type(){
		rd = -1;
		immediate = 0;
		rs = -1;
	}
	mem_type(std::string str);
	string check(string operation);
	void output_to_file(ofstream& file);
};

string mem_type::check(string operation)
{
	if(operation == "lw"|| operation == "sw"){
		return operation;
	}
	else{
		std::cout<<"Error! Invalid operation detected. Terminating program";
		exit(0);
	}
}
mem_type::mem_type(std::string str)
{
	type = "mem_type";
	std::string temp, temp1, temp2;
	std::stringstream ss(str);
	ss>>temp>>temp1>>temp2;
	opcode = op(temp);
	if(opcode == -1){
		cout<<"Error! Invalid operation detected. Terminating program\n";
		exit(0);
	}
	rd = reg_index(temp1);
	rs = reg_index(temp2);
	if(rd == -1 || rs == -1) {
		std::cout<<"Error! Invalid register name detected. Terminating program\n";
		exit(0);
	}
		
}

void mem_type::output_to_file(ofstream& file)
{
	file<<opcode<<"\t"<<rd<<"\t"<<rs;
}

//The R-type instruction ADT
struct r_type: instruction{
	int rd;
	int rs;
	int rt;
	r_type(){
		rd = -1;
		rs = -1;
		rt = -1;
	}
	r_type(std::string str);
	string check(string operation);
	void output_to_file(ofstream& file);
};

string r_type::check(string operation)
{
	if(operation == "add"|| operation == "addi"|| operation == "div"||operation == "beq"|| operation == "slt"){
		return operation;
	}
	else{
		std::cout<<"Error! Invalid operation detected. Terminating program";
		exit(0);
	}
}

r_type::r_type(std::string str){
	type = "r_type";
	std::string temp, temp1, temp2, temp3;
	std::stringstream ss(str);
	ss>>temp>>temp1>>temp2>>temp3;
	rd = reg_index(temp1);
	rs = reg_index(temp2);
	rt = reg_index(temp3);
	opcode = op(temp);
	if(opcode == -1){
		cout<<"Error! Invalid operation detected. Terminating program\n";
		exit(0);
	}
	if(rs == -1||rd == -1|| rt == -1){
		std::cout<<"Error! Invalid register name detected. Terminating program\n";
		exit(0);
	}
}

void r_type::output_to_file(ofstream& file)
{
	file<<opcode<<"\t"<<rd<<"\t"<<rs<<"\t"<<rt;
}


//The j-type instruction ADT
struct j_type: instruction{
	int rd;
	j_type(){
		rd = -1;
	}
	j_type(std::string str);
	string check(string instruction);
	void output_to_file(ofstream& file);
};

j_type::j_type(std::string str)
{
	type = "j_type";
	string instruction = check(str);
	std::string temp, temp1;
	std::stringstream ss(instruction);
	ss>>temp>>temp1;
	rd = reg_index(temp1);
	opcode = op(temp);
	if(opcode == -1){
		cout<<"Error! Invalid operation detected. Terminating program\n";
		exit(0);
	}
	if(rd == -1){
		std::cout<<"Error! Invalid register name detected. Terminating program\n";
		exit(0);
	}
}


string j_type::check(string instruction){ //Checks if the j_type instruction has the correct operation
	stringstream ss(instruction);
	string temp, temp1;
	ss>>temp>>temp1;
	if(temp == "j"||temp == "jr"){
		return instruction;
	}
	else{
		cout<<"Corrupt instruction detected. Terminating program\n";
		exit(0);
	}
}
void j_type::output_to_file(ofstream& file)
{
	file<<opcode<<"\t"<<rd;
}
