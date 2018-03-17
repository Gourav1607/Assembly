/*
 *  MachineOpcodeTable.h / Assembler
 *  Author : Gourav Siddhad
 */

#ifndef APP_HEADER_MACHINE_OPCODE_TABLE
#define APP_HEADER_MACHINE_OPCODE_TABLE

#include "cstring"
#include "vector"
#include "sstream"
#include "iomanip"
#include "SymbolTable.h"
#include "Scanner.h"
#include "ErrorList.h"

using namespace std;
struct OpcodeName {
	string Inst;
	string Op1;
	string Op2;
	int OpNumber;
};

struct Opcode {
	OpcodeName Name;
	int Code;
	int	Size;
};

class MachineOpcodeTable {
	vector<Opcode> Table;

	int Search(const string &Line) {
		Scanner Sc(Line);
		string  Inst, Op1, Op2;

		for (size_t i = 0; i < Table.size(); i++) {
			Sc.ResetTokenIndex();
			Inst = Sc.GetWord();
			if (Scanner::CheckEqual(Table[i].Name.Inst, Inst) == false)
				continue;
			if (Table[i].Name.OpNumber == 0 && Sc.EndReached())
				return i;
			if (Table[i].Name.Op1.empty()) {
				Op1 = Sc.GetToken();
				if (Sc.IsIdentifier(Op1) == false && Sc.IsInt(Op1) == false)
					continue;
			} else {
				Op1 = Sc.GetWord();
				if (Scanner::CheckEqual(Table[i].Name.Op1, Op1) == false)
					continue;
			}

			if (Table[i].Name.OpNumber == 1 && Sc.EndReached())
				return i;
			if (Table[i].Name.Op2.empty()) {
				Op2 = Sc.GetToken();
				if (Sc.IsIdentifier(Op2) == false && Sc.IsInt(Op2) == false)
					continue;
			} else {
				Op2 = Sc.GetWord();
				if (Scanner::CheckEqual(Table[i].Name.Op2, Op2) == false)
					continue;
			}

			if (Table[i].Name.OpNumber == 2 && Sc.EndReached())
				return i;
		}
		return -1;
	}

	void InsertIntInByteForm(stringstream &ss, int Value, size_t ByteCount = 4) {
		if ( ByteCount > 0 ) ss << setw(2) << (Value & 0x000000ff);
		if ( ByteCount > 1 ) ss << setw(2) << ((Value >> 8) & 0x000000ff);
		if ( ByteCount > 2 ) ss << setw(2) << ((Value >> 16) & 0x000000ff);
		if ( ByteCount > 3 ) ss << setw(2) << ((Value >> 24) & 0x000000ff);
	}

public:
	MachineOpcodeTable() {
		Table.push_back({ { "MVI",   "A", "", 2 }, 0, 5 });
		Table.push_back({ { "MVI",   "B", "", 2 }, 1, 5 });
		Table.push_back({ { "MVI",   "C", "", 2 }, 2, 5 });
		Table.push_back({ { "MVI",   "I", "", 2 }, 3, 5 });
		Table.push_back({ { "LOAD",  "",  "", 1 }, 4, 5 });
		Table.push_back({ { "STORE", "",  "", 1 }, 5, 5 });
		Table.push_back({ { "LOADI", "",  "", 0 }, 6, 1 });
		Table.push_back({ { "STORI", "",  "", 0 }, 7, 1 });
		Table.push_back({ { "ADD",   "B", "", 1 }, 8, 1 });
		Table.push_back({ { "ADD",   "C", "", 1 }, 9, 1 });
		Table.push_back({ { "MOV",   "A", "B", 2 }, 10, 1 });
		Table.push_back({ { "MOV",   "A", "C", 2 }, 11, 1 });
		Table.push_back({ { "MOV",   "B", "C", 2 }, 12, 1 });
		Table.push_back({ { "MOV",   "B", "A", 2 }, 13, 1 });
		Table.push_back({ { "MOV",   "C", "A", 2 }, 14, 1 });
		Table.push_back({ { "MOV",   "C", "B", 2 }, 15, 1 });
		Table.push_back({ { "INC",   "A", "", 1 }, 16, 1 });
		Table.push_back({ { "INC",   "B", "", 1 }, 17, 1 });
		Table.push_back({ { "INC",   "C", "", 1 }, 18, 1 });
		Table.push_back({ { "CMP",   "A", "", 2 }, 19, 5 });
		Table.push_back({ { "CMP",   "B", "", 2 }, 20, 5 });
		Table.push_back({ { "CMP",   "C", "", 2 }, 21, 5 });
		Table.push_back({ { "ADDI",  "",  "", 1 }, 22, 5 });
		Table.push_back({ { "JE",    "",  "", 1 }, 23, 5 });
		Table.push_back({ { "JMP",   "",  "", 1 }, 24, 5 });
		Table.push_back({ { "STOP",  "",  "", 0 }, 25, 1 });
	}

	int GetSize(const string &Line) {
		int index = Search(Line);
		if (index == -1)
			return -1;
		else
			return Table[index].Size;
	}

    bool IsMachineOpcode(const string &Line) {
		if (Search(Line) == -1)
			return false;
		else
			return true;
	}

	string GetCode(const string &Line) {
		stringstream CodeStream;
		string Code;
		int Index = -1;
		Scanner Sc(Line);

		if ((Index = Search(Line)) != -1) {
			CodeStream << hex << uppercase << setfill('0');
        InsertIntInByteForm(CodeStream, Table[Index].Code, 1);
        Sc.GetWord();
			if (Table[Index].Name.OpNumber > 0 && Table[Index].Name.Op1.empty() == true) {
				string Op1 = Sc.GetToken();
				if (Sc.IsIdentifier(Op1)) {
					int Op1Loc = SYMTAB.GetLocation(Op1);
					if (Op1Loc != -1)
						InsertIntInByteForm(CodeStream, Op1Loc);
					else
						Errors.Add(Errors.E_SYMBOL_NOT_DEFINED);
				} else if (Sc.IsInt(Op1)) {
					InsertIntInByteForm(CodeStream, stoi(Op1));
				}
			}

			if (Table[Index].Name.OpNumber > 1 && Table[Index].Name.Op2.empty() == true) {
				if (Table[Index].Name.Op1.empty() == false)
					Sc.GetToken();

				string Op2 = Sc.GetToken();
				if (Sc.IsIdentifier(Op2)) {
					int Op2Loc = SYMTAB.GetLocation(Op2);
					if (Op2Loc != -1)
						InsertIntInByteForm(CodeStream, Op2Loc);
					else
						Errors.Add(Errors.E_SYMBOL_NOT_DEFINED);
				} else if (Sc.IsInt(Op2)) {
					InsertIntInByteForm(CodeStream, stoi(Op2));
				}
			}
		}
		CodeStream >> Code;
		return Code;
	}
}MOT;

#endif
