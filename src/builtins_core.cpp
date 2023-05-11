#include <iostream>

#include "vm.hpp"
#include "builtins_core.hpp"

void doColon() {
	endAnon();
	auto s = parseWord();
	addWordHere(s, false);
}

void doDoubleColon() {
	endAnon();
	auto s = parseWord();
	addWordHere(s, true);
}

void doSemicolon() {
	writeCellAndIncHere(heap, returnXT);
	endAnon();
	dictionary.front().isVisible = true;
	createAnon();
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

void addCoreBuiltins() {
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