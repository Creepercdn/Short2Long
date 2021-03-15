#include <stdio.h>
#include <Windows.h>
#include <fileapi.h>
#include <strsafe.h>

void ErrorExit(LPTSTR lpszFunction)
{
	// Retrieve the system error message for the last-error code

	LPVOID lpMsgBuf;
	LPVOID lpDisplayBuf;
	DWORD dw = GetLastError();

	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM |
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		dw,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf,
		0, NULL);

	// Display the error message and exit the process

	lpDisplayBuf = (LPVOID)LocalAlloc(LMEM_ZEROINIT,
		(lstrlen((LPCTSTR)lpMsgBuf) + lstrlen((LPCTSTR)lpszFunction) + 40) * sizeof(TCHAR));
	StringCchPrintf((LPTSTR)lpDisplayBuf,
		LocalSize(lpDisplayBuf) / sizeof(TCHAR),
		TEXT("%s failed with error %d: %s"),
		lpszFunction, dw, lpMsgBuf);
	MessageBox(NULL, (LPCTSTR)lpDisplayBuf, TEXT("Error"), MB_OK);

	LocalFree(lpMsgBuf);
	LocalFree(lpDisplayBuf);
	ExitProcess(dw);
}

int main(int argc, char* argv[])
{
	bool rFLAG = 0;
	char* lpszPath;

	if (argc < 2 || argc > 3) {
		printf("%s\n%s %s\n%s\n","Usage:", argv[0], "[/r] <path>","/r:long to short");
		exit(0);
	}
	if (argc==3) {
		rFLAG = 1;
		lpszPath = argv[2];
	}
	else {
		lpszPath = argv[1];
	}

	long length = 0;
	TCHAR* buffer = NULL;

	if (rFLAG) {
		length = GetShortPathName(lpszPath, NULL, 0);
		if (length == 0)
			ErrorExit(TEXT((LPTSTR)"GetShortPathName"));
	}
	else {
		length = GetLongPathName(lpszPath, NULL, 0);
		if (length == 0) {
			ErrorExit(TEXT((LPTSTR)"GetLongPathName"));
		}
	}

	buffer = new TCHAR[length];

	if (rFLAG) {
		length = GetShortPathName(lpszPath, buffer, length);
		if (length == 0)
			ErrorExit(TEXT((LPTSTR)"GetShortPathName"));
	}
	else {
		length = GetLongPathName(lpszPath, buffer, length);
		if (length == 0) {
			ErrorExit(TEXT((LPTSTR)"GetLongPathName"));
		}
	}

	printf("%s\n", buffer);

	delete[] buffer;
	return 0;
}
