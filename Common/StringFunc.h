#ifndef __STRING_FUNCTION_H__
#define __STRING_FUNCTION_H__

#include "std_inc.h"


TString TStringFromChars(const char *ansiChars);
TString TStringFromWChars(const WCHAR *wChars);

int AnsiCharsFromTString(char *buf, UINT bufSize, const TString& cstr);
int WCharsFromTString(WCHAR *buf, UINT bufSize, const TString& cstr);
std::string AnsiStringFromTString(const TString& cstr);
TString TStringFromAnsiString(const std::string& ansiString);

SStringT GetFileBaseName(LPCTSTR lpszPathName);

#endif //__STRING_FUNCTION_H__