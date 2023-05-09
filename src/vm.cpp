#include "vm.hpp"

std::map<std::string, WordEntries> dictionary;
std::vector<uint64_t> heap(HeapSize/CellSize);
Stack dataStack(MinStackSize);
Stack returnStack(MinStackSize);

void initVM() {
	for(size_t i = 0; i < heap.size(); i++) {
		heap[i] = 0;
	}
}

void freeVM() {
	
}