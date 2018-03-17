/*
 *	SymbolTable / Assembler
 * Author : Gourav Siddhad
 */

#ifndef APP_HEADER_SYMBOL_TABLE
#define APP_HEADER_SYMBOL_TABLE

#include "cstring"
#include "vector"
using namespace std;

struct Symbol {
	string Name;
	string Type;
	int Location;
};

class SymbolTable {
	vector<Symbol> Table;
	int Search(const string &Name) {
		for (size_t i = 0; i < Table.size(); i++)
			if (Table[i].Name.compare(Name) == 0)
				return i;
		return -1;
	}

public:
	int Insert(string Name, string Type, int Location) {
		if (Search(Name) != -1)
			return -1;

		Table.push_back({ Name, Type, Location });
		return Table.size() - 1;
	}

	int GetLocation(const string &Name) {
		int index = Search(Name);
		if ( index != -1 )
			return Table[index].Location;
		return -1;
	}
}SYMTAB;

#endif
