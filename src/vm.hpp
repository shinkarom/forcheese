#pragma once

#include <string>
#include <vector>
#include <map>
#include <deque>
#include <list>

#include "common.hpp"

using BuiltinFunc = void (*)();

struct WordEntry {
	bool isMacro = false;
	bool isBuiltin = false;
	bool isVisible = true;
	CellType xt;
	BuiltinFunc handler;
};

using WordEntries = std::list<WordEntry>;
using Stack = std::deque<CellType>;

extern std::map<std::string, WordEntries> dictionary;
extern Stack dataStack;
extern Stack returnStack;
extern std::vector<CellType> heap;
extern std::vector<BuiltinFunc> builtinHandlers;
extern std::istream* inputStream;

extern CellType herePoiter;

void initVM();
void freeVM();

WordEntry* findWord(std::string searchTerm);
void addWord(std::string wordName, bool isMacro, BuiltinFunc handler = nullptr);
void addWordHere(std::string wordName, bool isMacro);
void addBuiltin(std::string wordName, bool isMacro, BuiltinFunc handler);
void executeXT(CellType xt);
std::string parseWord();