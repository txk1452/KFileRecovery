// KFileRecoveryView.cpp : implementation of the CKFileRecoveryView class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "KFileRecoveryView.h"

BOOL CKFileRecoveryView::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}
