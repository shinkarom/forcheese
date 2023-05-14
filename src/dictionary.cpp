#include <iostream>

#include "dictionary.hpp"

Dictionary dictionary;

WordEntry& Dictionary::lastEntry() {
	return dict.front();
}

void Dictionary::clear() {
	dict.clear();
}

WordEntry* Dictionary::findWord(std::string searchTerm) {
	for(auto it = dict.begin(); it != dict.end(); ++it) {
		if(it->name == searchTerm && it->isVisible) {
			//std::cout<<"found \""<<searchTerm<<"\" at "<<it->xt<<std::endl;
			return &(*it);
		}
	}
	return nullptr;
}

std::string Dictionary::findName(CellType xt) {
	for(auto it = dict.begin(); it != dict.end(); ++it) {
		if(it->xt == xt) {
			//std::cout<<"found \""<<searchTerm<<"\" at "<<it->xt<<std::endl;
			return it->name;
		}
	}
	return "unknown xt";
}

void Dictionary::removeLast() {
	extern CellType herePointer;
	//std::cout<<"remove \""<<dictionary.front().name<<"\""<<std::endl;
	herePointer = dict.front().xt;
	dict.pop_front();
	if(herePointer < 0){
		herePointer = 0;
	}
}

void Dictionary::addEntry(WordEntry entry) {
	//std::cout<<"add entry \""<<entry.name<<"\" with xt "<<entry.xt<<std::endl;
	dict.push_front(entry);
}