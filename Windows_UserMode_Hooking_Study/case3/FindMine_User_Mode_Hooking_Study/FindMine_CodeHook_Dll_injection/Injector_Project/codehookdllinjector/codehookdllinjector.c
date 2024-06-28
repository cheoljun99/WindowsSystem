#include <stdio.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <tchar.h>

BOOL GetProcessPid(HANDLE* hProcessSnap, DWORD* PID, PROCESSENTRY32* pe32, LPCTSTR* DllPath);
BOOL InjectDll(HANDLE hProcess, DWORD PID, LPCTSTR* DllPath);

int main(void)
{
	HANDLE hProcessSnap, hProcess = 0;
	PROCESSENTRY32 pe32 = { 0 }; // ���μ��� ������ ��� ����ü
	DWORD PID;
	//LPCTSTR DllPath = "C:\\general.dll";
	char* DllPath = "C:\\codehook.dll";// C����̺꿡 ������ dll �ֱ�

	GetProcessPid(&hProcessSnap, &PID, &pe32, &DllPath); // ���μ����� �ڵ� ������
	InjectDll(hProcess, PID, &DllPath);
}


BOOL GetProcessPid(HANDLE* hProcessSnap, DWORD* PID, PROCESSENTRY32* pe32, LPCTSTR* DllPath)
{
	// �ý��ۿ��� ���� ���� ���� ���μ����� ������ ��� �ͼ� �������� �����.
	hProcessSnap = (HANDLE*)CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		_tprintf(_T("CreateToolhelp32Snapshot (of processes)"));
		return(FALSE);
	}

	// Set the size of the structure before using it.
	pe32->dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (Process32First(hProcessSnap, pe32))
	{
		do
		{
			if (!_tcsicmp(pe32->szExeFile, _T("����ã��.exe")))
			{
				*PID = pe32->th32ProcessID;
				_tprintf(_T("[*] Process Name : %s\n"), pe32->szExeFile);
				_tprintf(_T("[*] PID is : %u\n\n"), *PID);
				break;
			}
		} while (Process32Next(hProcessSnap, pe32));
		CloseHandle(hProcessSnap);
	}
	else
	{
		_tprintf(_T("Process32First error! Error Code is : %d\n"), GetLastError()); // show cause of failure
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return(FALSE);
	}

}


BOOL InjectDll(HANDLE hProcess, DWORD PID, LPCTSTR* DllPath)
{
	HANDLE hThread;
	LPVOID pRemoteBuf;
	HMODULE hMod = 0;
	LPTHREAD_START_ROUTINE pThreadProc;

	// #1. PID �� �̿��Ͽ� ��� ���μ����� HANDLE�� ���Ѵ�.
	if (!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID)))
	{
		_tprintf(_T("OpenProcess(%d) failed!!! [%d]\n"), PID, GetLastError());
		return FALSE;
	}

	// #2. ��� ���μ��� �޸𸮿� DllPath ũ�⸸ŭ �޸𸮸� �Ҵ��Ѵ�.
	SIZE_T pathSize = (_tcslen(DllPath) + 1) * sizeof(TCHAR); //�����κ�

	if (!(pRemoteBuf = VirtualAllocEx(hProcess, NULL, pathSize, MEM_COMMIT, PAGE_READWRITE)))
		_tprintf(_T("VirtualAllocEx() Failed!!\n"));
	else _tprintf(_T("-> Virtual Memory is : %x\n"), (unsigned int)pRemoteBuf);

	// #3. �Ҵ� ���� �޸𸮿� ����ã��_ex.dll ��θ� ����.
	if (!(WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)*DllPath, pathSize, NULL)))
		_tprintf(_T("WriteProcessMemory() failed!!\n"));

	// #4. KERNEL �ڵ� ���ϱ�
	if (!(hMod = GetModuleHandle(_T("Kernel32.dll"))))
		_tprintf(_T("GetModuleHandle() Failed!!, Error Code is : %d\n"), GetLastError());
	else _tprintf(_T("-> KERNEL.dll memory is : %x\n"), (unsigned int)hMod);

	// #5. LoadLibraryA() API �ּҸ� ���Ѵ�.
	if (!(pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryA")))
		_tprintf(_T("GetProcAddress() Failed!!, Error Code is : %d\n"), GetLastError());
	else _tprintf(_T("-> KERNEL32.LoadLibraryW : % x\n"), pThreadProc);

	// #6. notepad.exe ���μ����� �����带 ����
	if (!(hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pRemoteBuf, 0, NULL)))
		_tprintf(_T("CreateRemoteThread() Failed!!, Error Code is : %d\n"), GetLastError());
	WaitForSingleObject(hThread, INFINITE);

	_tprintf(_T("-> Thread Handle is : %x\n\n"), hThread);
}