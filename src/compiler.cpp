#include <iostream>

#include "compiler.hpp"

std::string readWord(std::istream& str) {
	std::string lexeme("");
	str>>lexeme;
	return lexeme;
}

void compile(std::istream& str) {
	inputStream = &str;
	while(!str.eof()) {
		auto s = readWord(str);
		std::cout<<"Read word \""<<s<<"\""<<std::endl;
		auto entry = findWord(s);
		if(entry) {
			if(entry->isMacro) {
				executeXT(entry->xt);
			} else {
				// compile
			}
		} else {
			/*
			if number:
				literal
			else:
				error
			*/
		}	
	}
}