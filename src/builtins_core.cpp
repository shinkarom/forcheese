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
	dataStack.push(x);
	ipPointer += CellSize;
}

void doComma() {
	CellType x = dataStack.pop();
	writeCellAndIncHere(heap, x);
}

void doPlus() {
	auto b = dataStack.pop();
	auto a = dataStack.pop();
	dataStack.push(a + b);
}

void doDot() {
	auto x = dataStack.pop();
	std::cout<<x;
}

void doDup() {
	auto x = dataStack.pop();
	dataStack.push(x);
	dataStack.push(x);
}

void doReturn() {
	auto x = returnStack.pop();
	ipPointer = x;
}

void doMinus() {
	auto b = dataStack.pop();
	auto a = dataStack.pop();
	dataStack.push(a - b);
}

void doMultiply() {
	auto b = dataStack.pop();
	auto a = dataStack.pop();
	dataStack.push(a * b);
}

void doSwap() {
	auto a = dataStack.pop();
	auto b = dataStack.pop();
	dataStack.push(a);
	dataStack.push(b);
}
void doRot() {
	auto c = dataStack.pop();
	auto b = dataStack.pop();
	auto a = dataStack.pop();
	dataStack.push(b);
	dataStack.push(c);
	dataStack.push(a);
}

void doPop() {
	dataStack.pop();
}

void doOver() {
	auto b = dataStack.pop();
	auto a = dataStack.pop();
	dataStack.push(a);
	dataStack.push(b);
	dataStack.push(a);
}

void doHere() {
	dataStack.push(herePointer);
}

void do2Pop() {
	dataStack.pop();
	dataStack.pop();
}

void do2Dup() {
	auto b = dataStack.pop();
	auto a = dataStack.pop();
	dataStack.push(a);
	dataStack.push(b);
	dataStack.push(a);
	dataStack.push(b);
}

void do2Over() {
	auto d = dataStack.pop();
	auto c = dataStack.pop();
	auto b = dataStack.pop();
	auto a = dataStack.pop();
	dataStack.push(a);
	dataStack.push(b);
	dataStack.push(c);
	dataStack.push(d);
	dataStack.push(a);
	dataStack.push(b);
}

void do2Swap() {
	auto d = dataStack.pop();
	auto c = dataStack.pop();
	auto b = dataStack.pop();
	auto a = dataStack.pop();
	dataStack.push(c);
	dataStack.push(d);
	dataStack.push(a);
	dataStack.push(b);
}

void doMore() {
	auto n2 = dataStack.pop();
	auto n1 = dataStack.pop();
	dataStack.push((n1>n2)?0:!0);
}

void doLess() {
	auto n2 = dataStack.pop();
	auto n1 = dataStack.pop();
	dataStack.push((n1<n2)?0:!0);
}

void doUMore() {
	auto n2 = (uint64_t)(dataStack.pop());
	auto n1 = (uint64_t)(dataStack.pop());
	dataStack.push((n1>n2)?0:!0);
}

void doULess() {
	auto n2 = (uint64_t)(dataStack.pop());
	auto n1 = (uint64_t)(dataStack.pop());
	dataStack.push((n1<n2)?0:!0);
}

void doEqual() {
	auto n2 = dataStack.pop();
	auto n1 = dataStack.pop();
	dataStack.push((n1=n2)?0:!0);
}

void doNotEqual() {
	auto n2 = dataStack.pop();
	auto n1 = dataStack.pop();
	dataStack.push((n1!=n2)?0:!0);
}

void doEmit() {
	auto c = dataStack.pop();
	if(c>=' '&&c<='z') {
		std::cout<<(char)c;
	}
}

void doCompileComma() {
	auto xt = dataStack.pop();
	compileXT(xt);
}

void doLiteral() {
	auto n = dataStack.pop();
	compileNumber(n);
}

void doPostpone() {
	auto s = parseWord();
	auto entry = dictionary.findWord(s);
	if(entry && entry->name!="") {
		if(entry->isMacro) {
			compileXT(entry->xt);
		} else {
			dataStack.push(entry->xt);
			doCompileComma();
		}
	} else {
		if(isNumber(s)) {
			auto n = toNumber(s);
			dataStack.push(n);
			doLiteral();
		} else {
			std::cout<<"Error: unknown word \""<<s<<"\"."<<std::endl;
		}
	}	
}

void doBl() {
	dataStack.push((CellType)(' '));
}

void doExclamationMark() {
	auto addr = dataStack.pop();
	auto x = dataStack.pop();
	writeCell(heap, addr, x);
}

void doAt() {
	auto addr = dataStack.pop();
	auto x = readCell(heap, addr);
	dataStack.push(x);
}

void doDivMod() {
	auto n2 = dataStack.pop();
	auto n1 = dataStack.pop();
	auto n3 = n1 % n2;
	auto n4 = n1 / n2;
	dataStack.push(n3);
	dataStack.push(n4);
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
	addBuiltin(">", false, &doMore);
	addBuiltin("<", false, &doLess);
	addBuiltin("u>", false, &doUMore);
	addBuiltin("u<", false, &doULess);
	addBuiltin("=", false, &doEqual);
	addBuiltin("<>", false, &doNotEqual);
	addBuiltin("emit", false, &doEmit);
	addBuiltin("postpone", true, &doPostpone);
	addBuiltin("literal", false, &doLiteral);
	addBuiltin(",", false, &doComma);
	addBuiltin("compile,", false, &doCompileComma);
	addBuiltin("bl", false, &doBl);
	addBuiltin("!", false, &doExclamationMark);
	addBuiltin("@", false, &doAt);
	addBuiltin("/mod", false, &doDivMod);
}