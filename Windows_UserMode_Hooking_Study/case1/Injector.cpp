#include <windows.h>
#include <iostream>
#include <string>

bool InjectDLL(DWORD dwProcessID, const std::string& dllPath) {
    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
    if (hProcess == NULL) {
        std::cerr << "OpenProcess failed. Error: " << GetLastError() << std::endl;
        return false;
    }

    LPVOID pRemoteMemory = VirtualAllocEx(hProcess, NULL, dllPath.size() + 1, MEM_COMMIT, PAGE_READWRITE);
    if (pRemoteMemory == NULL) {
        std::cerr << "VirtualAllocEx failed. Error: " << GetLastError() << std::endl;
        CloseHandle(hProcess);
        return false;
    }

    if (!WriteProcessMemory(hProcess, pRemoteMemory, dllPath.c_str(), dllPath.size() + 1, NULL)) {
        std::cerr << "WriteProcessMemory failed. Error: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0,
        (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandleA("kernel32.dll"), "LoadLibraryA"),
        pRemoteMemory, 0, NULL);
    if (hThread == NULL) {
        std::cerr << "CreateRemoteThread failed. Error: " << GetLastError() << std::endl;
        VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
        CloseHandle(hProcess);
        return false;
    }

    WaitForSingleObject(hThread, INFINITE);

    VirtualFreeEx(hProcess, pRemoteMemory, 0, MEM_RELEASE);
    CloseHandle(hThread);
    CloseHandle(hProcess);

    return true;
}

int main() {
    DWORD dwProcessID; // 후킹할 프로세스 ID
    std::string dllPath = "C:\\path\\to\\hook.dll"; // 생성한 후킹 DLL 경로

    std::cout << "Enter the process ID to inject DLL into: ";
    std::cin >> dwProcessID;

    if (InjectDLL(dwProcessID, dllPath)) {
        std::cout << "DLL injection successful." << std::endl;
    }
    else {
        std::cerr << "DLL injection failed." << std::endl;
    }

    return 0;
}
