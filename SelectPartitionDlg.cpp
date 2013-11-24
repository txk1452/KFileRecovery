#include "StdAfx.h"
#include "resource.h"

#include "SelectPartitionDlg.h"

#include "KDiskEnum.h"
#include "KDisk.h"

LRESULT CSelectPartitionDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());

	if (CKDiskEnum::Instance()->EnumDisk() != 0)
	{
		MessageBox(CKDiskEnum::Instance()->GetError(), _T("´íÎó"), MB_ICONEXCLAMATION);
		return TRUE;
	}

	// First DDX call, hooks up variables to controls.
	DoDataExchange(false);

	CImageList iml;
	iml.CreateFromImage ( IDB_BITMAP_DISK, 16, 1, CLR_NONE, IMAGE_BITMAP, LR_CREATEDIBSECTION );

	m_wndTree.SetImageList(iml);

	const std::vector<StDiskInfo>& vecDiskInfo = CKDiskEnum::Instance()->GetDiskInfoList();
	for(size_t sz = 0; sz != vecDiskInfo.size(); ++sz)
	{
		const StDiskInfo& stDiskInfo = vecDiskInfo[sz];

		CString strDiskName;
		CString strDiskSize;
		if (stDiskInfo.ulDiskSize > 1024 * 1024 * 1024)
		{
			strDiskSize.Format(_T("%I64d GB"), stDiskInfo.ulDiskSize / (1024 * 1024 * 1024));
		}
		else if (stDiskInfo.ulDiskSize > 1024 * 1024)
		{
			strDiskSize.Format(_T("%I64d MB"), stDiskInfo.ulDiskSize / (1024 * 1024));
		}
		else 
		{
			strDiskSize.Format(_T("%I64d KB"), stDiskInfo.ulDiskSize / 1024);
		}

		strDiskName.Format(_T("%s%d: %s%s (%s, %s)"), (stDiskInfo.bRemovableMedia ? _T("RM") : _T("HD")),
				stDiskInfo.nDeviceNumber, stDiskInfo.strVendorId, stDiskInfo.strProductId, strDiskSize, stDiskInfo.strBusType);
		int nImage = stDiskInfo.bRemovableMedia ? 1 : 0;
		CTreeItem tiRootItem = m_wndTree.InsertItem(strDiskName, nImage, nImage, TVI_ROOT, TVI_LAST);
		CKDisk oKDisk;
		if (oKDisk.OpenDisk(stDiskInfo.nDeviceNumber) != 0)
		{
			MessageBox(oKDisk.GetError(), _T("´íÎó"), MB_ICONEXCLAMATION);
			continue;
		}

		const std::vector<StDriver>& vecDrivers = oKDisk.GetDriversList();
		for(size_t i = 0; i < vecDrivers.size(); ++i)
		{
			const StDriver& stDriver = vecDrivers[i];
			int nImage = stDiskInfo.bRemovableMedia ? 2 : 3;
			CString strDriver;
			strDriver.Format(_T("·ÖÇø%d (%s)"), i, stDriver.strTypeDesc);
			tiRootItem.AddTail(strDriver, nImage);
		}

		tiRootItem.Expand();
	}
	//m_wndTree.InsertItem()

	return TRUE;
}

LRESULT CSelectPartitionDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CSelectPartitionDlg::OnOKCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}