#include <iostream>

#include "vm.hpp"

std::list< WordEntry> dictionary;
Heap heap(HeapSize);
Stack dataStack(MinStackSize);
Stack returnStack(MinStackSize);
std::vector<BuiltinFunc> builtinHandlers;
std::istream* inputStream;

CellType herePointer;
CellType ipPointer;

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

void removeLastEntry() {
	//std::cout<<"remove \""<<dictionary.front().name<<"\""<<std::endl;
	dictionary.pop_front();
	herePointer = dictionary.front().xt;
	if(herePointer < 0){
		herePointer = 0;
	}
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
}

void freeVM() {
	
}

void addEntry(WordEntry entry) {
	//std::cout<<"add entry \""<<entry.name<<"\" with xt "<<entry.xt<<std::endl;
	dictionary.push_front(entry);
}

bool hasAnon() {
	return dictionary.front().isAnon;
}

void createAnon() {
	WordEntry thisEntry;
	thisEntry.isAnon = true;
	thisEntry.xt = herePointer;
	thisEntry.name = "";
	addEntry(thisEntry);
}

void endAnon() {
	if(!hasAnon()) {
		return;
	}
	if(readCellHere(heap) != returnXT) {
		writeCellAndIncHere(heap, returnXT);
	}
	auto anon = dictionary.front();
	if(anon.xt != herePointer) {
		runXT(anon.xt);
	}
	removeLastEntry();	
	
}

WordEntry* findWord(std::string searchTerm) {
	for(auto it = dictionary.begin(); it != dictionary.end(); ++it) {
		if(it->name == searchTerm && it->isVisible) {
			//std::cout<<"found \""<<searchTerm<<"\""<<std::endl;
			return &(*it);
		}
	}
	return nullptr;
}



void addWordHere(std::string wordName, bool isMacro) {
	WordEntry thisEntry;
	thisEntry.isMacro = isMacro;
	thisEntry.xt = herePointer;
	thisEntry.name = wordName;
	
	addEntry(thisEntry);
}

void addBuiltin(std::string wordName, bool isMacro, BuiltinFunc handler) {
	WordEntry thisEntry;
	thisEntry.isMacro = isMacro;
	thisEntry.isBuiltin = true;
	thisEntry.xt = addToHandlers(handler);
	thisEntry.name = wordName;
	
	addEntry(thisEntry);
}

void executeBuiltin(CellType xt) {
	uint64_t s = -xt - 1;
	if(s >= builtinHandlers.size()) {
		std::cout<<"Error: wrong builtin xt: "<<xt<<"."<<std::endl;
		return;
	}
	builtinHandlers[s]();
}

void runXT(CellType xt) {
	//std::cout<<"run xt "<<xt<<std::endl;
	if(xt >= 0) {
		ipPointer = xt;
		CellType x;
		auto before = returnStack.size();
		while(true) {
			x = readCell(heap, ipPointer);
			ipPointer += CellSize;
			if(x == returnXT && returnStack.size() == before) {
				break;
			}
			executeXT(x);
		};
	} else {
		executeBuiltin(xt);
	} 
}

void executeXT(CellType xt) {
	//std::cout<<"execute xt "<<xt<<std::endl;
	if(xt >= 0) {
		push(returnStack, ipPointer);
		ipPointer = xt;
	} else {
		executeBuiltin(xt);
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