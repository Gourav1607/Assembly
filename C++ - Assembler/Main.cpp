/*
 *	Main.cpp / Assembler
 *	Author : Gourav Siddhad
 */

#include "iostream"
#include "cstring"
#include "Assembler.h"

string InputFileName;
string OutputFileName;
string OutputExtention = ".obj";

void PrintHelp(string ProgramName) {
	cout << "\n AASM  2.4";
	cout << "\n\n Usage : " << ProgramName << " [/? | -h]";
	cout << "\n         " << ProgramName << " <InputFile> <OutputFile>";
	cout << "\n\n /? | -h        Show this help";
	cout << "\n InputFile      Assembly source code file name";
	cout << "\n OutputFile     Binary output file namen\n\n";
}

bool ParseArguments(int argc, char *argv[]) {
	if (argc > 1) {
		if (string(argv[1]).compare("/?") == 0
				|| string(argv[1]).compare("-h") == 0) {
			PrintHelp(argv[0]);
			exit(0);
		}
		InputFileName = string(argv[2]);

		if (argc > 2)
			OutputFileName = string(argv[3]);
		else
			OutputFileName = InputFileName + OutputExtention;
	}

	if (argc > 3)
		return false;
	else
		return true;
}

bool GetFileNames() {
	cout << "\n\nProvide input and output file names [Press Enter to Quit]";
	cout << "\n\nInput File : ";
	getline(cin, InputFileName);
	if (InputFileName.empty())
		return false;

	cout << "\nOutput File : ";
	getline(cin, OutputFileName);
	if (OutputFileName.empty())
		return false;

	return true;
}

int main(int argc, char *argv[]) {
	if (argc > 1) {
		if (ParseArguments(argc, argv) == true)
			if (AASM.Assemble(InputFileName, OutputFileName) == false) {
				cout << "\n\n";
				system("pause");
			} else
				PrintHelp(argv[0]);
	} else {
		if (GetFileNames() == true)
			if (AASM.Assemble(InputFileName, OutputFileName) == false) {
				cout << "\n\n";
				system("pause");
			}
	}
	return 0;
}
