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
	DWORD	dwBytesPerSector;		///< 每扇区字节数
	ULONGLONG ulDiskSize;			///< 磁盘大小字节数
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

