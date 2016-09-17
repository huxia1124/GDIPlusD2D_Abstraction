#include "stdafx.h"

#include "MainWindow.h"

#include <atlbase.h>
#include <atlcom.h>
#include <atlwin.h>
#include <atltypes.h>
#include <atlctl.h>
#include <atlhost.h>
#include <atlplus.h>

#include <gdiplus.h>

[module(exe, name = "STXGraphicsDemoApplication")]
class CUniversalClientModule
{
public:
	int WinMain(int nCmdShow)
	{
		ULONG_PTR gdiplusToken = 0;
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

		CMainWindow wnd;
		wnd.Create(NULL, NULL, _T("STXGraphics Demo Application"));
		wnd.ShowWindow(SW_SHOW);

		RunMessageLoop();

		Gdiplus::GdiplusShutdown(gdiplusToken);

		return 0;
	}
};
