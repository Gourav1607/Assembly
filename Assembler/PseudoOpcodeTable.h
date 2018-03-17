/*
 * 	PseudoOpcodeTable.h / Assembler
 * Author : Gourav Siddhad
 */

#ifndef APP_HEADER_PSEUDO_OPCODE_TABLE
#define APP_HEADER_PSEUDO_OPCODE_TABLE

#include "cstring"
#include "vector"
#include "ErrorList.h"
using namespace std;

struct PseudoOpcode {
	string Name;
	int Size;
};

class PseudoOpcodeTable {
	vector<PseudoOpcode> Table;
	void InsertIntInByteForm(stringstream &ss, int Value, size_t ByteCount = 4) {
		if ( ByteCount > 0 ) ss << setw(2) << (Value & 0x000000ff);
		if ( ByteCount > 1 ) ss << setw(2) << ((Value >> 8) & 0x000000ff);
		if ( ByteCount > 2 ) ss << setw(2) << ((Value >> 16) & 0x000000ff);
		if ( ByteCount > 3 ) ss << setw(2) << ((Value >> 24) & 0x000000ff);
	}

	int Search(const string &Line) {
		Scanner Sc(Line);
		string  VarName, PseudoOpcode;
		VarName = Sc.GetIdentifier();

		if (VarName.empty() || Sc.EndReached())
			return -1;

		PseudoOpcode = Sc.GetWord();
		for (size_t i = 0; i < Table.size(); i++)
			if (Scanner::CheckEqual(Table[i].Name, PseudoOpcode) == true) {
				while (!Sc.EndReached())
					if (Sc.GetInt() == -1) {
						Errors.Add(Errors.E_INVALID_OPERAND);
						return -1;
					}
				return i;
			}
		return -1;
	}

public:
	PseudoOpcodeTable() {
		Table.push_back({ "DD", 4 });
		Table.push_back({ "DW", 2 });
	}

	bool IsPseudoOpcode(const string &Line) {
		if (Search(Line) == -1)
			return false;
		else
			return true;
	}

	int  GetTypeSize(const string &Line) {
		int index = Search(Line);
		if (index == -1)
			return -1;
		else
			return Table[index].Size;
	}

	int GetOperandsSize(const string &Line) {
		Scanner Sc(Line);
		int Index = Search(Line);
		int OperandCount = 0;

		if (Index == -1)
			return -1;

		Sc.GetIdentifier();
		Sc.GetWord();

		while (!Sc.EndReached())
			if (Sc.GetInt() != -1)
				++OperandCount;
			else
				Errors.Add(Errors.E_INVALID_OPERAND);
		return OperandCount * Table[Index].Size;
	}

	string GetVariableName(const string &Line) {
		Scanner Sc(Line);
		return Sc.GetIdentifier();
	}

	string GetCode(const string &Line) {
		stringstream CodeStream;
		string Code;
		int Index = Search(Line);
		Scanner Sc(Line);

		if (Index != -1) {
			CodeStream << hex << uppercase << setfill('0');
			Sc.GetIdentifier();
			Sc.GetWord();

			while (!Sc.EndReached())
				InsertIntInByteForm(CodeStream, Sc.GetInt(), Table[Index].Size);

			CodeStream >> Code;
		}
		return Code;
	}
}POT;

#endif
