#pragma once

#include <list>
#include <string>

#include "common.hpp"

using BuiltinFunc = void (*)();

struct WordEntry {
	bool isMacro = false;
	bool isAnon = false;
	bool isBuiltin = false;
	bool isVisible = true;
	CellType xt;
	std::string name;
};

class Dictionary {
	public:
		WordEntry& lastEntry();
		void clear();
		void addEntry(WordEntry entry);
		void removeLast();
		WordEntry* findWord(std::string searchTerm);
		std::string findName(CellType xt);
	private:
		std::list<WordEntry> dict;
};

extern Dictionary dictionary;