#include <iostream>

#include "vm.hpp"

std::map<std::string, WordEntries> dictionary;
std::vector<CellType> heap(HeapSize/CellSize);
Stack dataStack(MinStackSize);
Stack returnStack(MinStackSize);
std::vector<BuiltinFunc> builtinHandlers;
std::istream* inputStream;

CellType herePointer;
int numBuiltins;

WordEntry* lastEntry;

void doColon() {
	std::cout<<"doColon"<<std::endl;
	auto s = parseWord();
	addWordHere(s, false);
}

void doDoubleColon() {
	std::cout<<"doDoubleColon"<<std::endl;
	auto s = parseWord();
	addWordHere(s, true);
}

void initVM() {
	for(size_t i = 0; i < heap.size(); i++) {
		heap[i] = 0;
	}
	dictionary.clear();
	dataStack.clear();
	returnStack.clear();
	builtinHandlers.clear();
	herePointer = 0;
	numBuiltins = 0;
	
	addBuiltin(":", true, &doColon);
	addBuiltin("::", true, &doDoubleColon);
}

void freeVM() {
	
}

WordEntry* findWord(std::string searchTerm) {
	const auto searchResult = dictionary.find(searchTerm);
	if(searchResult != dictionary.end()) {
		return &(searchResult->second.back());
	} else {
		return nullptr;
	}
}

void addWord(std::string wordName, bool isMacro, BuiltinFunc handler) {
	WordEntry thisEntry;
	thisEntry.isMacro = isMacro;
	if(handler == nullptr) {
		thisEntry.isVisible = true;
		thisEntry.isBuiltin = true;
		thisEntry.xt = -1 - (builtinHandlers.size());
		builtinHandlers.push_back(handler);
	} else {
		thisEntry.isBuiltin = false;
		thisEntry.xt = herePointer;
	}
	
	
	WordEntries e;
	
	const auto searchResult = dictionary.find(wordName);
	
	if(searchResult != dictionary.end()) {
		e = searchResult->second;
		e.push_back(thisEntry);
	} else {
		e = WordEntries();
		e.push_back(thisEntry);
		dictionary.insert({wordName, e});
	}
}

void addEntry(std::string wordName, WordEntry entry) {
	WordEntries e;
	
	const auto searchResult = dictionary.find(wordName);
	
	if(searchResult != dictionary.end()) {
		e = searchResult->second;
		e.push_back(entry);
	} else {
		e = WordEntries();
		e.push_back(entry);
		dictionary.insert({wordName, e});
	}
}

void addWordHere(std::string wordName, bool isMacro) {
	WordEntry thisEntry;
	thisEntry.isMacro = isMacro;
	thisEntry.isBuiltin = false;
	thisEntry.xt = herePointer;
	
	addEntry(wordName, thisEntry);
}

void addBuiltin(std::string wordName, bool isMacro, BuiltinFunc handler) {
	WordEntry thisEntry;
	thisEntry.isMacro = isMacro;
	thisEntry.isBuiltin = true;
	thisEntry.xt = -1 - (builtinHandlers.size());
	builtinHandlers.push_back(handler);
	
	addEntry(wordName, thisEntry);
}

void executeXT(CellType xt) {
	if(-1 - xt > builtinHandlers.size()) {
		
	} else {
		builtinHandlers[-1 -xt]();
	}
}

std::string parseWord() {
	std::string s;
	*inputStream >> s;
	return s;
}