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
	dictionary.lastEntry().isVisible = true;
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

void doMinus() {
	auto b = pop(dataStack);
	auto a = pop(dataStack);
	push(dataStack, a - b);
}

void doMultiply() {
	auto b = pop(dataStack);
	auto a = pop(dataStack);
	push(dataStack, a * b);
}

void doSwap() {
	auto a = pop(dataStack);
	auto b = pop(dataStack);
	push(dataStack, a);
	push(dataStack, b);
}
void doRot() {
	auto c = pop(dataStack);
	auto b = pop(dataStack);
	auto a = pop(dataStack);
	push(dataStack, b);
	push(dataStack, c);
	push(dataStack, a);
}

void doPop() {
	pop(dataStack);
}

void doOver() {
	auto b = pop(dataStack);
	auto a = pop(dataStack);
	push(dataStack, a);
	push(dataStack, b);
	push(dataStack, a);
}

void doHere() {
	push(dataStack, herePointer);
}

void do2Pop() {
	pop(dataStack);
	pop(dataStack);
	pop(dataStack);
	pop(dataStack);
}

void do2Dup() {
	auto b = pop(dataStack);
	auto a = pop(dataStack);
	push(dataStack, a);
	push(dataStack, b);
	push(dataStack, a);
	push(dataStack, b);
}

void do2Over() {
	auto d = pop(dataStack);
	auto c = pop(dataStack);
	auto b = pop(dataStack);
	auto a = pop(dataStack);
	push(dataStack, a);
	push(dataStack, b);
	push(dataStack, c);
	push(dataStack, d);
	push(dataStack, a);
	push(dataStack, b);
}

void do2Swap() {
	auto d = pop(dataStack);
	auto c = pop(dataStack);
	auto b = pop(dataStack);
	auto a = pop(dataStack);
	push(dataStack, c);
	push(dataStack, d);
	push(dataStack, a);
	push(dataStack, b);
}

void addCoreBuiltins() {
	returnXT = addToHandlers(&doReturn);
	pushCompiledNumberXT = addToHandlers(&doPushCompiledNumber);
	
	addBuiltin(",", false, &doComma);
	addBuiltin(":", true, &doColon);
	addBuiltin("::", true, &doDoubleColon);
	addBuiltin(";", true, &doSemicolon);
	addBuiltin("+", false, &doPlus);
	addBuiltin("-", false, &doMinus);
	addBuiltin("*", false, &doMultiply);
	addBuiltin(".", false, &doDot);
	addBuiltin("dup", false, &doDup);
	addBuiltin("swap", false, &doSwap);
	addBuiltin("rot", false, &doRot);
	addBuiltin("pop", false, &doPop);
	addBuiltin("over", false, &doOver);
	addBuiltin("2swap", false, &do2Swap);
	addBuiltin("2dup", false, &do2Dup);
	addBuiltin("2pop", false, &do2Pop);
	addBuiltin("2over", false, &do2Over);
	addBuiltin("here", false, &doHere);
}