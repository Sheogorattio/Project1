#include <windows.h>
#include <tchar.h>
#include <string>
#include <commctrl.h>
#include <tlhelp32.h>
#include <fstream>
#include"Timer.h"
#include "resource.h"

#pragma comment(lib,"comctl32")

using namespace std;

BOOL CALLBACK DlgProc(HWND, UINT, WPARAM, LPARAM);

//HWND hReload;
HWND hTerminate;
HWND hNew;
HWND hProcName;
HWND hProcList;
HANDLE hThreadTimer;

DWORD WINAPI TimerProc(LPARAM lp) {
	Timer t;
	t.DlProc(HWND(lp), WM_INITDIALOG, 0, 0);
	while (TRUE) {
		t.DlProc(t.hTimeEdit, WM_TIMER, 0, 0);
	}
	return 0;
}

int WINAPI _tWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPTSTR lpszCmdLine, int nCmdShow)
{
	INITCOMMONCONTROLSEX icc = { sizeof(INITCOMMONCONTROLSEX) };
	icc.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&icc);

	return DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DLGPROC)DlgProc);
}

void ShowProcList(HWND hList) {
	//EnableWindow(hTerminate, FALSE);
	EnableWindow(hNew, TRUE);

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 processInfo = { sizeof(PROCESSENTRY32) };

	BOOL res = Process32First(hSnapshot, &processInfo);

	if (res == TRUE) {
		do {
			LRESULT index = SendMessage(hList, LB_ADDSTRING, 0, LPARAM(processInfo.szExeFile));
			SendMessage(hList, LB_SETITEMDATA, WPARAM(index), LPARAM(processInfo.th32ProcessID));
		} while (Process32Next(hSnapshot, &processInfo));
	}

	CloseHandle(hSnapshot);
}

BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp)
{
	switch (mes)
	{
	case WM_INITDIALOG:
	{	
		Timer t;
		hThreadTimer = CreateThread(NULL, 0,(LPTHREAD_START_ROUTINE)TimerProc, hWnd, 0, NULL);
		//hReload = GetDlgItem(hWnd, IDC_BUTTON1);
		hTerminate = GetDlgItem(hWnd, IDC_BUTTON2);
		hNew = GetDlgItem(hWnd, IDC_BUTTON3);
		hProcName = GetDlgItem(hWnd, IDC_EDIT1);
		hProcList = GetDlgItem(hWnd, IDC_LIST1);
	   	ShowProcList(hProcList);
		SetTimer(hWnd, 0, 10000, 0);
	}
	return TRUE;

	case WM_TIMER://WM_COMMAND:
	{
		//if (LOWORD(wp) == IDC_BUTTON1) {
			SendMessage(hProcList, LB_RESETCONTENT, 0, 0);
			ShowProcList(hProcList);


			wofstream ToFile("Proc. list.txt", ios::in | ios::trunc);
			TCHAR ProcName[255];
			HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
			PROCESSENTRY32 processInfo = { sizeof(PROCESSENTRY32) };
			BOOL check = Process32First(hSnapshot, &processInfo);
			if (check) {
				for (int i = 0; Process32Next(hSnapshot, &processInfo); i++) {
					SendMessage(hProcList, LB_GETTEXT, i, LPARAM(ProcName));
					ToFile << endl << ProcName;
				}
			}


		//}
		else if (LOWORD(wp) == IDC_BUTTON2) {
			LRESULT index = SendMessage(hProcList, LB_GETCURSEL, 0, 0);
			DWORD procId = SendMessage(hProcList, LB_GETITEMDATA, WPARAM(index), 0);
			HANDLE procHandle = OpenProcess(PROCESS_TERMINATE, TRUE, procId);
			TerminateProcess(procHandle, -0);
			CloseHandle(procHandle);
		}
		else if (LOWORD(wp) == IDC_BUTTON3) 
		{
			wchar_t buf[260]{ 0 };
			GetWindowText(hProcName, buf, 260);
			STARTUPINFO info = { sizeof(info) };
			PROCESS_INFORMATION processInfo;
			if (CreateProcess(buf, buf, NULL, NULL, TRUE, 0, NULL, NULL, &info, &processInfo))
			{
				ShowProcList(hProcList);
			}
		}
		else if (HIWORD(wp) == LBN_SELCHANGE) {
			EnableWindow(hTerminate, TRUE);
			EnableWindow(hNew, TRUE);

			LRESULT index = SendMessage(hProcList, LB_GETCURSEL, 0, 0);

			wchar_t buf[260]{ 0 };
			SendMessage(hProcList, LB_GETTEXT, WPARAM(index), LPARAM(buf));

			SetWindowText(hProcName, buf);
		}
	}
	break;

	case WM_CLOSE:
		TerminateThread(hThreadTimer, 0);
		KillTimer(hWnd, 0);
		DestroyWindow(hWnd);
		EndDialog(hWnd, 0);
		return TRUE;
	}
	return FALSE;
}

