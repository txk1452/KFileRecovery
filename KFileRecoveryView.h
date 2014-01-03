// KFileRecoveryView.h : interface of the CKFileRecoveryView class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class CKFileRecoveryView : public CDialogImpl<CKFileRecoveryView>
{
public:
	enum { IDD = IDD_KFILERECOVERY_FORM };

	BOOL PreTranslateMessage(MSG* pMsg);

	BEGIN_MSG_MAP(CKFileRecoveryView)
	END_MSG_MAP()

// Handler prototypes (uncomment arguments if needed):
//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)
};
