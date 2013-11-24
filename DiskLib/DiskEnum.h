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
};

class CDiskEnum
{
protected:
	CDiskEnum(void);

public:
	~CDiskEnum(void);

public:
	static CDiskEnum* Instance();

	int EnumDisk();



	LPCTSTR GetError(){return m_strError;}

private:
	LPCTSTR GetBusType(int nBusType);

private:
	CString	m_strError;

	std::vector<StDiskInfo> m_vecDiskInfo;
};

