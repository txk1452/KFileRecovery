#pragma once

enum EDriverType
{
	eTypeUnknown = -1,
	eTypeFat12,
	eTypeFat16,
	eTypeFat32,
	eTypeNTFS,
	eTypeLinuxSwap,
	eTypeLinuxNative
};

struct StDriver
{
	DWORD	dwPhyStartSectors;	//物理开始扇区
	DWORD	dwBytesPerSector;   //每扇区字节数
	DWORD   dwTotalSectors;     //总扇区数
	WORD	wDriverType;		//分区类型,目前支持FAT12/16/32,NTFS
	CString strTypeDesc;		//描述内容
};

class CKDisk
{
public:
	CKDisk(void);
	~CKDisk(void);

public:
	int OpenDisk(int nDiskNum);
	void CloseDisk();

	LPCTSTR GetError() const {return m_strError;}
	operator HANDLE() {return m_hDisk;}

private:
	int AnalyseDriver(void* pMbr, DWORD dwMainPrevSectors, DWORD dwPrevSectors);

private:
	HANDLE				m_hDisk;
	DWORD				m_dwBytesPerSector;  ///< 每扇区字节数
	CString				m_strError;

	std::vector<StDriver>	m_vecDrivers;
};

