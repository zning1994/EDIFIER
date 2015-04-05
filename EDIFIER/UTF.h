/**
*��UTF-8��ʽ����ת����ASCII��ʽ����
**/
#include <windows.h>
#ifndef _UTF_H_
#define _UTF_H_

wstring UTF8ToUnicode(const char *utfStr)
{
	int unicodeLen = ::MultiByteToWideChar(CP_UTF8,
		0,
		utfStr,
		-1,
		NULL,
		0);
	wchar_t *pUnicode;
	pUnicode = new wchar_t[unicodeLen + 1];
	memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
	::MultiByteToWideChar(CP_UTF8,
		0,
		utfStr,
		-1,
		(LPWSTR)pUnicode,
		unicodeLen);

	wstring unicodeStr;
	unicodeStr = (wchar_t*)pUnicode;
	delete pUnicode;

	return unicodeStr;
}

#endif