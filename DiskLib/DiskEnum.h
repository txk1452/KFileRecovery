#pragma once

struct StDiskInfo
{
	int nDeviceNumber;				///< 磁盘编号
	BOOL bRemovableMedia;			///< 是否为可移动磁盘
	CString strVendorId;			///< 产品名称
	CString strProductId;			///< 产品ID
	CString strProductRevision;		///< 固件版本号
	CString strSerialNumber;		///< 序列号
	CString strBusType;				///< 磁盘类型(SATA/USB等)  
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

