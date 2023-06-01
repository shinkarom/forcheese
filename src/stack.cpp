#include <iostream>

#include "stack.hpp"

CellType Stack::pop() {
	if(stack.size() == 0) {
		std::cout<<"Error: stack underflow."<<std::endl;
	}
	CellType x = stack.back();
	stack.pop_back();
	//std::cout<<"pop "<<x<<std::endl;
	return x;
}

void Stack::push(CellType value) {
	stack.push_back(value);
	//std::cout<<"push "<<value<<std::endl;
}

void Stack::clear() {
	stack.clear();
}

int Stack::size() {
	return stack.size();
}