#pragma once

#include <string>
#include <vector>
#include <list>

#include "common.hpp"

using BuiltinFunc = void (*)();

struct WordEntry {
	bool isMacro = false;
	bool isBuiltin = false;
	bool isVisible = true;
	CellType xt;
	std::string name;
};

using WordEntries = std::list<WordEntry>;
using Stack = std::list<CellType>;
using Heap = std::vector<uint8_t>;

extern std::list<WordEntry> dictionary;
extern Stack dataStack;
extern Stack returnStack;
extern Heap heap;
extern std::vector<BuiltinFunc> builtinHandlers;
extern std::istream* inputStream;

extern WordEntry* lastEntry;

extern CellType herePoiter;

extern CellType compileNumberXT;

void initVM();
void freeVM();

CellType readCell(Heap& area, size_t position);
CellType readCellHere(Heap& area);
CellType readCellAndIncHere(Heap& area);

void writeCell(Heap& area, size_t position, CellType value);
void writeCellHere(Heap& area, CellType value);
void writeCellAndIncHere(Heap& area, CellType value);

CellType pop(Stack& stack);
void push(Stack& stack, CellType value);

WordEntry* findWord(std::string searchTerm);
void addWord(std::string wordName, bool isMacro, BuiltinFunc handler = nullptr);
void addWordHere(std::string wordName, bool isMacro);
void addBuiltin(std::string wordName, bool isMacro, BuiltinFunc handler);
void runXT(CellType xt);
void executeXT(CellType xt);
void compileXT(CellType xt);
std::string parseWord();
bool isNumber(std::string s);
CellType toNumber(std::string s);
void compileNumber(CellType n);