#ifndef INJECTOR_H
#define INJECTOR_H

#include <windows.h>
#include <string>

bool InjectDLL(DWORD dwProcessID, const std::string& dllPath);

#endif // INJECTOR_H