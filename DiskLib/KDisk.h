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
	DWORD	dwPhyStartSectors;	//����ʼ����
	DWORD	dwBytesPerSector;   //ÿ�����ֽ���
	DWORD   dwTotalSectors;     //��������
	WORD	wDriverType;		//��������,Ŀǰ֧��FAT12/16/32,NTFS
	CString strTypeDesc;		//��������
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
	DWORD				m_dwBytesPerSector;  ///< ÿ�����ֽ���
	CString				m_strError;

	std::vector<StDriver>	m_vecDrivers;
};

