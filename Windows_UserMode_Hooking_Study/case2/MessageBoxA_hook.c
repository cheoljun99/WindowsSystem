#include <stdio.h>
#include <windows.h>

BYTE HookCode[12] = { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0 };
BYTE OriginCode[12] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

int __stdcall NewMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
    // 상위 5 바이트를 원본 코드로 패치
    memcpy(MessageBoxA, OriginCode, 12);
    // lpText 변수를 조작해 함수를 정상적으로 실행
    int ret = MessageBoxA(hWnd, "Hooked..!", lpCaption, uType);
    // 다시 후킹 코드로 패치
    memcpy(MessageBoxA, HookCode, 12);

    // 반환값 전달
    return ret;
}

int main(int argc, char* argv[])
{
    
    // GetModuleHandleA 함수로 hModule을 구한다.
    HMODULE hModule = GetModuleHandleA("user32.dll");

    // 만약 GetModuleHandleA 함수로 구하지 못하였다면 LoadLibraryA 함수로 DLL을 로드한다.
    if (hModule == NULL)
        hModule = LoadLibraryA("user32.dll");

    if (hModule == NULL)
    {
        printf("[-] user32.dll not found\n");
        return -1;
    }

    printf("[+] user32.dll found!\n");

    // GetProcAddress 함수를 이용해 주소를 가져옴
    PVOID pMessageBoxA = (PVOID)GetProcAddress(hModule, "MessageBoxA");

    if (pMessageBoxA == NULL)
    {
        printf("[-] MessageBoxA not found\n");
        return -1;
    }

    printf("[+] MessageBoxA found!\n");

    // 원본 메모리 보호 옵션을 저장할 변수
    DWORD OldProtect;

    /*
    VirtualProtect 함수를 사용해 MessageBoxA 주소로부터 5바이트 만큼의 메모리 보호 옵션을
    PAGE_EXECUTE_READWRITE(ERW--)로 변경한다.
    */
    if (VirtualProtect(pMessageBoxA, 12, PAGE_EXECUTE_READWRITE, &OldProtect) == FALSE)
    {
        printf("VirtualProtect Failed\n");
        return -1;
    }

    printf("[*] Changed the top 12 bytes memory protection option in MessageBoxA to ERW--\n");

    // 점프할 주소를 저장
    *(ULONGLONG*)(HookCode + 2) = (ULONGLONG)NewMessageBoxA;

    printf("[*] Patch code :    ");

    for (int i = 0; i < 12; i++)
        printf("0x%02X ", HookCode[i]);

    putchar('\n');

    // 원본 12이트 백업
    memcpy(OriginCode, pMessageBoxA, 12);
    printf("[*] Original code : ");

    for (int i = 0; i < 12; i++)
        printf("0x%02X ", OriginCode[i]);

    putchar('\n');

    // 후킹 코드로 패치
    memcpy(pMessageBoxA, HookCode, 12);

    printf("[+] Trampoline Hooking Success!\n");
    printf("Press the any key...");
    getchar();

    // 후킹된 MessageBoxA 함수 호출
    MessageBoxA(NULL, "Not Hooked...!", "Trampoline Hook", 0);

    return 0;
}