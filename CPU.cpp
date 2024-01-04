//Source file for the definitions of the CPU class functions and methods
#include "CPU.h"

void computer::CPU::load_program(ifstream& in)
{
	string temp;
	int count = 0;

	while(getline(in, temp)){
		j_type* j_ins  = new j_type;
		r_type* ins = new r_type;
		mem_type* m_ins = new mem_type;
		int opcode, rd, rt, rs;
		std::string temp, bad_input = "Bad input";
		std::stringstream ss(temp);
		int count = 0;
		while(ss>>temp){
			count+=1;   //counts the number of whitespaces encountered in input
		}
		switch(count){
			case(2)://Instruction is j_type
				ss>>opcode>>rd;
				j_ins->opcode = opcode;
				j_ins->rd = rd;
				LPDDR4X.insert(j_ins);
				delete m_ins;
				delete ins;
				break;
			case(3)://Instruction is a load or a store;
				ss>>opcode>>rd>>rs;
				m_ins->opcode = opcode;
				m_ins->rd = rd;
				m_ins->rs = rs;
				LPDDR4X.insert(m_ins);
				delete j_ins;
				delete ins;
				break;
			case(4): //Instruction is r_type
				ss>>opcode>>rd>>rt>>rs;
				ins->opcode = opcode;
				ins->rd = rd;
				ins->rt = rt;
				ins->rs = rs;
				LPDDR4X.insert(ins);
				delete j_ins;
				delete m_ins;
				break;
			default:
				std::cout<<"Corrupt instruction detected at line "<<count<<" of machine code. Terminating program\n";
				exit(0);
		}
		count++;
	}
}
instruction* computer::CPU::fetch_instruction(int curr)
{
	if(curr<LPDDR4X.im_size()){
		return LPDDR4X.get_instruction(curr);
	}
	else{
		cout<<"Error! Out of bounds access of instruction memory attempted. Terminating operation\n";
		exit(0);
	}
}

void computer::CPU::decode_and_execute(instruction* curr) //Decodes and runs the instruction
{
	r_type* r_temp;
	mem_type* m_temp;
	j_type* j_temp;
	double result, source1, source2, destination, source;
	switch(curr->opcode){
		case(2): //addi
			r_temp = dynamic_cast<r_type*>(curr);
			source1 = registers[r_temp->rt].data;	
			result = alu.add(source1,r_temp->rs);
			registers[r_temp->rd].data = result;
			break; 
			
		 case(1): //add
		 	r_temp = dynamic_cast<r_type*>(curr);
			source1 = registers[r_temp->rt].data;
			source2 = registers[r_temp->rs].data;
			result = alu.add(source1,source2);
			cout<<"Storing the result in register "<<r_temp->rd<<"\n";
			registers[r_temp->rd].data = result;
			break;		 
		case(3):  //mul
		  	r_temp = dynamic_cast<r_type*>(curr);
			source1 = registers[r_temp->rt].data;
			source2 = registers[r_temp->rs].data;
			result = alu.multiply(source1,source2);
			cout<<"Storing the result in register "<<r_temp->rd<<"\n";
			registers[r_temp->rd].data = result;
		  	break;
		case(4): //div
			r_temp = dynamic_cast<r_type*>(curr);
			source1 = registers[r_temp->rt].data;
			source2 = registers[r_temp->rs].data;
			result = alu.divide(source1,source2);
			cout<<"Storing the result in register "<<r_temp->rd<<"\n";
			registers[r_temp->rd].data = result;
			break;
		case(7): //beq
			r_temp = dynamic_cast<r_type*>(curr);
			source1 = registers[r_temp->rt].data;
			source2 = registers[r_temp->rs].data;
			if(alu.compare(source1, source2)){
				current_instruction = r_temp->rd;	
			}
			break;
		case(8): //slt
			r_temp = dynamic_cast<r_type*>(curr);
			source1 = registers[r_temp->rt].data;
			source2 = registers[r_temp->rs].data;
			if(alu.compare(source1, source2)){
				cout<<"Setting the register "<<r_temp->rd<<" to 1\n";
				registers[r_temp->rd].data = 1;
			}
			else{
				cout<<"Setting the register "<<r_temp->rd<<" to 0\n";
				registers[r_temp->rd].data = 0;
			}
			break;
		case(5):  //lw
			m_temp = dynamic_cast<mem_type*>(curr);
			source = m_temp->rs;
			destination = m_temp->rd;
			cout<<"Fetching data from memory location "<<source<<" to register "<<destination<<"\n";
			registers[int(destination)].data = LPDDR4X.get_data(source);
			break;
		case(6):  //sw
			m_temp = dynamic_cast<mem_type*>(curr);
			source = m_temp->rd;
			destination = m_temp->rs;
			result = registers[int(source)].data;
			cout<<"Storing "<<result<<" from register "<<source<<" in memory loacation "<<destination<<"\n";
			LPDDR4X.insert_data_at(result, destination);
			break;
		case(9):  //j
			j_temp = dynamic_cast<j_type*>(curr);
			destination = j_temp->rd;
			cout<<"jumpint to instruction "<<destination<<"\n";
			current_instruction = destination;
			break;
		case(10):  //jr
			j_temp = dynamic_cast<j_type*>(curr);
			cout<<"Jumping to register "<<j_temp->rd<<"\n"; 
			current_instruction = j_temp->rd;
			break;
		case(11):  //jal
			j_temp = dynamic_cast<j_type*>(curr);
			cout<<"Saving the return address in the return address register\n";
			registers[15].data = current_instruction;
			cout<<"Jumping to the function at location "<<j_temp->rd<<"\n";
			current_instruction = j_temp->rd;
			break;
		default:
			cout<<"Error! Unrecognised operation attempted. Terminating operation\n";
			exit(0);
		
	}
}

int computer::RAM::retrieve_at(int pos) // Retrieves the data at specified index in the memory
		{
			if(pos<data.size()) return data[pos];
			else {
				std::cout<<"Error! Out of bounds memory access attempted\n";
				exit(0);
			}
		}
void computer::RAM::allocate(int elem) //Allocate memory for elem in RAM
{
	for(int i = 0; i<data.size(); i++){
		if(data[i] = empty_slot){
			data[i] = elem;
			return;
		}
	}
	if(data.size()<capacity) data.push_back(elem);
	else{
		std::cout<<"Error! Memory is full. Terminating operation.\n";
		exit(0);
	}
}
void computer::RAM::deallocate(int pos) //Deallocate memory for the element at position pos
{
	for(int i = pos; i<data.size(); i++)
	{
		data[i] = data[i+1];
	}
	data[data.size()-1] = -10000; //Set the last position to -10000 to indicate that the spot is empty;
}
 void computer::run()
 {
 	stringstream ss;
 	ifstream in;
	string temp;
	compiler c;
	int selection;
	while(true){
	std::cout<<"Select action\n1. Open a file\n2. Write new code\n3. Run a program\n4. Exit ";
	std::cin>>selection;
	switch(selection){
		case(1):
			
			break;
		case(2):
			std::cout<<"\nEnter code:\n";
			while(true){
				getline(cin, temp);
				if(temp == "exit") break;
				ss<<temp<<"\n";
			}
			c.compile(ss);
			break;
		case(3):
			cout<<"Enter the name of the program. Do not enter the file extension\n";
			std::cin>>temp;
			temp+=".machine";
			in.open(temp);
			if(!in) {
				std::cout<<"The file "<<temp<<" could not be located, please try again\n";
			}
			else{
				cout<<"Commencing operation...\n";
				system("clear");
				CORTEX_M7.load_program(in);
				for(int i = CORTEX_M7.curr_instruction(); i<LPDDR4X.im_size(); i++){
					CORTEX_M7.decode_and_execute(CORTEX_M7.fetch_instruction(i));
					
				}
			}
			break;
		case(4):
			return;
		default:
			cout<<"Invalid choice, terminating operation\n";
			exit(0);
	}
	}
 }