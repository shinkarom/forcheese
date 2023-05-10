#include <iostream>

#include "compiler.hpp"

void compile(std::istream& str) {
	inputStream = &str;
	while(!str.eof()) {
		auto s = parseWord();
		//std::cout<<"read word \""<<s<<"\""<<std::endl;
		auto entry = findWord(s);
		if(entry) {
			if(entry->isMacro) {
				runXT(entry->xt);
			} else {
				compileXT(entry->xt);
			}
		} else {
			if(isNumber(s)) {
				auto n = toNumber(s);
				compileNumber(n);
			} else {
				std::cout<<"Error: unknown word \""<<s<<"\"."<<std::endl;
				break;
			}
		}	
	}
}