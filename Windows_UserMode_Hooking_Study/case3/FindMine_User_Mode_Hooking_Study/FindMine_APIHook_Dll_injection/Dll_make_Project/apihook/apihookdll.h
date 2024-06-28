// apihookdll.h
#pragma once

#ifndef DLLHEADER_H
#define DLLHEADER_H

#include <stdio.h>
#include <Windows.h>
#include <tchar.h>

#endif

// CREATEDLL_EXPORTS��� ��ó���Ⱑ ���ǵǾ� �ִ��� Ȯ��
// ������ *.h ������ dll ���� ������Ʈ�� dll ȣ�� ������Ʈ���� ����ϱ� ���ؼ��̴�
#ifdef CREATEDLL_EXPORTS 
// CREATEDLL_EXPORTS�� ���ǵǾ� ������
// __declspec(dllexport) Ű���尡 ���� �Լ��� DLL �ܺη� ����ǹǷ�, DLL�� ����� __declspec(dllexport) Ű���尡 ���� �Լ��� ȣ���� �� �ִٴ� �ǹ��̴�.
// __declspec(dllexport) Ű���尡 ���� �Լ��� ���ǵǾ� �־ DLL �ܺο��� ȣ���� �� ����.
#define MYMATH_DECLSPEC __declspec(dllexport)
#else
// CREATEDLL_EXPORTS�� ���ǵǾ� ���� ���� ��
// __declspec(dllimport) Ű����� DLL�� ���ǵ� �Լ��� ȣ���ϱ� ���� ���ȴ�.
// ��, DLL���� __declspec(dllexport)�� ���ǵ� �Լ��� __declspec(dllimport)�� ȣ���Ѵٴ� ���̴�.
#define MYMATH_DECLSPEC __declspec(dllimport)
#endif

// Name Mangling : �����Ϸ��� �Լ��� �̸��� ������ ��Ģ�� ������ �����ϴ� ��
// DLL�� ����Ͽ� ����� ��ŷ(Explicit Linking)�� �� ��� ȣ���� �Լ��� �̸��� �˾ƾ� �ϴµ�,
// �Լ��� �̸��� DLL�� �����Ǹ鼭 �ͱ۸� �Ǹ� ȣ���ϱⰡ �����Ƿ� extern "C" Ű���带 �ٿ� ���� �ͱ۸��� �������� �ʰڴٰ� �����ϴ� ���̴�.
extern "C" MYMATH_DECLSPEC DWORD WINAPI ShowMessage(LPVOID lParam);
//extern "C" MYMATH_DECLSPEC BOOL WINAPI DllMain(HINSTANCE hinstDll, DWORD fdwReason, LPVOID lpvReserved)