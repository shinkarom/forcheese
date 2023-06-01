#pragma once

#include <list>

#include "common.hpp"

class Stack {
	public:
		Stack1();
		CellType pop();
		void push(CellType value);
		void clear();
		int size();
	private:
		std::list<CellType> stack;
};