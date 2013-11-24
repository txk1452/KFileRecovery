#pragma once

struct StDiskInfo
{
	int nDeviceNumber;				///< ���̱��
	BOOL bRemovableMedia;			///< �Ƿ�Ϊ���ƶ�����
	CString strVendorId;			///< ��Ʒ����
	CString strProductId;			///< ��ƷID
	CString strProductRevision;		///< �̼��汾��
	CString strSerialNumber;		///< ���к�
	CString strBusType;				///< ��������(SATA/USB��)
	DWORD	dwBytesPerSector;		///< ÿ�����ֽ���
	ULONGLONG ulDiskSize;			///< ���̴�С�ֽ���
};

class CKDiskEnum
{
protected:
	CKDiskEnum(void);

public:
	~CKDiskEnum(void);

public:
	static CKDiskEnum* Instance();

	int EnumDisk();

	const std::vector<StDiskInfo>& GetDiskInfoList() {return m_vecDiskInfo;}

	LPCTSTR GetError(){return m_strError;}

private:
	LPCTSTR GetBusType(int nBusType);

private:
	CString	m_strError;

	std::vector<StDiskInfo> m_vecDiskInfo;
};

