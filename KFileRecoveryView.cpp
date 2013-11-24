// KFileRecoveryView.cpp : implementation of the CKFileRecoveryView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "KFileRecoveryView.h"

BOOL CKFileRecoveryView::PreTranslateMessage(MSG* pMsg)
{
	pMsg;
	return FALSE;
}

LRESULT CKFileRecoveryView::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC dc(m_hWnd);

	//TODO: Add your drawing code here

	return 0;
}
