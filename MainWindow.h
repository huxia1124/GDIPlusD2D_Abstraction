#pragma once

#include <atlwin.h>

#include "STXAnchor.h"

class CMainWindow : public CWindowImpl<CMainWindow, CWindow, CFrameWinTraits>
{
public:
	CMainWindow();
	~CMainWindow();

protected:
	BEGIN_MSG_MAP(CMainWindow)
		MESSAGE_HANDLER(WM_CLOSE, OnClose)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		MESSAGE_HANDLER(WM_CREATE, OnCreate)
		MESSAGE_HANDLER(WM_PAINT, OnPaint)
	END_MSG_MAP()


protected:
	CSTXAnchor *_anchor;
	CString _currentMode;	//GDI+ or Direct2D

protected:
	LRESULT OnClose(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{

		DestroyWindow();
		return 0;
	}
	LRESULT OnDestroy(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		if (_anchor)
		{
			delete _anchor;
			_anchor = NULL;
		}

		PostQuitMessage(0);
		return 0;
	}
	LRESULT OnCreate(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
	{
		ModifyStyle(0, WS_CLIPCHILDREN);
		RECT rcWnd;
		GetClientRect(&rcWnd);

		_anchor = new CSTXAnchor(m_hWnd);

		//_anchor->AddItem(_dlg.m_hWnd, STXANCHOR_ALL);

		//_anchor->AddItem(IDC_STATIC_TIPS_TEXT, STXANCHOR_BOTTOM | STXANCHOR_LEFT);
		
		return 0;
	}
	LRESULT OnPaint(UINT msg, WPARAM wParam, LPARAM lParam, BOOL &bHandled);

private:

public:


};

