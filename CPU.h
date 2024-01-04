// Source file for the definition of the computer class and all it's component classes, including the CPU
#include "Instructions.cpp"
#include "compiler.cpp"
using namespace std;

class computer{
	class CPU{  //A CPU is part of a computer
		int current_instruction;
		stringstream current_program;
		class CU{ //The control unit class
			
		};
		class ALU{ //The arithmetic and logic unit class
			public:
				double add(double source1, double source2){
					cout<<"Adding "<<source1<<" and "<<source2<<"\n";
					return source1+source2;
				};
				double subtract(int source1, int source2){
					cout<<"Subtracting "<<source1<<" and "<<source2<<"\n";
					return source1- source2;
				};
				double multiply(double source1, double source2){
					cout<<"Multiplying "<<source1<<" and "<< source2<<"\n";
					return source1* source2;
				};
				double divide(double source1, double source2){
					cout<<"Dividing "<<source1<<" by "<<source2<<"\n";
					return source1/source2;
				};
				bool compare(double source1, double source2)
				{
					cout<<"Performing a comparison between "<<source1<<" and"<< source2<<"\n";
					return source1 == source2;
				}
		};
		ALU alu;
		struct Register{ //Registers are part of the CPU
			int data;
			int offset;
			public:
				Register()
				{
					data = -10000;  //An impossible value to indicate the register is empty
					offset = 0;
				};
		};
		Register registers[16];
		public:
			CPU(){
				current_instruction = 0;
			}
			void set_current_instruction(int new_curr){
				current_instruction = new_curr;
			}
			int curr_instruction(){
				return current_instruction;
			}
			void load_program(ifstream& in);
			instruction* fetch_instruction(int curr);  // Fetches the instruction at curr from instruction memory
			void decode_and_execute(instruction* curr);  //Decodes and executes the fetched instruction
	};
	
	
	class RAM{  //Memory is part of the computer
		vector<instruction*> IM;
		vector<double> data;
		int capacity = 1024; //Capacity is 1GB
		int empty_slot = -10000;   //An impossible number to indicate an empty slot in the memory
		public:
			int im_size(){ //Returns the number of instructions stored in the instruction memory
				return IM.size();
			}
			void insert(instruction* ins){ //Inserts an instruction into the instruction memory
				IM.push_back(ins);
			}
			instruction* get_instruction(int pos) //returns the instruction at pos
			{
				return IM[pos];
			}
			double get_data(int pos)  //returns data at pos
			{
				return data[pos];
			}
			void insert_data_at(double info, int pos){
				data[pos] = info;
			}
			int retrieve_at(int pos); // Retrieves the data at specified index in the memory
			void allocate(int elem); //Allocate memory for elem in RAM
			void deallocate(int pos); //Deallocate memory for the element at position pos
	};
	static RAM LPDDR4X; // Ideal for IoT applications
	static CPU CORTEX_M7; //The processor
	public:
		void run();
};
computer::RAM computer::LPDDR4X;
computer::CPU computer::CORTEX_M7;

