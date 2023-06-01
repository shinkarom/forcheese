#pragma once

#include <string>
#include <vector>
#include <list>

#include "common.hpp"
#include "dictionary.hpp"
#include "stack.hpp"

using Heap = std::vector<uint8_t>;

extern Stack dataStack;
extern Stack returnStack;
extern Heap heap;
extern std::vector<BuiltinFunc> builtinHandlers;
extern std::istream* inputStream;

extern CellType herePointer;
extern CellType ipPointer;

extern WordEntry* lastEntry;

extern CellType compileNumberXT;

void initVM();
void freeVM();

CellType readCell(Heap& area, size_t position);
CellType readCellHere(Heap& area);
CellType readCellAndIncHere(Heap& area);

void writeCell(Heap& area, size_t position, CellType value);
void writeCellHere(Heap& area, CellType value);
void writeCellAndIncHere(Heap& area, CellType value);

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

void createAnon();
void endAnon();
bool hasAnon();

extern CellType pushCompiledNumberXT;
extern CellType returnXT;

CellType addToHandlers(BuiltinFunc handler);