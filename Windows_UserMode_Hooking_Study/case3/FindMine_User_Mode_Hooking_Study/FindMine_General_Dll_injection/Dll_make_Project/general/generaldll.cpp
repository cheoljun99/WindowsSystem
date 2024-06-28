#include "generaldll.h"

DWORD WINAPI ShowMessage(LPVOID lParam)
{
	MessageBox(NULL, _T("DLL Injection Success"), _T("MessageBox"), MB_OK);
	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved)
{
	HANDLE hThread = NULL;

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		OutputDebugString(_T("<Áö·ÚÃ£±â.dll> Injection!!!"));
		hThread = CreateThread(NULL, 0, ShowMessage, NULL, 0, NULL);
		CloseHandle(hThread);
		break;
	}

	return TRUE;
}