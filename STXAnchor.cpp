//
//Copyright(c) 2016. Huan Xia
//
//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated 
//documentation files(the "Software"), to deal in the Software without restriction, including without limitation
//the rights to use, copy, modify, merge, publish, distribute, sublicense, and / or sell copies of the Software,
//and to permit persons to whom the Software is furnished to do so, subject to the following conditions :
//
//The above copyright notice and this permission notice shall be included in all copies or substantial portions
//of the Software.
//
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
//TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL
//THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
//CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//DEALINGS IN THE SOFTWARE.

#include "stdafx.h"
#include "STXAnchor.h"
#include "WinUser.h"
#include <atlstr.h>

//////////////////////////////////////////////////////////////////////
// CSTXAnchorItem

CSTXAnchorItem::CSTXAnchorItem(HWND hToolWnd,DWORD dwAnchor)
{
	m_pParentAnchor = NULL;
	
	m_AnchorItemInfo.iItemType = STXANCHOR_ITEM_WINDOW;

	m_AnchorItemInfo.dwAnchorTool = (DWORD_PTR)hToolWnd;

	m_AnchorItemInfo.dwAnchor = dwAnchor;
	
	GetWindowRect(hToolWnd, &m_AnchorItemInfo.rcItem);
	ScreenToClient(GetParent(hToolWnd), (LPPOINT)&m_AnchorItemInfo.rcItem);
	ScreenToClient(GetParent(hToolWnd), ((LPPOINT)&m_AnchorItemInfo.rcItem) + 1);

	m_AnchorItemInfo.bInitialized = TRUE;
}

CSTXAnchorItem::CSTXAnchorItem(UINT uDlgItemID,DWORD dwAnchor)
{
	m_pParentAnchor = NULL;

	m_AnchorItemInfo.iItemType = STXANCHOR_ITEM_DLGITEM;

	m_AnchorItemInfo.dwAnchorTool = (DWORD_PTR)uDlgItemID;

	m_AnchorItemInfo.dwAnchor = dwAnchor;

	m_AnchorItemInfo.bInitialized = FALSE;
}

CSTXAnchorItem::~CSTXAnchorItem()
{
}

void CSTXAnchorItem::Adjust(int cxDelta,int cyDelta)
{
	HWND hToolWnd = NULL;
	switch(m_AnchorItemInfo.iItemType)
	{
	case STXANCHOR_ITEM_WINDOW:
		hToolWnd = (HWND)m_AnchorItemInfo.dwAnchorTool;
		break;
	case STXANCHOR_ITEM_DLGITEM:
		hToolWnd = GetDlgItem(m_pParentAnchor->GetTraceWindow(), (int)m_AnchorItemInfo.dwAnchorTool);
		break;
	}

	if(hToolWnd == NULL)
		return;
	
	//Retrieve the rect of the Tool
	if(m_AnchorItemInfo.bInitialized == FALSE)
	{
		GetWindowRect(hToolWnd, &m_AnchorItemInfo.rcItem);
		ScreenToClient(GetParent(hToolWnd), (LPPOINT)&m_AnchorItemInfo.rcItem);
		ScreenToClient(GetParent(hToolWnd), ((LPPOINT)&m_AnchorItemInfo.rcItem) + 1);
		m_AnchorItemInfo.bInitialized = TRUE;
	}

	RECT rcOld = m_AnchorItemInfo.rcItem;	//save old invalid rect for later use

	if((m_AnchorItemInfo.dwAnchor & STXANCHOR_RIGHT) == STXANCHOR_RIGHT)	//Right-anchor
		m_AnchorItemInfo.rcItem.right += cxDelta;
	if((m_AnchorItemInfo.dwAnchor & STXANCHOR_BOTTOM) == STXANCHOR_BOTTOM)	//Bottom-anchor
		m_AnchorItemInfo.rcItem.bottom += cyDelta;

	if((m_AnchorItemInfo.dwAnchor & STXANCHOR_LEFT) != STXANCHOR_LEFT
		&& (m_AnchorItemInfo.dwAnchor & STXANCHOR_RIGHT) == STXANCHOR_RIGHT)	//NO Left-anchor, move!
		m_AnchorItemInfo.rcItem.left += cxDelta;
	if((m_AnchorItemInfo.dwAnchor & STXANCHOR_TOP) != STXANCHOR_TOP
		&& (m_AnchorItemInfo.dwAnchor & STXANCHOR_BOTTOM) == STXANCHOR_BOTTOM)	//NO Top-anchor, move!
		m_AnchorItemInfo.rcItem.top += cyDelta;

	MoveWindow(hToolWnd, m_AnchorItemInfo.rcItem.left, m_AnchorItemInfo.rcItem.top, m_AnchorItemInfo.rcItem.right - m_AnchorItemInfo.rcItem.left, m_AnchorItemInfo.rcItem.bottom - m_AnchorItemInfo.rcItem.top, TRUE);	//FALSE for the 2nd parameter here means to relay on the following code

	//The following code is to avoid flash
	InvalidateRect(GetParent(hToolWnd) ,&rcOld, TRUE);
	ValidateRect(GetParent(hToolWnd), &m_AnchorItemInfo.rcItem);
	RedrawWindow(hToolWnd, NULL, NULL, 0);
	UpdateWindow(GetParent(hToolWnd));

}

void CSTXAnchorItem::SetParentAnchor(CSTXAnchor *pParentAnchor)
{
	m_pParentAnchor = pParentAnchor;
}


//////////////////////////////////////////////////////////////////////
// CSTXAnchor

CSTXAnchor::CSTXAnchor()
{
	s_AnchorBaseID = 0;
	m_hSizeTraceWnd = NULL;
	m_pfnOldProc = NULL;
	m_iAnchorID = s_AnchorBaseID++;
}

CSTXAnchor::CSTXAnchor(HWND hSizeTraceWnd)
{
	s_AnchorBaseID = 0;

	m_iAnchorID = s_AnchorBaseID++;
	m_hSizeTraceWnd = hSizeTraceWnd;

	//Redirect WndProc , save original one first
	m_pfnOldProc = (WNDPROC)::GetWindowLongPtr(hSizeTraceWnd,GWLP_WNDPROC);
	::SetWindowLongPtr(hSizeTraceWnd,GWLP_WNDPROC,(LONG_PTR)&CSTXAnchor::NewWndProc);

	//Set a property for used in New WndProc
	CString strAnchorDataName;
	strAnchorDataName.Format(_T("STX_ANCHOR_DATA_%d"),m_iAnchorID);
	::SetProp(hSizeTraceWnd,strAnchorDataName,(HANDLE)this);

	//Get the client size of the window
	RECT rcClient;
	GetClientRect(hSizeTraceWnd, &rcClient);
	m_sizeTrace.cx = rcClient.right - rcClient.left;
	m_sizeTrace.cy = rcClient.bottom - rcClient.top;

	//Save an original WndProc for multi Tricks
	HANDLE hOriginWndProc = ::GetProp(hSizeTraceWnd,_T("STX_ORIGIN_WNDPROC"));
	if(hOriginWndProc == NULL)
	{
		::SetProp(hSizeTraceWnd,_T("STX_ORIGIN_WNDPROC"),(HANDLE)m_pfnOldProc);
		::SetProp(hSizeTraceWnd,_T("STX_WNDPROC_REFERENCE_COUNT"),(HANDLE)1);
	}
	else
	{
		DWORD dwReferenceCount = (DWORD)::GetProp(hSizeTraceWnd,_T("STX_WNDPROC_REFERENCE_COUNT"));
		dwReferenceCount++;
		::SetProp(hSizeTraceWnd,_T("STX_WNDPROC_REFERENCE_COUNT"),(HANDLE)dwReferenceCount);
	}
}

CSTXAnchor::~CSTXAnchor()
{
	DeleteAllItems();

	if(m_hSizeTraceWnd)
	{
		//Remove the property
		CString strAnchorDataName;
		strAnchorDataName.Format(_T("STX_ANCHOR_DATA_%d"),m_iAnchorID);
		::RemoveProp(m_hSizeTraceWnd,strAnchorDataName);

		DWORD dwReferenceCount = (DWORD)::GetProp(m_hSizeTraceWnd,_T("STX_WNDPROC_REFERENCE_COUNT"));
		dwReferenceCount--;
		if(dwReferenceCount > 0)
		{
			::SetProp(m_hSizeTraceWnd,_T("STX_WNDPROC_REFERENCE_COUNT"),(HANDLE)dwReferenceCount);
		}
		else
		{
			::RemoveProp(m_hSizeTraceWnd,_T("STX_WNDPROC_REFERENCE_COUNT"));
			WNDPROC pfnOriginWndProc = (WNDPROC)::GetProp(m_hSizeTraceWnd,_T("STX_ORIGIN_WNDPROC"));

			//Restore WndProc
			::SetWindowLongPtr(m_hSizeTraceWnd,GWLP_WNDPROC,(LONG_PTR)pfnOriginWndProc);

			::RemoveProp(m_hSizeTraceWnd,_T("STX_ORIGIN_WNDPROC"));
		}
	}
}

void CSTXAnchor::DeleteAllItems()
{
	for(size_t i=0;i<m_arrpItems.size();i++)
		if(m_arrpItems[i])
			delete m_arrpItems[i];
	m_arrpItems.clear();
}

void CSTXAnchor::InsertItem(int iIndex,HWND hToolWnd,DWORD dwAnchor)
{
	CSTXAnchorItem *pItem = new CSTXAnchorItem(hToolWnd,dwAnchor);

	pItem->SetParentAnchor(this);
	m_arrpItems.insert(m_arrpItems.begin() + iIndex,pItem);
}

void CSTXAnchor::InsertItem(int iIndex,UINT uDlgItemID,DWORD dwAnchor)
{
	CSTXAnchorItem *pItem = new CSTXAnchorItem(uDlgItemID,dwAnchor);

	pItem->SetParentAnchor(this);
	m_arrpItems.insert(m_arrpItems.begin() + iIndex,pItem);
}

void CSTXAnchor::AddItem(HWND hToolWnd,DWORD dwAnchor)
{
	InsertItem(m_arrpItems.size(), hToolWnd,dwAnchor);
}

void CSTXAnchor::AddItem(UINT uDlgItemID,DWORD dwAnchor)
{
	InsertItem(m_arrpItems.size(),uDlgItemID,dwAnchor);
}

void CSTXAnchor::Adjust(int cxOld,int cyOld,int cxNew,int cyNew)
{
	Adjust(cxNew - cxOld , cyNew - cyOld);
}

void CSTXAnchor::Adjust(int cxDelta,int cyDelta)
{
	for(size_t i=0;i<m_arrpItems.size();i++)
		m_arrpItems[i]->Adjust(cxDelta,cyDelta);
}

int CSTXAnchor::GetTraceWindowWidth() const
{
	return m_sizeTrace.cx;
}

int CSTXAnchor::GetTraceWindowHeight() const
{
	return m_sizeTrace.cy;
}

void CSTXAnchor::SetTraceSize(int cx,int cy)
{
	m_sizeTrace.cx = cx;
	m_sizeTrace.cy = cy;
}

HWND CSTXAnchor::GetTraceWindow()
{
	return m_hSizeTraceWnd;
}

void CSTXAnchor::AddAllChildren(DWORD dwAnchor)
{
	DWORD_PTR *pData = new DWORD_PTR[2];
	pData[0] = (DWORD_PTR)this;
	pData[1] = dwAnchor;
	EnumChildWindows(m_hSizeTraceWnd,&CSTXAnchor::EnumChildProc,(LPARAM)pData);
	delete []pData;
}


//static
LRESULT CALLBACK CSTXAnchor::NewWndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == WM_SIZE)
	{
		CSTXAnchor *pAnchor = NULL;
		int i = 0;
		CString strAnchorDataName;
		strAnchorDataName.Format(_T("STX_ANCHOR_DATA_%d"),i);
		while(pAnchor = (CSTXAnchor*)::GetProp(hwnd,strAnchorDataName))
		{
			if(pAnchor)
			{
				int cx = LOWORD(lParam);
				int cy = HIWORD(lParam);
				
				pAnchor->Adjust(cx - pAnchor->GetTraceWindowWidth(), cy - pAnchor->GetTraceWindowHeight());
				pAnchor->SetTraceSize(cx,cy);
			}
			strAnchorDataName.Format(_T("STX_ANCHOR_DATA_%d"),++i);
		}
	}
	
	WNDPROC pfnOldWndProc = (WNDPROC)::GetProp(hwnd,_T("STX_ORIGIN_WNDPROC"));
	return CallWindowProc(pfnOldWndProc,hwnd,uMsg,wParam,lParam);
}

BOOL CALLBACK CSTXAnchor::EnumChildProc(HWND hwnd, LPARAM lParam)
{
	DWORD_PTR *pData = (DWORD_PTR*)lParam;
	CSTXAnchor* pAnchor = (CSTXAnchor*)pData[0];
	pAnchor->AddItem((UINT)GetDlgCtrlID(hwnd),pData[1]);
	return TRUE;
}
