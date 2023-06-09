#include <iostream>
#include <fstream>
#include <string>

#include "common.hpp"
#include "vm.hpp"
#include "compiler.hpp"
#include "builtins_core.hpp"

int main(int argc, char *argv[])
{
	if(argc < 2) {
		std::cerr<<"Error: no input file."<<std::endl;
		return 1;
	}
	std::ifstream ifs;
	try {
		std::string fileName(argv[1]);
		
		ifs.open(fileName, std::fstream::in);
		if(ifs.fail())
		{
			std::cerr<<"Error: couldn't open file"<<std::endl;
			return 1;
		}
		initVM();
		addCoreBuiltins();
		createAnon();
		std::cout<<"Running "<<fileName<<"..."<<std::endl;
		compile(ifs);
	}
	catch(const std::exception& e) {
		std::cerr<<"Error: "<<e.what()<<std::endl;
	}
	freeVM();
	ifs.close();
	return 0;
}