#include <windows.h>
#include <iostream>

typedef int (WINAPI* MessageBoxA_t)(HWND, LPCSTR, LPCSTR, UINT);
MessageBoxA_t pOriginalMessageBoxA = NULL;

int WINAPI HookedMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType) {
    std::cout << "MessageBoxA Hooked: " << lpText << std::endl;
    return pOriginalMessageBoxA(hWnd, lpText, lpCaption, uType);
}

void InstallHook() {
    HMODULE hUser32 = GetModuleHandleA("user32.dll");
    pOriginalMessageBoxA = (MessageBoxA_t)GetProcAddress(hUser32, "MessageBoxA");

    DWORD oldProtect;
    VirtualProtect(pOriginalMessageBoxA, 5, PAGE_EXECUTE_READWRITE, &oldProtect);

    DWORD relativeAddress = ((DWORD)HookedMessageBoxA - (DWORD)pOriginalMessageBoxA - 5);
    *((BYTE*)pOriginalMessageBoxA) = 0xE9;
    *((DWORD*)((BYTE*)pOriginalMessageBoxA + 1)) = relativeAddress;

    VirtualProtect(pOriginalMessageBoxA, 5, oldProtect, &oldProtect);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        InstallHook();
    }
    return TRUE;
}
