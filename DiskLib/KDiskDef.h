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


//分区类型,参考:http://en.wikipedia.org/wiki/Partition_type
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

#define KMBR_SIZE				512			//MBR记录字节数
#define KMBR_PARTITION_CONT		4			//每个MBR分区个数

#pragma pack(push, curAlignment)
#pragma pack(1)

//Partition Table Entry 
struct StKPTE
{
	kuint8	kActive;					//80 = active partition,  00 = no-active partition | 引导指示符(Boot Indicator)   指明该分区是否是活动分区。
	kuint8	kStartHead;					//开始磁头(Starting Head)
	kuint8	kStartSector;				//开始扇区(Starting Sector) 只用了0~5位。后面的两位(第6位和第7位)被开始柱面字段所使用
	kuint8  kStartCylinder;				//开始柱面(Starting Cylinder)   除了开始扇区字段的最后两位外，还使用了1位来组成该柱面值。开始柱面是一个10位数，最大值为1023
	kuint8	kType;						//系统ID(System ID) 定义了分区的类型
	kuint8	kEndHead;					//结束磁头(Ending Head)
	kuint8	kEndSector;					//结束扇区(Ending Sector)     只使用了0~5位。最后两位(第6、7位)被结束柱面字段所使用
	kuint8  kEndCylinder;				//结束柱面(Ending Cylinder) 除了结束扇区字段最后的两位外，还使用了1位，以组成该柱面值。结束柱面是一个10位的数，最大值为1023
	kuint32 kRelativeSectors;			//相对扇区数(Relative Sectors) 从该磁盘的开始到该分区的开始的位移量，以扇区来计算
	kuint32 kTotalSectors;				//总扇区数(Total Sectors) 该分区中的扇区总数
};

//Master Boot Record
struct StKMBR
{
	kuint8	kLoaderCode[440];			//Master bootstrap loader code
	kuint8  kDiskSign[6];				//Windows disk signature
	StKPTE  kPTE[KMBR_PARTITION_CONT];	//4个分区表项
	kuint8  kSignature[2];				//Signature (55 AA)
};


//FAT DBR
struct StKFatDBR
{
	kuint8	kJMPBoot[3];				//跳转指令
	kuint8  kOEMName[8];				//厂商标志和os版本号
	kuint16 kBytsPerSec;				//扇区字节数
	kuint8	kSecPerClus;				//每簇扇区数
	kuint16 kRsvdSecCnt;				//保留扇区数
	kuint8	kNumFATs;					//FAT数目
	kuint16 kRootEntCnt;				//根目录项数,只有FAT12/FAT16使用此字段。对FAT32分区而言,本字段必须设置为0
	kuint16 kTotSec16;					//小扇区数(只有FAT12/FAT16使用此字段)对FAT32分区而言，本字段必须设置为0
	kuint8	kMedia;						//媒体描述符( Media Descriptor)提供有关媒体被使用的信息。值0xF8表示硬盘,0xF0表示高密度的3.5寸软盘.媒体描述符要用于MS-DOS FAT16磁盘，在Windows 2000中未被使用
	kuint16 kFATSz16;					//每FAT扇区数(Sectors Per FAT)只被FAT12/FAT16所使用,对FAT32分区而言
	kuint16 kSecPerTrk;					//每道扇区数
	kuint16 kNumHeads;					//磁头数
	kuint32 kHiddSec;					//隐藏扇区数(Hidden Sector)该分区上引导扇区之前的扇区数
	kuint32 kTotSec32;					//总扇区数(Large Sector) 本字段包含FAT32分区中总的扇区数
	union 
	{
		struct StKFatSection     //FAT12/16区块
		{
			kuint8  kDrvNum;            //物理驱动器号( Physical Drive Number)与BIOS物理驱动器号有关。软盘驱动器被标识为0x00，物理硬盘被标识为0x80，而与物理磁盘驱动器无关。
			kuint8  kReserved;			//保留(Reserved) FAT16分区一般将本字段的值设置为0
			kuint8  kBootSig;			//扩展引导标签(Extended Boot Signature) 本字段必须要有能被Windows 2000所识别的值0x28或0x29
			kuint32 kVolId;             //分区序号(Volume Serial Number) 在格式化磁盘时所产生的一个随机序号，它有助于区分磁盘
			kint8	kVolName[11];		//卷标
			kint8	kFileSysType[8];	//系统ID
		};

		struct StKFat32Section
		{
			kuint32 kFATSz32;             //每FAT扇区数(Sectors Per FAT)(只被FAT32使用)该分区每个FAT所占的扇区数
			kuint16 kExtFlags;            //扩展标志
			kuint16 kFSVer;               //文件系统版本(File ystem Version)只供FAT32使用,高字节是主要的修订号，而低字节是次要的修订号
			kuint32 kRootClus;            //根目录簇号(Root Cluster Number)(只供FAT32使用) 根目录第一簇的簇号。本字段的值一般为2，但不总是如此
			kuint16 kFSInfo;              //文件系统信息扇区号
			kuint16 kBkBootSec;           //备份引导扇区
			kuint8	kReserved[12];		  //保留
			kuint8  kDrvNum;			  //物理驱动器号( Physical Drive Number)与BIOS物理驱动器号有关。软盘驱动器被标识为0x00，物理硬盘被标识为0x80，而与物理磁盘驱动器无关。
			kuint8  kReserved1;			  //保留(Reserved) FAT16分区一般将本字段的值设置为0
			kuint8  kBootSig;			  //扩展引导标签(Extended Boot Signature) 本字段必须要有能被Windows 2000所识别的值0x28或0x29
			kuint32 kVolId;               //分区序号(Volume Serial Number) 在格式化磁盘时所产生的一个随机序号，它有助于区分磁盘
			kint8	kVolName[11];		  //卷标
			kint8	kFileSysType[8];	  //系统ID
		};
	};

	kuint8	kLoaderCode[310];			//bootstrap loader code
	kuint8  kSignature[2];				//Signature (55 AA)
};

#pragma pack(pop, curAlignment)



