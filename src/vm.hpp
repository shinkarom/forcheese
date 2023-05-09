#pragma once

#include <string>
#include <vector>
#include <map>
#include <deque>
#include <list>

#include "common.hpp"

class WordEntry {
	
};

using WordEntries = std::list<WordEntry>;
using Stack = std::deque<CellType>;

extern std::map<std::string, WordEntries> dictionary;
extern Stack dataStack;
extern Stack returnStack;
extern std::vector<uint64_t> heap;

void initVM();
void freeVM();