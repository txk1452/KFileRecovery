#include "StdAfx.h"
#include "DiskEnum.h"
#include "PubFun.h"

#include <SetupAPI.h>

#pragma comment(lib, "Setupapi")

#define BUF_SIZE	1024


CDiskEnum::CDiskEnum(void)
{
}


CDiskEnum::~CDiskEnum(void)
{
}

CDiskEnum* CDiskEnum::Instance()
{
	static CDiskEnum oDiskEnum;

	return &oDiskEnum;
}

int CDiskEnum::EnumDisk()
{
	//get a handle to a device information set
	HDEVINFO hDevInfo = SetupDiGetClassDevs(
		&GUID_DEVINTERFACE_DISK,      // class GUID
		NULL,        // Enumerator
		NULL,        // hwndParent
		DIGCF_PRESENT | DIGCF_DEVICEINTERFACE    // present devices
		);

	if (INVALID_HANDLE_VALUE == hDevInfo)
	{
		m_strError.Format(_T("Failed to enum disk when call SetupDiGetClassDevs,%s"), CPubFun::GetLastErrorMessage());
		return 1;
	}

	int nRet = 0;
	int nIndex = 0;
	SP_DEVICE_INTERFACE_DATA sdifData;
	sdifData.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);
	PSP_DEVICE_INTERFACE_DETAIL_DATA detail = (PSP_DEVICE_INTERFACE_DETAIL_DATA)new BYTE[BUF_SIZE];
	PSTORAGE_DEVICE_DESCRIPTOR pDevDesc = (PSTORAGE_DEVICE_DESCRIPTOR)new BYTE[BUF_SIZE];
	while(SetupDiEnumDeviceInterfaces(hDevInfo, NULL, &GUID_DEVINTERFACE_DISK, nIndex, &sdifData))
	{
		::memset(detail, 0, BUF_SIZE);
		detail->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		if (!SetupDiGetDeviceInterfaceDetail(hDevInfo, &sdifData, detail, BUF_SIZE, NULL, NULL))
		{
			m_strError.Format(_T("Failed to enum disk when call SetupDiGetDeviceInterfaceDetail,%s"), CPubFun::GetLastErrorMessage());
			nRet = 1;
			break;
		}
		
		HANDLE hDevice = CreateFile(detail->DevicePath, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 
			NULL, OPEN_EXISTING, 0, NULL);
		if (INVALID_HANDLE_VALUE == hDevice)
		{
			m_strError.Format(_T("Failed to enum disk when call CreateFile,DevicePath:%s,%s"), detail->DevicePath, CPubFun::GetLastErrorMessage());
			nRet = 1;
			break;
		}

		//获取磁盘编号
		STORAGE_DEVICE_NUMBER sdNumber;
		DWORD dwRead;
		if (!DeviceIoControl(hDevice, IOCTL_STORAGE_GET_DEVICE_NUMBER, NULL, 0, &sdNumber, sizeof(STORAGE_DEVICE_NUMBER), &dwRead, NULL))
		{
			m_strError.Format(_T("Failed to enum disk when call DeviceIoControl,IoControlCode:IOCTL_STORAGE_GET_DEVICE_NUMBER,%s"), CPubFun::GetLastErrorMessage());
			nRet = 1;
			::CloseHandle(hDevice);
			break;
		}

		//查询磁盘信息
		STORAGE_PROPERTY_QUERY   stpQuery;  //查询输入参数 
		//指定查询方式  
		stpQuery.PropertyId   =   StorageDeviceProperty;  
		stpQuery.QueryType   =   PropertyStandardQuery; 

		::memset(pDevDesc, 0, BUF_SIZE);
		pDevDesc->Size = BUF_SIZE;
		if (!DeviceIoControl(hDevice, IOCTL_STORAGE_QUERY_PROPERTY, &stpQuery, sizeof(STORAGE_PROPERTY_QUERY), pDevDesc, pDevDesc->Size, &dwRead, NULL))
		{
			m_strError.Format(_T("Failed to enum disk when call DeviceIoControl,IoControlCode:IOCTL_STORAGE_QUERY_PROPERTY,%s"), CPubFun::GetLastErrorMessage());
			nRet = 1;
			::CloseHandle(hDevice);
			break;
		}

		::CloseHandle(hDevice);
		hDevice = INVALID_HANDLE_VALUE;

		m_vecDiskInfo.push_back(StDiskInfo());
		StDiskInfo& stDiskInfo = m_vecDiskInfo.back();
		stDiskInfo.nDeviceNumber = sdNumber.DeviceNumber;
		stDiskInfo.bRemovableMedia = pDevDesc->RemovableMedia;
		stDiskInfo.strBusType = GetBusType(pDevDesc->BusType);

		const char *p= (char*)pDevDesc;

		if (pDevDesc->VendorIdOffset)
		{
			stDiskInfo.strVendorId = CA2T(&p[pDevDesc->VendorIdOffset]);
		}

		if (pDevDesc->ProductIdOffset)
		{
			stDiskInfo.strProductId = CA2T(&p[pDevDesc->ProductIdOffset]);
		}

		if (pDevDesc->ProductRevisionOffset)
		{
			stDiskInfo.strProductRevision = CA2T(&p[pDevDesc->ProductRevisionOffset]);
		}

		if (pDevDesc->SerialNumberOffset)
		{
			stDiskInfo.strSerialNumber = CA2T(&p[pDevDesc->SerialNumberOffset]);
		}

		nIndex++;
	}

	delete [] detail;
	delete [] pDevDesc;

	SetupDiDestroyDeviceInfoList(hDevInfo);

	return nRet;
}


LPCTSTR CDiskEnum::GetBusType(int nBusType)
{
	struct _StBusTypeDesc
	{
		int nBusType;
		LPCTSTR szDesc;
	};
	_StBusTypeDesc stBusTypeDesc[] = {
		{BusTypeScsi, _T("SCSI")},
		{BusTypeAtapi, _T("ATAPI")},
		{BusTypeAta, _T("ATA")},
		{BusType1394, _T("1394")},
		{BusTypeSsa, _T("SSA")},
		{BusTypeFibre, _T("FIBRE")},
		{BusTypeUsb, _T("USB")},
		{BusTypeRAID, _T("RAID")},
		{BusTypeiScsi, _T("SCSI")},
		{BusTypeSas, _T("SAS")},
		{BusTypeSata, _T("SATA")},
		{BusTypeVirtual, _T("VIRTUAL")},
		{BusTypeFileBackedVirtual, _T("FILEBACKEDVIRTUAL")}
	};

	for(int i = 0; i < sizeof(stBusTypeDesc) / sizeof(stBusTypeDesc[0]); ++i)
	{
		if (stBusTypeDesc[i].nBusType == nBusType)
		{
			return stBusTypeDesc[i].szDesc;
		}
	}

	return _T("UNKNOWN");
}