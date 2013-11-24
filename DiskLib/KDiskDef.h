#pragma once

#ifdef _WIN32
typedef char				kint8;
typedef unsigned char		kuint8;
typedef short				kint16;
typedef unsigned short		kuint16;
typedef long				kint32;
typedef unsigned long		kuint32;
typedef long long			kint64;
typedef unsigned long long	kuint64;
#endif


//��������,�ο�:http://en.wikipedia.org/wiki/Partition_type
enum eKPT
{
	EKPT_EMPTY			= 0x00,				//Empty partition entry
	EKPT_DOS20_FAT		= 0x01,				//FAT12 as primary partition in first physical 32 MB of disk or as logical drive anywhere on disk (else use 06h instead)
	EKPT_DOS30_FAT		= 0x04,				//FAT16 with less than 65536 sectors (32 MB). As primary partition it must reside in first physical 32 MB of disk, or as logical drive anywhere on disk (else use 06h instead)
	EKPT_EXTENDED		= 0x05,				//Extended partition with CHS addressing. It must reside in first physical 8 GB of disk, else use 0Fh instead (see 0Fh, 85h, C5h, D5h)
	EKPT_DOS33_FAT		= 0x06,				//FAT16B with 65536 or more sectors. It must reside in first physical 8 GB of disk, unless used for logical drives in an 0Fh extended partition (else use 0Eh instead). Also used for FAT12 and FAT16 volumes in primary partitions if they are not residing in first physical 32 MB of disk.[nb 1]
	EKPT_WINNT_NTFS		= 0x07,				//
	EKPT_DOS71_FAT		= 0x0B,				//FAT32 with CHS addressing
	EKPT_DOS71X_FAT		= 0x0C,				//FAT32X with LBA
	EKPT_DOS70_FAT		= 0x0E,				//FAT16X with LBA
	EKPT_EXTENDED_LBA	= 0x0F,				//Extended partition with LBA (see 05h and CFh)
	EKPT_LINUX_OLDSWAP  = 0x42,				//Old Linux swap (disk shared with DR DOS 6.0) (corresponds with 82h)
	EKPT_LINUX_SWAP		= 0x82,				//Linux swap space (corresponds with 42h)
	EKPT_LINUX_OLDNATIVE = 0x43,			//Old Linux native (disk shared with DR DOS 6.0) (corresponds with 83h)
	EKPT_LINUX_NATIVE	= 0x83				//Any native Linux file system (see 93h, corresponds with 43h)

};

#define KMBR_SIZE				512			//MBR��¼�ֽ���
#define KMBR_PARTITION_CONT		4			//ÿ��MBR��������

#pragma pack(push, curAlignment)
#pragma pack(1)

//Partition Table Entry 
struct StKPTE
{
	kuint8	kActive;					//80 = active partition,  00 = no-active partition | ����ָʾ��(Boot Indicator)   ָ���÷����Ƿ��ǻ������
	kuint8	kStartHead;					//��ʼ��ͷ(Starting Head)
	kuint8	kStartSector;				//��ʼ����(Starting Sector) ֻ����0~5λ���������λ(��6λ�͵�7λ)����ʼ�����ֶ���ʹ��
	kuint8  kStartCylinder;				//��ʼ����(Starting Cylinder)   ���˿�ʼ�����ֶε������λ�⣬��ʹ����1λ����ɸ�����ֵ����ʼ������һ��10λ�������ֵΪ1023
	kuint8	kType;						//ϵͳID(System ID) �����˷���������
	kuint8	kEndHead;					//������ͷ(Ending Head)
	kuint8	kEndSector;					//��������(Ending Sector)     ֻʹ����0~5λ�������λ(��6��7λ)�����������ֶ���ʹ��
	kuint8  kEndCylinder;				//��������(Ending Cylinder) ���˽��������ֶ�������λ�⣬��ʹ����1λ������ɸ�����ֵ������������һ��10λ���������ֵΪ1023
	kuint32 kRelativeSectors;			//���������(Relative Sectors) �Ӹô��̵Ŀ�ʼ���÷����Ŀ�ʼ��λ������������������
	kuint32 kTotalSectors;				//��������(Total Sectors) �÷����е���������
};

//Master Boot Record
struct StKMBR
{
	kuint8	kLoaderCode[440];			//Master bootstrap loader code
	kuint8  kDiskSign[6];				//Windows disk signature
	StKPTE  kPTE[KMBR_PARTITION_CONT];	//4����������
	kuint8  kSignature[2];				//Signature (55 AA)
};


//FAT DBR
struct StKFatDBR
{
	kuint8	kJMPBoot[3];				//��תָ��
	kuint8  kOEMName[8];				//���̱�־��os�汾��
	kuint16 kBytsPerSec;				//�����ֽ���
	kuint8	kSecPerClus;				//ÿ��������
	kuint16 kRsvdSecCnt;				//����������
	kuint8	kNumFATs;					//FAT��Ŀ
	kuint16 kRootEntCnt;				//��Ŀ¼����,ֻ��FAT12/FAT16ʹ�ô��ֶΡ���FAT32��������,���ֶα�������Ϊ0
	kuint16 kTotSec16;					//С������(ֻ��FAT12/FAT16ʹ�ô��ֶ�)��FAT32�������ԣ����ֶα�������Ϊ0
	kuint8	kMedia;						//ý��������( Media Descriptor)�ṩ�й�ý�屻ʹ�õ���Ϣ��ֵ0xF8��ʾӲ��,0xF0��ʾ���ܶȵ�3.5������.ý��������Ҫ����MS-DOS FAT16���̣���Windows 2000��δ��ʹ��
	kuint16 kFATSz16;					//ÿFAT������(Sectors Per FAT)ֻ��FAT12/FAT16��ʹ��,��FAT32��������
	kuint16 kSecPerTrk;					//ÿ��������
	kuint16 kNumHeads;					//��ͷ��
	kuint32 kHiddSec;					//����������(Hidden Sector)�÷�������������֮ǰ��������
	kuint32 kTotSec32;					//��������(Large Sector) ���ֶΰ���FAT32�������ܵ�������
	union 
	{
		struct StKFatSection     //FAT12/16����
		{
			kuint8  kDrvNum;            //������������( Physical Drive Number)��BIOS�������������йء���������������ʶΪ0x00������Ӳ�̱���ʶΪ0x80��������������������޹ء�
			kuint8  kReserved;			//����(Reserved) FAT16����һ�㽫���ֶε�ֵ����Ϊ0
			kuint8  kBootSig;			//��չ������ǩ(Extended Boot Signature) ���ֶα���Ҫ���ܱ�Windows 2000��ʶ���ֵ0x28��0x29
			kuint32 kVolId;             //�������(Volume Serial Number) �ڸ�ʽ������ʱ��������һ�������ţ������������ִ���
			kint8	kVolName[11];		//���
			kint8	kFileSysType[8];	//ϵͳID
		};

		struct StKFat32Section
		{
			kuint32 kFATSz32;             //ÿFAT������(Sectors Per FAT)(ֻ��FAT32ʹ��)�÷���ÿ��FAT��ռ��������
			kuint16 kExtFlags;            //��չ��־
			kuint16 kFSVer;               //�ļ�ϵͳ�汾(File ystem Version)ֻ��FAT32ʹ��,���ֽ�����Ҫ���޶��ţ������ֽ��Ǵ�Ҫ���޶���
			kuint32 kRootClus;            //��Ŀ¼�غ�(Root Cluster Number)(ֻ��FAT32ʹ��) ��Ŀ¼��һ�صĴغš����ֶε�ֵһ��Ϊ2�������������
			kuint16 kFSInfo;              //�ļ�ϵͳ��Ϣ������
			kuint16 kBkBootSec;           //������������
			kuint8	kReserved[12];		  //����
			kuint8  kDrvNum;			  //������������( Physical Drive Number)��BIOS�������������йء���������������ʶΪ0x00������Ӳ�̱���ʶΪ0x80��������������������޹ء�
			kuint8  kReserved1;			  //����(Reserved) FAT16����һ�㽫���ֶε�ֵ����Ϊ0
			kuint8  kBootSig;			  //��չ������ǩ(Extended Boot Signature) ���ֶα���Ҫ���ܱ�Windows 2000��ʶ���ֵ0x28��0x29
			kuint32 kVolId;               //�������(Volume Serial Number) �ڸ�ʽ������ʱ��������һ�������ţ������������ִ���
			kint8	kVolName[11];		  //���
			kint8	kFileSysType[8];	  //ϵͳID
		};
	};

	kuint8	kLoaderCode[310];			//bootstrap loader code
	kuint8  kSignature[2];				//Signature (55 AA)
};

#pragma pack(pop, curAlignment)



