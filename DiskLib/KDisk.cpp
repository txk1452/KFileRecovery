#include "StdAfx.h"
#include "KDiskDef.h"
#include "KDisk.h"
#include "PubFun.h"


CKDisk::CKDisk(void)
{
	m_hDisk = INVALID_HANDLE_VALUE;
}


CKDisk::~CKDisk(void)
{
	CloseDisk();
}


int CKDisk::OpenDisk(int nDiskNum)
{
	CloseDisk();

	CString strPhysicalDrive;
	strPhysicalDrive.Format(_T("%s%d"), _T("\\\\.\\PhysicalDrive"), nDiskNum);

	m_hDisk = CreateFile(strPhysicalDrive, GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, 0 ,OPEN_EXISTING, 0, 0);
	if (INVALID_HANDLE_VALUE == m_hDisk)
	{
		m_strError.Format(_T("Failed to open disk(%s), %s"), strPhysicalDrive, CPubFun::GetLastErrorMessage());
		return 1;
	}

	//获取每扇区的字节数
	DISK_GEOMETRY dg;
	DWORD dwReturned;
	if (!DeviceIoControl(m_hDisk, IOCTL_DISK_GET_DRIVE_GEOMETRY, NULL, 0, &dg, 
		sizeof(DISK_GEOMETRY), &dwReturned, NULL))
	{
		m_strError.Format(_T("Failed to get driver geometry, %s"), CPubFun::GetLastErrorMessage());
		CloseDisk();
		return 1;
	}

	m_dwBytesPerSector = dg.BytesPerSector;
	
	//读取变分析MBR
	StKMBR	stKMbr;
	DWORD dwBytesRead;
	if (!ReadFile(m_hDisk, &stKMbr, KMBR_SIZE, &dwBytesRead, 0))
	{
		m_strError.Format(_T("Failed to read MBR record,%s"), CPubFun::GetLastErrorMessage());
		CloseDisk();
		return 1; 
	}

	int nRet = AnalyseDriver(&stKMbr, 0, 0);
	if (nRet != 0)
	{
		CloseDisk();
	}

	return nRet;
}

void CKDisk::CloseDisk()
{
	if(INVALID_HANDLE_VALUE != m_hDisk)
	{
		::CloseHandle(m_hDisk);
		m_hDisk = INVALID_HANDLE_VALUE;
	}

	m_vecDrivers.clear();
}

int CKDisk::AnalyseDriver(void* pMbr, DWORD dwMainPrevSectors, DWORD dwPrevSectors)
{
	StKMBR& oKMbr = *((StKMBR*)pMbr);
	for(int i = 0; i < KMBR_PARTITION_CONT; ++i)
	{
		StKPTE& stKPTE = oKMbr.kPTE[i];
		if((EKPT_EXTENDED == stKPTE.kType) || (EKPT_EXTENDED_LBA == stKPTE.kType))
		{
			//如果是扩展分区,递归查找

			LARGE_INTEGER n64Pos;
			n64Pos.QuadPart = ((LONGLONG) stKPTE.kRelativeSectors + dwMainPrevSectors) * m_dwBytesPerSector;
			DWORD dwRet = SetFilePointer(m_hDisk, n64Pos.LowPart,&n64Pos.HighPart, FILE_BEGIN);
			if (0xFFFFFFFF == dwRet)
			{
				m_strError.Format(_T("Failed to location EBR, %s"), CPubFun::GetLastErrorMessage());
				return 1;
			}

			DWORD dwBytesRead;
			StKMBR oKEbr;
			if (!ReadFile(m_hDisk, &oKEbr, KMBR_SIZE, &dwBytesRead, 0))
			{
				m_strError.Format(_T("Failed to read EBR record,%s"), CPubFun::GetLastErrorMessage());
				return 1; 
			}

			if (0 == dwMainPrevSectors)
			{
				//是主MBR
				dwMainPrevSectors = stKPTE.kRelativeSectors;
			}
			else
			{
				//是EBR(扩展MBR)
				dwPrevSectors = stKPTE.kRelativeSectors;
			}

			return AnalyseDriver(&oKEbr, dwMainPrevSectors, dwPrevSectors);
		}
		else if (EKPT_EMPTY != stKPTE.kType)
		{
			m_vecDrivers.push_back(StDriver());
			StDriver& stDriver = m_vecDrivers.back();
			stDriver.dwPhyStartSectors = dwMainPrevSectors + dwPrevSectors + stKPTE.kRelativeSectors;
			stDriver.dwTotalSectors = stKPTE.kTotalSectors;
			stDriver.dwBytesPerSector = m_dwBytesPerSector;

			switch(stKPTE.kType)
			{
			case EKPT_DOS20_FAT:
				{
					stDriver.wDriverType = eTypeFat12;
					stDriver.strTypeDesc = _T("FAT12");
				}
				break;

			case EKPT_DOS30_FAT:
			case EKPT_DOS33_FAT:
			case EKPT_DOS70_FAT:
				{
					stDriver.wDriverType = eTypeFat16;
					stDriver.strTypeDesc = _T("FAT16");
				}
				break;

			case EKPT_DOS71_FAT:
			case EKPT_DOS71X_FAT:
				{
					stDriver.wDriverType = eTypeFat32;
					stDriver.strTypeDesc = _T("FAT32");
				}
				break;

			case EKPT_WINNT_NTFS:
				{
					stDriver.wDriverType = eTypeNTFS;
					stDriver.strTypeDesc = _T("NTFS");
				}
				break;

			case EKPT_LINUX_OLDSWAP:
			case EKPT_LINUX_SWAP:
				{
					stDriver.wDriverType = eTypeLinuxSwap;
					stDriver.strTypeDesc = _T("Linux Swap");
				}
				break;

			case EKPT_LINUX_OLDNATIVE:
			case EKPT_LINUX_NATIVE:
				{
					stDriver.wDriverType = eTypeLinuxNative;
					stDriver.strTypeDesc = _T("Linux Native");
				}
				break;

			default:
				{
					stDriver.wDriverType = eTypeUnknown;
					stDriver.strTypeDesc = _T("Unknown");
				}
				break;
			}
			//stDriver.oKPTE = stKPTE; 
		}
	}

	return 0;
}