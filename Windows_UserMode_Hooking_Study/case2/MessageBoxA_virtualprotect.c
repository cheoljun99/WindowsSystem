#include <stdio.h>
#include <windows.h>

int main(int argc, char* argv[])
{
    // GetModuleHandleA �Լ��� hModule�� ���Ѵ�.
    HMODULE hModule = GetModuleHandleA("user32.dll");

    // ���� GetModuleHandleA �Լ��� ������ ���Ͽ��ٸ� LoadLibraryA �Լ��� DLL�� �ε��Ѵ�.
    if (hModule == NULL)
        hModule = LoadLibraryA("user32.dll");

    // GetProcAddress �Լ��� �̿��� �ּҸ� ������
    PVOID pMessageBoxA = (PVOID)GetProcAddress(hModule, "MessageBoxA");

    // ���� �޸� ��ȣ �ɼ��� ������ ����
    DWORD OldProtect;

    /*
    VirtualProtect �Լ��� ����� MessageBoxA �ּҷκ��� 12����Ʈ ��ŭ�� �޸� ��ȣ �ɼ���
    PAGE_EXECUTE_READWRITE(ERW--)�� �����Ѵ�.
    */
    if (VirtualProtect(pMessageBoxA, 12, PAGE_EXECUTE_READWRITE, &OldProtect) == FALSE)
    {
        printf("VirtualProtect Failed\n");
        return -1;
    }

    printf("Changed the top 12 bytes memory protection option in MessageBoxA to ERW--\n");
}