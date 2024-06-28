#include <stdio.h>
#include <windows.h>

int main(int argc, char* argv[])
{
    // GetModuleHandleA 함수로 hModule을 구한다.
    HMODULE hModule = GetModuleHandleA("user32.dll");

    // 만약 GetModuleHandleA 함수로 구하지 못하였다면 LoadLibraryA 함수로 DLL을 로드한다.
    if (hModule == NULL)
        hModule = LoadLibraryA("user32.dll");

    // GetProcAddress 함수를 이용해 주소를 가져옴
    PVOID pMessageBoxA = (PVOID)GetProcAddress(hModule, "MessageBoxA");

    // 원본 메모리 보호 옵션을 저장할 변수
    DWORD OldProtect;

    /*
    VirtualProtect 함수를 사용해 MessageBoxA 주소로부터 12바이트 만큼의 메모리 보호 옵션을
    PAGE_EXECUTE_READWRITE(ERW--)로 변경한다.
    */
    if (VirtualProtect(pMessageBoxA, 12, PAGE_EXECUTE_READWRITE, &OldProtect) == FALSE)
    {
        printf("VirtualProtect Failed\n");
        return -1;
    }

    printf("Changed the top 12 bytes memory protection option in MessageBoxA to ERW--\n");
}