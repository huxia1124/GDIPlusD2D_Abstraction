#include "stdafx.h"

#include "MainWindow.h"

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>
#include <atlplus.h>

[module(exe, name = "STXGraphicsDemoApplication")]
class CUniversalClientModule
{
public:
	int WinMain(int nCmdShow)
	{

		CMainWindow wnd;
		wnd.Create(NULL, NULL, _T("UniversalClient"));
		wnd.ShowWindow(SW_SHOW);

		RunMessageLoop();

		return 0;
	}
};
