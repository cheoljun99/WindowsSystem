#include "apihookdll.h"

DWORD WINAPI APIHooking(LPVOID lParam)
{
	HMODULE hMod;
	LPVOID TargetAddr;
	HANDLE hThread = NULL;
	DWORD OldProtect = 0;

	// NtUserSetTimer API �ּ� ���ϱ�
	hMod = GetModuleHandle(TEXT("win32u.dll"));
	TargetAddr = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "NtUserSetCapture");

	// 12����Ʈ ���� PAGE_READWRITE�� ����
	VirtualProtect(TargetAddr, 12, PAGE_READWRITE, &OldProtect);

	// win32u.NtUserSetCapture API EP �ڵ� ��ŷ
	*((LPBYTE)TargetAddr + 0) = 0x6A;
	*((LPBYTE)TargetAddr + 1) = 0x0A;
	*((LPBYTE)TargetAddr + 2) = 0x90;
	*((LPBYTE)TargetAddr + 3) = 0x90;
	*((LPBYTE)TargetAddr + 4) = 0x90;
	*((LPBYTE)TargetAddr + 5) = 0xE8;
	*((LPBYTE)TargetAddr + 6) = 0x56;
	*((LPBYTE)TargetAddr + 7) = 0xFF;
	*((LPBYTE)TargetAddr + 8) = 0x2D;
	*((LPBYTE)TargetAddr + 9) = 0x8A;
	*((LPBYTE)TargetAddr + 10) = 0x90;
	*((LPBYTE)TargetAddr + 11) = 0x90;

	// ������ ���� ����
	VirtualProtect(TargetAddr, 12, OldProtect, &OldProtect);

	return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
	HANDLE hThread = NULL;

	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
		hThread = CreateThread(NULL, 0, APIHooking, NULL, 0, NULL);
	}
	return TRUE;
}