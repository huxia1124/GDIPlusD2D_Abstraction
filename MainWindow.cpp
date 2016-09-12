#include "stdafx.h"
#include "MainWindow.h"
#include "STXGraphics.h"

//#pragma comment(lib,"rpcrt4")
//#pragma comment(lib,"ole32")

CMainWindow::CMainWindow()
{
	_anchor = NULL;
}

CMainWindow::~CMainWindow()
{
}

LRESULT CMainWindow::OnPaint(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(&ps);
	RECT rcClient;
	GetClientRect(&rcClient);
	CSTXGraphics *pGraphics = CSTXGraphics::CreateAutoGraphics(m_hWnd, hdc, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top);

	// TODO: Paint here

	delete pGraphics;
	EndPaint(&ps);

	return 0;
}
