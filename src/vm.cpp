#include <iostream>

#include "vm.hpp"

std::map<std::string, WordEntries> dictionary;
Heap heap(HeapSize);
Stack dataStack(MinStackSize);
Stack returnStack(MinStackSize);
std::vector<BuiltinFunc> builtinHandlers;
std::istream* inputStream;

CellType herePointer;
CellType ipPointer;

WordEntry* lastEntry;

CellType pushCompiledNumberXT;
CellType returnXT;

CellType readCell(Heap& area, size_t position) {
	auto pointer = &(area.data()[position]);
	auto cellPointer = reinterpret_cast<CellType*>(pointer);
	auto r = *cellPointer;
	//std::cout<<"read "<<r<<" from "<<position<<std::endl;
	return r;
}

CellType readCellHere(Heap& area) {
	return readCell(area, herePointer);
}

CellType readCellAndIncHere(Heap& area) {
	CellType x = readCellHere(area);
	herePointer += CellSize;
	return x;
}

void writeCell(Heap& area, size_t position, CellType value) {
	//std::cout<<"write "<<value<<" at "<<position<<std::endl;
	auto pointer = &(area.data()[position]);
	auto cellPointer = reinterpret_cast<CellType*>(pointer);
	*cellPointer = value;
}

void writeCellHere(Heap& area, CellType value) {
	writeCell(area, herePointer, value);
}

void writeCellAndIncHere(Heap& area, CellType value) {
	writeCellHere(area, value);
	herePointer += CellSize;
}

CellType addToHandlers(BuiltinFunc handler) {
	CellType xt = -1 - (builtinHandlers.size());
	//std::cout<<"add handler "<<xt<<std::endl;
	builtinHandlers.push_back(handler);
	return xt;
}

CellType pop(Stack& stack) {
	CellType x = stack.back();
	stack.pop_back();
	//std::cout<<"pop "<<x<<std::endl;
	return x;
}

void push(Stack& stack, CellType value) {
	stack.push_back(value);
	//std::cout<<"push "<<value<<std::endl;
}

void doColon() {
	auto s = parseWord();
	addWordHere(s, false);
}

void doDoubleColon() {
	auto s = parseWord();
	addWordHere(s, true);
}

void doSemicolon() {
	lastEntry->isVisible = true;
	writeCellAndIncHere(heap, returnXT);
}

void doPushCompiledNumber() {
	CellType x = readCell(heap, ipPointer);
	push(dataStack, x);
	ipPointer += CellSize;
}

void doComma() {
	CellType x = pop(dataStack);
	writeCellAndIncHere(heap, x);
}

void doPlus() {
	auto b = pop(dataStack);
	auto a = pop(dataStack);
	push(dataStack, a + b);
}

void doDot() {
	auto x = pop(dataStack);
	std::cout<<x;
}

void doDup() {
	auto x = pop(dataStack);
	push(dataStack, x);
	push(dataStack, x);
}

void doReturn() {
	auto x = pop(returnStack);
	ipPointer = x;
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
	ipPointer = 0;
	lastEntry = nullptr;
	
	returnXT = addToHandlers(&doReturn);
	pushCompiledNumberXT = addToHandlers(&doPushCompiledNumber);
	
	addBuiltin(",", false, &doComma);
	addBuiltin(":", true, &doColon);
	addBuiltin("::", true, &doDoubleColon);
	addBuiltin(";", true, &doSemicolon);
	addBuiltin("+", false, &doPlus);
	addBuiltin(".", false, &doDot);
	addBuiltin("dup", false, &doDup);
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
	lastEntry = &entry;
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
	thisEntry.xt = addToHandlers(handler);
	
	addEntry(wordName, thisEntry);
}

void executeXT(CellType xt) {
	if(xt >= 0) {
		//std::cout<<"execute xt "<<xt<<std::endl;
		auto before = returnStack.size();
		push(returnStack, ipPointer);
		do {
			auto x = readCell(heap, ipPointer);
			ipPointer += CellSize;
			executeXT(x);
		} while(returnStack.size() > before);
	} else {
		uint64_t s = -xt - 1;
		if(s >= builtinHandlers.size()) {
			std::cout<<"Error: wrong builtin xt: "<<xt<<"."<<std::endl;
			return;
		}
		//std::cout<<"execute builtin "<<xt<<std::endl;
		builtinHandlers[s]();
	}
}

void compileXT(CellType xt) {
	//std::cout<<"compile xt "<<xt<<std::endl;
	writeCellAndIncHere(heap, xt);
}

std::string parseWord() {
	std::string s;
	*inputStream >> s;
	return s;
}

bool isNumber(std::string s) {
	for(size_t i  = 0; i<s.length(); i++) {
		if(s[i]>='0' && s[i]<='9') {
			continue;
		} else {
			return false;
		}
	}
	return true;
}

CellType toNumber(std::string s) {
	CellType result = 0;
	for(size_t i  = 0; i<s.length(); i++) {
		if(s[i]>='0' && s[i]<='9') {
			result = result*10 + (s[i]-'0');
		} else {
			return 0;
		}
	}
	return result;
}

void compileNumber(CellType n) {
	//std::cout<<"compile number "<<n<<std::endl;
	writeCellAndIncHere(heap, pushCompiledNumberXT);
	writeCellAndIncHere(heap, n);
}