/*
 * Assembler.h / Assembler
 * Author : Gourav Siddhad
 */

#ifndef APP_HEADER_ASSEMBLER
#define APP_HEADER_ASSEMBLER

#include "iostream"
#include "fstream"
#include "cstring"
#include "MachineOpcodeTable.h"
#include "PseudoOpcodeTable.h"
#include "SymbolTable.h"
#include "ErrorList.h"

#define MNEMONIC_TYPE_MO 1
#define MNEMONIC_TYPE_PO 2
#define MNEMONIC_TYPE_IGNORE 3
#define MNEMONIC_TYPE_INVALID -1
#define TYPE_LABEL "LABEL"
#define TYPE_VAR "VAR"

class Assembler {
	ifstream InputFile;
	ofstream OutputFile;

	int GetMnemonicType(const string &Line) {
		Scanner Sc(Line);
		if (Sc.EndReached() || Line.empty())
			return MNEMONIC_TYPE_IGNORE;
		else if (POT.IsPseudoOpcode(Line))
			return MNEMONIC_TYPE_PO;
		else if (MOT.IsMachineOpcode(Line))
			return MNEMONIC_TYPE_MO;
		else
			return MNEMONIC_TYPE_INVALID;
	}

	bool PassI() {
		int LC = 0, LineNumber = 1;
		string Line, Label, VarName;
		Errors.Clear();
		InputFile.clear();
		InputFile.seekg(0);

		while (InputFile.eof() == false) {
			getline(InputFile, Line);
			Scanner Sc(Line);
			Errors.SetCurrentLine(LineNumber);
			Label = Sc.GetLabel();
			if (Sc.IsLabelPresent() == true && Label.empty()) {
				Errors.Add(Errors.E_INVALID_LABEL);
				++LineNumber;
				continue;
			}

			switch (GetMnemonicType(Line)) {
				case MNEMONIC_TYPE_MO:
					if (Sc.IsLabelPresent())
						SYMTAB.Insert(Label, TYPE_LABEL, LC);
					LC += MOT.GetSize(Line);
					break;

				case MNEMONIC_TYPE_PO:
					if (Sc.IsLabelPresent())
						SYMTAB.Insert(Label, TYPE_LABEL, LC);
					VarName = POT.GetVariableName(Line);
					SYMTAB.Insert(VarName, TYPE_VAR, LC);
					LC += POT.GetOperandsSize(Line);
					break;

				case MNEMONIC_TYPE_IGNORE:
					break;

				case MNEMONIC_TYPE_INVALID:
					Errors.Add(Errors.E_INVALID_INSTRUCTION);
					break;

				default:
					Errors.Add("Something Terrible Happened");
					break;
			}
			++LineNumber;
		}
		return Errors.IsEmpty() ? true : false;
	}

	bool PassII() {
		int LC = 0, LineNumber = 1;
		string Line, Code;
		Errors.Clear();
		InputFile.clear();
		InputFile.seekg(0);
		OutputFile.seekp(0);
		while (InputFile.eof() == false) {
			getline(InputFile, Line);
			Errors.SetCurrentLine(LineNumber);

			switch (GetMnemonicType(Line)) {
				case MNEMONIC_TYPE_MO:
					Code = MOT.GetCode(Line);
					if (!Code.empty()) {
							OutputFile << Code << "                 : " << Line << endl;
						LC += MOT.GetSize(Line);
					}
					break;

				case MNEMONIC_TYPE_PO:
					Code = POT.GetCode(Line);
					if (!Code.empty()) {
						OutputFile << Code << "                 :" << Line << endl;
						LC += POT.GetOperandsSize(Line);
					}
					break;

				case MNEMONIC_TYPE_IGNORE:
					break;

				case MNEMONIC_TYPE_INVALID:
					Errors.Add("Something Terrible Happened");
					break;

				default:
					Errors.Add("Something Terrible Happened");
					break;
				}
				++LineNumber;
			}
			return Errors.IsEmpty() ? true : false;
		}

	public:
		bool Assemble(const string &InputFileName, const string &OutputFileName) {
			InputFile.open(InputFileName);
			if (InputFile.good()) {
				OutputFile.open(OutputFileName, ios::trunc);
				if (OutputFile.good()) {
					if (PassI() == true) {
						if (PassII() == true) {
							return true;
						} else {
							cout << "\n PassII failed";
							Errors.Display();
						}
					} else {
						cout << "\n PassI failed";
						Errors.Display();
					}
				} else
					cout << "\nError! Could not open output file for writting";
			} else
				cout << "\nError! Could not open input file for reading";

			if (InputFile.is_open())
				InputFile.close();

			if (OutputFile.is_open())
				OutputFile.close();

			return false;
		}
}AASM;

#endif
