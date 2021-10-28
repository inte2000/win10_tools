#pragma once

#include <vector>
#include <Windows.h>

class CHookShellTray;

typedef struct tagSubclassItem
{
	HWND hWnd;
	CHookShellTray*pHooker;
}SubclassItem;


void InitSubclassMgmt();
void ReleaseSubclassMgmt();
BOOL AddSubclassItem(HWND hWnd, CHookShellTray*pHooker);
void RemoveSubclassItem(HWND hWnd);
BOOL IsWndSubclassed(HWND hWnd);
CHookShellTray*FindSubclassWnd(HWND hWnd);
