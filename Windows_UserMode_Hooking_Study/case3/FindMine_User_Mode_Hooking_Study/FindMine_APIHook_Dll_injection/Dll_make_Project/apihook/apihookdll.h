// apihookdll.h
#pragma once

#ifndef DLLHEADER_H
#define DLLHEADER_H

#include <stdio.h>
#include <Windows.h>
#include <tchar.h>

#endif

// CREATEDLL_EXPORTS라는 전처리기가 정의되어 있는지 확인
// 동일한 *.h 파일을 dll 생성 프로젝트와 dll 호출 프로젝트에서 사용하기 위해서이다
#ifdef CREATEDLL_EXPORTS 
// CREATEDLL_EXPORTS가 정의되어 있을때
// __declspec(dllexport) 키워드가 붙은 함수는 DLL 외부로 노출되므로, DLL을 사용해 __declspec(dllexport) 키워드가 붙은 함수를 호출할 수 있다는 의미이다.
// __declspec(dllexport) 키워드가 없는 함수는 정의되어 있어도 DLL 외부에서 호출할 수 없다.
#define MYMATH_DECLSPEC __declspec(dllexport)
#else
// CREATEDLL_EXPORTS가 정의되어 있지 않을 때
// __declspec(dllimport) 키워드는 DLL에 정의된 함수를 호출하기 위해 사용된다.
// 즉, DLL에서 __declspec(dllexport)로 정의된 함수를 __declspec(dllimport)로 호출한다는 것이다.
#define MYMATH_DECLSPEC __declspec(dllimport)
#endif

// Name Mangling : 컴파일러가 함수의 이름을 일정한 규칙을 가지고 변형하는 것
// DLL을 사용하여 명시적 링킹(Explicit Linking)을 할 경우 호출할 함수의 이름을 알아야 하는데,
// 함수의 이름이 DLL로 생성되면서 맹글링 되면 호출하기가 어려우므로 extern "C" 키워드를 붙여 네임 맹글링을 수행하지 않겠다고 정의하는 것이다.
extern "C" MYMATH_DECLSPEC DWORD WINAPI ShowMessage(LPVOID lParam);
//extern "C" MYMATH_DECLSPEC BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved)