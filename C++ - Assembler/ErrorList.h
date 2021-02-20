/*
 * ErrorList.h / Assembler
 * Author : Gourav Siddhad
 */
#ifndef APP_HEADER_ERROR_LIST
#define APP_HEADER_ERROR_LIST

#include "iostream"
#include "iomanip"
#include "cstring"
#include "vector"
using namespace std;

struct Error
{
	size_t LineNumber;
	string ErrorType;
};

class ErrorList
{
	vector<Error> List;
	size_t CurrentLine;

public:
	const string E_INVALID_INSTRUCTION = "Invalid Instruction";
	const string E_INVALID_OPERAND = "Operand is not valid for this instruction";
	const string E_INVALID_LABEL = "Not a valid Label name";
	const string E_MISSING_OPERAND = "Not sufficient arguments for instruction";
	const string E_SYMBOL_REDECLARED = "Symbol already defined";
	const string E_SYMBOL_NOT_DEFINED = "Symbol not defined";

	void SetCurrentLine(size_t CurrentLine)
	{
		this->CurrentLine = CurrentLine;
	}

	bool IsEmpty()
	{
		return List.empty();
	}

	void Add(size_t LineNumber, string ErrorType)
	{
		List.push_back({LineNumber, ErrorType});
	}

	void Add(string ErrorType)
	{
		List.push_back({CurrentLine, ErrorType});
	}

	void Clear()
	{
		List.clear();
	}

	void Display()
	{
		cout << "\n\n  Total Errors = " << List.size() << "\n\n";
		for (size_t i = 0; i < List.size(); i++)
		{
			cout << "[ Error " << setw(4) << i + 1 << " ] ";
			cout << "[ Line " << setw(5) << List[i].LineNumber << " ] ";
			cout << List[i].ErrorType << endl;
		}
	}
} Errors;

#endif
