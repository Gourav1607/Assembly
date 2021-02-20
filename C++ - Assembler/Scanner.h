/*
 *  Scanner.h / Assembler
 *  Author : Gourav Siddhad
 */

#ifndef APP_HEADER_SCANNER
#define APP_HEADER_SCANNER

#include "cstring"
#include "vector"
#include "cctype"
using namespace std;

class Scanner
{
	string Label;
	vector<string> Tokens;
	size_t TokenIndex = 0;

	void ApplyKungfu(string Line)
	{
		size_t i = 0;

		i = Line.find_first_of(';');
		if (i != Line.npos)
			Line.erase(i, Line.npos);

		for (size_t i = 0; i < Line.length();)
			if (isspace(Line[i]))
				Line.erase(Line.begin() + i);
			else
				break;

		i = Line.find_first_of(':');
		if (i != Line.npos)
		{
			Label = Line.substr(0, i);
			Line.erase(0, i + 1);
		}

		i = 0;
		while (i < Line.length())
		{
			string Token;
			while (i < Line.length() && (Line[i] == ' ' || Line[i] == '\t' || Line[i] == ','))
				i++;

			while (i < Line.length() && Line[i] != ' ' && Line[i] != '\t' && Line[i] != ',')
			{
				Token.push_back(Line[i]);
				i++;
			}

			if (!Token.empty())
				Tokens.push_back(Token);
		}
	}

public:
	Scanner(const string &Line)
	{
		SetLine(Line);
	}

	void SetLine(const string &Line)
	{
		TokenIndex = 0;
		Tokens.clear();
		Label.clear();

		ApplyKungfu(Line);
	}

	bool EndReached()
	{
		if (TokenIndex >= Tokens.size())
			return true;
		else
			return false;
	}

	void ResetTokenIndex()
	{
		TokenIndex = 0;
	}

	bool IsWord(const string &Token)
	{
		bool Result = true;
		if (Token.empty())
			Result = false;

		for (size_t i = 0; i < Token.length(); i++)
			if (!isalpha(Token[i]))
			{
				Result = false;
				break;
			}
		return Result;
	}

	bool IsInt(const string &Token)
	{
		bool Result = true;
		if (Token.empty())
			Result = false;

		for (size_t i = 0; i < Token.length(); i++)
			if (!isdigit(Token[i]))
			{
				Result = false;
				break;
			}
		return Result;
	}

	bool IsIdentifier(const string &Token)
	{
		bool Result = true;
		if (Token.empty())
			Result = false;

		if (Token.length() > 0 && !isalpha(Token[0]) && Token[0] != '_')
			Result = false;

		for (size_t i = 1; i < Token.length(); i++)
			if (!isalnum(Token[i]))
			{
				Result = false;
				break;
			}
		return Result;
	}

	bool IsLabelPresent()
	{
		return !Label.empty();
	}

	string GetLabel()
	{
		if (IsIdentifier(Label))
			return Label;
		else
			return "";
	}

	string GetWord()
	{
		string Word;
		if (!EndReached() && IsWord(Tokens[TokenIndex]))
		{
			Word = Tokens[TokenIndex];
			++TokenIndex;
		}
		return Word;
	}

	int GetInt()
	{
		int x = -1;
		if (!EndReached() && IsInt(Tokens[TokenIndex]))
		{
			x = stoi(Tokens[TokenIndex]);
			++TokenIndex;
		}
		return x;
	}

	string GetIdentifier()
	{
		string Id;
		if (!EndReached() && IsIdentifier(Tokens[TokenIndex]))
		{
			Id = Tokens[TokenIndex];
			++TokenIndex;
		}
		return Id;
	}

	string GetToken()
	{
		string Token;
		if (!EndReached())
		{
			Token = Tokens[TokenIndex];
			++TokenIndex;
		}
		return Token;
	}

	static bool CheckEqual(const string &Str1, const string &Str2)
	{
		if (Str1.length() != Str2.length())
			return false;
		for (size_t i = 0; i < Str1.length(); i++)
		{
			if (tolower(Str1[i]) != tolower(Str2[i]))
				return false;
		}
		return true;
	}
};

#endif
