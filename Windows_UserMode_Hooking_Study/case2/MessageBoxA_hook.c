#include <stdio.h>
#include <windows.h>

BYTE HookCode[12] = { 0x48, 0xB8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xE0 };
BYTE OriginCode[12] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

int __stdcall NewMessageBoxA(HWND hWnd, LPCSTR lpText, LPCSTR lpCaption, UINT uType)
{
    // ���� 5 ����Ʈ�� ���� �ڵ�� ��ġ
    memcpy(MessageBoxA, OriginCode, 12);
    // lpText ������ ������ �Լ��� ���������� ����
    int ret = MessageBoxA(hWnd, "Hooked..!", lpCaption, uType);
    // �ٽ� ��ŷ �ڵ�� ��ġ
    memcpy(MessageBoxA, HookCode, 12);

    // ��ȯ�� ����
    return ret;
}

int main(int argc, char* argv[])
{
    
    // GetModuleHandleA �Լ��� hModule�� ���Ѵ�.
    HMODULE hModule = GetModuleHandleA("user32.dll");

    // ���� GetModuleHandleA �Լ��� ������ ���Ͽ��ٸ� LoadLibraryA �Լ��� DLL�� �ε��Ѵ�.
    if (hModule == NULL)
        hModule = LoadLibraryA("user32.dll");

    if (hModule == NULL)
    {
        printf("[-] user32.dll not found\n");
        return -1;
    }

    printf("[+] user32.dll found!\n");

    // GetProcAddress �Լ��� �̿��� �ּҸ� ������
    PVOID pMessageBoxA = (PVOID)GetProcAddress(hModule, "MessageBoxA");

    if (pMessageBoxA == NULL)
    {
        printf("[-] MessageBoxA not found\n");
        return -1;
    }

    printf("[+] MessageBoxA found!\n");

    // ���� �޸� ��ȣ �ɼ��� ������ ����
    DWORD OldProtect;

    /*
    VirtualProtect �Լ��� ����� MessageBoxA �ּҷκ��� 5����Ʈ ��ŭ�� �޸� ��ȣ �ɼ���
    PAGE_EXECUTE_READWRITE(ERW--)�� �����Ѵ�.
    */
    if (VirtualProtect(pMessageBoxA, 12, PAGE_EXECUTE_READWRITE, &OldProtect) == FALSE)
    {
        printf("VirtualProtect Failed\n");
        return -1;
    }

    printf("[*] Changed the top 12 bytes memory protection option in MessageBoxA to ERW--\n");

    // ������ �ּҸ� ����
    *(ULONGLONG*)(HookCode + 2) = (ULONGLONG)NewMessageBoxA;

    printf("[*] Patch code :    ");

    for (int i = 0; i < 12; i++)
        printf("0x%02X ", HookCode[i]);

    putchar('\n');

    // ���� 12��Ʈ ���
    memcpy(OriginCode, pMessageBoxA, 12);
    printf("[*] Original code : ");

    for (int i = 0; i < 12; i++)
        printf("0x%02X ", OriginCode[i]);

    putchar('\n');

    // ��ŷ �ڵ�� ��ġ
    memcpy(pMessageBoxA, HookCode, 12);

    printf("[+] Trampoline Hooking Success!\n");
    printf("Press the any key...");
    getchar();

    // ��ŷ�� MessageBoxA �Լ� ȣ��
    MessageBoxA(NULL, "Not Hooked...!", "Trampoline Hook", 0);

    return 0;
}