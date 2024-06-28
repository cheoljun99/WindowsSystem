#include "stdio.h"
#include "windows.h"

DWORD WINAPI CodeHooking(LPVOID lParam)
{
	LPVOID TargetAddr = (LPVOID)0x1001FAE;
	DWORD OldProtect = 0;

	// 0x1001FAE 부터 8바이트 -> 권한 PAGE_READWRITE
	VirtualProtect(TargetAddr, 8, PAGE_READWRITE, &OldProtect);

	// [FF 75 14 E8 56 F4 FF FF] -> [6A 0A 90 E8 CA 0F 00 00]
	*((LPBYTE)TargetAddr + 0) = 0x6A;
	*((LPBYTE)TargetAddr + 1) = 0x0A;
	*((LPBYTE)TargetAddr + 2) = 0x90;
	*((LPBYTE)TargetAddr + 3) = 0xE8;
	*((LPBYTE)TargetAddr + 4) = 0xCA;
	*((LPBYTE)TargetAddr + 5) = 0x0F;
	*((LPBYTE)TargetAddr + 6) = 0x00;
	*((LPBYTE)TargetAddr + 7) = 0x00;

	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	HANDLE hThread = NULL;
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		hThread = CreateThread(NULL, 0, CodeHooking, NULL, 0, NULL);
		CloseHandle(hThread);
	}
	return TRUE;
}