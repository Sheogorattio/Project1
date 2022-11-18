#include "Timer.h"
#include<ctime>
#include<windowsx.h>

Timer* Timer::ptr = NULL;

Timer::Timer()
{
	ptr = this;
}

BOOL Timer::DlProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message) {
		//HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInit);
		//HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);

		case WM_INITDIALOG: {
			SetTimer(hwnd, 0, 1000, 0);
			ptr->hTimeEdit = GetDlgItem(hwnd, IDC_TimeEdit);
			return TRUE;
		}
		case WM_CLOSE: {
			KillTimer(hwnd, 0);
			EndDialog(hwnd, 0);
			break;
		}
		case WM_TIMER: {
			LONGLONG t = time(NULL);
			TCHAR str[255];
			lstrcpy(str, _tctime(&t));
			str[lstrlen(str) - 1] = '\0';
			SetWindowText(ptr->hTimeEdit, str);
			return TRUE;
		}

	}
	return 0;
}

//BOOL Timer::Cls_OnInit(HWND hwnd, HWND hwndFocus, LPARAM lp)
//{
//	SetTimer(hwnd, 0, 1000, 0);
//	hTimeEdit = GetDlgItem(hwnd, IDC_TimeEdit);
//	return TRUE;
//}
//
//void Timer::Cls_OnClose(HWND hwnd)
//{
//	KillTimer(hwnd, 0);
//	EndDialog(hwnd, 0);
//}

//BOOL Timer::Cls_OnTimer(/*HWND hwnd, int id, HWND hwndCtrl, UINT codeNotify*/)
//{
//	LONGLONG t = time(NULL);
//	TCHAR str[255];
//	lstrcpy(str, _tctime(&t));
//	str[lstrlen(str) - 1] = '\0';
//	SetWindowText(hTimeEdit, str);
//	return TRUE;
//}
