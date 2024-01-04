
using namespace std;


class compiler{
	vector<instruction*> instruction_store;
	public:
		void compile(ifstream& in);
		void input_and_compile();
		void compile(stringstream& str);
		string process_instruction(std::string& instruction);
		void open_file();
		vector<instruction*>& compiled_instructions(){
			return instruction_store;
		}
};


//Function: Takes user input, compiles it, then saves output to named file
void compiler::input_and_compile()
{
	string temp;
	stringstream ss;
	std::cout<<"Enter assembly code: ";
	while(true){
		getline(cin, temp);
		ss<<temp;
	}
	compile(ss);
}
void compiler::compile(stringstream& str)
{
	string temp, filename;
	while(getline(str,temp )){
		string temp1 = process_instruction(temp);
		if(temp1 == "Bad input\n"){
			cout<<"Bad input\n";
			exit(0);
		}
		else if(temp1 == "r_type"){
			r_type* ins = new r_type(temp);
			instruction_store.push_back(ins);
		}
		else if(temp1 == "mem_ref"){
			mem_type* ins = new mem_type(temp);
			instruction_store.push_back(ins);
		}
		else if(temp1 == "j_type"){
			j_type* ins = new j_type(temp);
			instruction_store.push_back(ins);
		}
	}
	std::cout<<"Enter file name to save to: ";
	std::cin>>filename;
	ofstream out(filename+".machine");
	for(auto p: instruction_store){
		p->output_to_file(out);
		out<<"\n";
	}
	out.close();
	out.open(filename+".asm");
	out<<str.str();
	out.close();
}
void compiler::compile(ifstream& in)
{
	
	string temp, bad_input = "Bad input", instruction;
	stringstream ss;
	
	while(getline(in, temp)){
		string temp1 = process_instruction(temp);
		if(temp1 == "Bad input\n"){
			cout<<"Bad input\n";
			exit(0);
		}
		else if(temp1 == "r_type"){
			r_type* ins = new r_type(temp);
			instruction_store.push_back(ins);
		}
		else if(temp1 == "mem_ref"){
			mem_type* ins = new mem_type(temp);
			instruction_store.push_back(ins);
		}
		else if(temp1 == "j_type"){
			j_type* ins = new j_type(temp);
			instruction_store.push_back(ins);
		}
	}
}

//A function to test if an instruction has the relavant fields
string compiler::process_instruction(std::string& instruction)  
{
	std::string temp, bad_input = "Bad input";
	std::stringstream ss(instruction);
	int count = 0;
	while(ss>>temp){
		count+=1;   //counts the number of whitespaces encountered in input
	}
	switch(count){
		case(1):
			return bad_input;
			break;
		case(2):
			return "j_type";
			break;
		case(3):
			return "mem_ref";
			break;
		case(4):
			return "r_type";
			break;
		default:
			return bad_input;
	}
}

void compiler::open_file()
{
	string filename;
	cout<<"Enter file name: ";
	cin>>filename;
	filename = filename+".asm";
	ifstream in(filename);
	compile(in);
}
