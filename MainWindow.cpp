#include "stdafx.h"
#include "MainWindow.h"
#include "STXGraphics.h"

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
	// Detect Graphics Type;
	//if (dynamic_cast<CSTXGdiPlusGraphics*>(pGraphics))
	//	_currentMode = _T("GDI+");
	//else if (dynamic_cast<CSTXD2DGraphics*>(pGraphics))
	//	_currentMode = _T("Direct2D");

	//SetWindowText(_currentMode);

	pGraphics->BeginDraw();

	// TODO: Paint here
	//Erase the background
	CSTXGraphicsBrush *pBrushBK = pGraphics->CreateSolidBrush(255, 255, 255, 255);
	pBrushBK->SetOpacity(255);
	pGraphics->FillRectangle(rcClient.left, rcClient.top, rcClient.right - rcClient.left, rcClient.bottom - rcClient.top, pBrushBK);
	pBrushBK->Release();

	//Draw a green rectangle
	CSTXGraphicsBrush *pBrush = pGraphics->CreateSolidBrush(0, 128, 0, 255);
	pGraphics->FillRectangle(100, 100, 100, 100, pBrush);
	pBrush->Release();

	pGraphics->EndDraw();
	delete pGraphics;
	EndPaint(&ps);

	return 0;
}
