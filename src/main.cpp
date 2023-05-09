#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[])
{
	if(argc < 2) {
		std::cerr<<"Error: no input file."<<std::endl;
		return 1;
	}
	std::string fileName(argv[1]);
	
	std::ifstream ifs;
	ifs.open(fileName, std::fstream::in);
	if(ifs.fail())
	{
		std::cerr<<"Error: couldn't open file"<<std::endl;
		return 1;
	}
	std::cout<<"Running "<<fileName<<"..."<<std::endl;
	ifs.close();
	return 0;
}