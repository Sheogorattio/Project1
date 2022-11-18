#include<windows.h>
#include<tchar.h>
#include"resource.h"

class Timer {
public:
	HWND hTimeEdit;
	static Timer* ptr;
	Timer();
	static BOOL CALLBACK DlProc(HWND hwnd, UINT message, WPARAM wp, LPARAM lp);
	BOOL Cls_OnInit(HWND hwnd, HWND hwndFocus, LPARAM lp);
	void Cls_OnClose(HWND hwnd);
	//BOOL Cls_OnTimer(/*HWND hwnd, int id, HWND hwndCtrl, UINT codeNotify*/);
};