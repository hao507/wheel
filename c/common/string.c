<<<<<<< HEAD
﻿#ifndef __SM_BIOS_H__
#define __SM_BIOS_H__
=======
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include <Windows.h>
#include <WinSock2.h>

void test()
{
	printf("exit ....\n");
}
>>>>>>> a7f1d1762da8bab763177fc9fc8d3f66dffe648f

#define TRUE 1
#define FALSE 0
#define ERROR -1
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned long DWORD;
//typedef unsigned double QWORD;
//typedef unsigned double QWORD;


typedef struct
{
	BYTE Type;
	BYTE Length;
	WORD Handle;
}SMBStructsHeader;

typedef struct
{
	BYTE		AnchorString[4];
	BYTE		Checksum;
	BYTE		EntryPointLength;
	BYTE		SMBMajorVer;
	BYTE		SMBMinorVer;
	WORD		MaxStructSize;
	BYTE		EntryPointRev;
	BYTE		FormattedArea[5];
	BYTE		InterAnchorStr[5];
	BYTE		InterChecksum;
	WORD		StructTableLength;
	DWORD		StructTableAddr;
	WORD		NumOfSMBStructs;
	BYTE		SMBBCDRev;
}SMBStructTableEntryPoint;

typedef struct
{
	SMBStructsHeader Header;
	BYTE		Vendor;
	BYTE		Version;
	WORD		StartingAddrSeg;
	BYTE		ReleaseDate;
	BYTE		ROMSize;
	BYTE		Characteristics[8];
	WORD		Reserved;
	BYTE		MajorRelease;
	BYTE		MinorRelease;
	BYTE		FirmwareMajorRelease;
	BYTE		FirmwareMinorRelease;
}SMBBIOSInfo;

typedef struct
{
	DWORD time_low;
	WORD		time_mid;
	WORD		time_hi_and_ver;
	BYTE		clock_seq_hi_and_res;
	BYTE		clock_seq_low;
	BYTE		Node[6];
}UUIDStruct;

typedef struct
{
	SMBStructsHeader	Header;
	BYTE		Manufacturer;
	BYTE		ProductName;
	BYTE		Version;
	BYTE		SerialNumber;
	UUIDStruct	UUID;
	BYTE		WakeupType;
	/* 2.4+ */
	BYTE		SKUNumber;
	BYTE		Family;
}SMBSysInfo;

typedef struct
{
	SMBStructsHeader Header;
	BYTE		Manufacturer;
	BYTE		Product;
	BYTE		Version;
	BYTE		SerialNumber;
	BYTE		AssetTag;
	BYTE		FeatureFlags;
	BYTE		LocationInChassis;
	WORD		ChassisHandle;
	BYTE		BoardType;
	BYTE		Resvd[2];
}SMBBaseBoardInfo;

typedef struct
{
	SMBStructsHeader Header;
	BYTE		InterRefDesignator;
	BYTE		InterConType;
	BYTE		ExterRefDesignator;
	BYTE		ExterConType;
	BYTE		PortType;
}SMBPortInfo;

typedef struct
{
	SMBStructsHeader Header;
	BYTE		Manufacturer;
	BYTE		Type;
	BYTE		Version;
	BYTE		SerialNumber;
	BYTE		AssetTagNum;
	BYTE		BootupState;
	BYTE		PowerSupplyState;
	BYTE		ThermalState;
	BYTE		SecurityStatus;
	DWORD		OEMRes;
	/* 2.3+ */
	BYTE		Height;
	BYTE		NumOfPowerCords;
	BYTE		Resved[3]; /* Contained Elements */
}SMBSysEnclosure;

typedef struct
{
	SMBStructsHeader Header;
	BYTE		SocketDesignation;
	BYTE		Type;
	BYTE		Family;
	BYTE		Manufacturer;
	BYTE		ID[8];
	BYTE		Version;
	BYTE		Voltage;
	WORD		ExternalClock;
	WORD		MaxSpeed;
	WORD		CurrentSpeed;
	BYTE		Status;
	BYTE		Upgrade;
	WORD		L1CacheHandle;
	WORD		L2CacheHandle;
	WORD		L3CacheHandle;
	/* 2.3+ */
	BYTE		SerialNumber;
	BYTE		AssetTag;
	BYTE		PartNumber;
	/* 2.5+ */
	BYTE		CoreCount;
	BYTE		CoreEnabled;
	BYTE		ThreadCount;
	WORD		Characterisitics;
	/* 2.6+ */
	WORD		ProcessorFamily2;
}SMBProcessorInfo;

typedef struct
{
	SMBStructsHeader Header;
	BYTE		SocketDesignation;
	WORD		Configuration;
	WORD		MaxCacheSize;
	WORD		InstalledSize;
	WORD		SupportedSRAMType;
	WORD		CurrentSRAMType;
	BYTE		Speed;
	BYTE		ErrCorrectionType;
	BYTE		SysCacheType;
	BYTE		Associativity;
}SMBCacheInfo;

typedef struct
{
	SMBStructsHeader Header;
	BYTE		Designation;
	BYTE		Type;
	BYTE		DataBusWidth;
	BYTE		CurrentUsage;
	BYTE		Length;
	WORD		ID;
	BYTE		Characteristics1;
	BYTE		Characteristics2;
	/* 2.6+ */
	WORD		SegGroupNum;
	BYTE		BusNumber;
	BYTE		Dev_FuncNum;
}SMBSystemSlots;

typedef struct
{
	SMBStructsHeader Header;
	BYTE		Location;
	BYTE		Use;
	BYTE		MemErrCorrection;
	DWORD		MaxCapacity;
	WORD		MemErrInfoHandle;
	WORD		NumOfMemDevices;
}SMBPhyMemArray;

typedef struct
{
	SMBStructsHeader Header;
	WORD		PhyMemArrayHandle;
	WORD		MemErrInfoHandle;
	WORD		TotalWidth;
	WORD		DataWidth;
	WORD		Size;
	BYTE		FormFactor;
	BYTE		DeviceSet;
	BYTE		DeviceLocator;
	BYTE		BankLocator;
	BYTE		MemoryType;
	WORD		TypeDetail;
	/* 2.3+ */
	WORD		Speed;
	BYTE		Manufacturer;
	BYTE		SerialNumber;
	BYTE		AssetTag;
	BYTE		PartNumber;
	/* 2.6+ */
	BYTE		Attributes;
}SMBMemDevice;

typedef struct
{
	SMBStructsHeader Header;
	DWORD		StartingAddr;
	DWORD		EndingAddr;
	WORD		MemArrayHandle;
	BYTE		PartitionWidth;
}MemArrayMappedAddr;

typedef struct
{
<<<<<<< HEAD
	SMBStructsHeader Header;
	BYTE		DeviceType;
	BYTE		DescriptionStr;
}OnBoardDeviceInfo;

DWORD GetFlagPos(BYTE *flag, DWORD start, DWORD end);
DWORD GetStruct(DWORD TableAddress, WORD StructCount, BYTE Type, BYTE n);
BYTE *ReadStr(DWORD handle, BYTE n);
BYTE CheckBit(WORD value, BYTE bit);

/*BYTE *GenUUID(UUIDStruct uuid);*/
/*BYTE *GenWakeupType(BYTE value);*/
/*BYTE *GenEnclosureType(BYTE value);*/
/*BYTE *GenEnclosureStates(BYTE value);*/
BYTE *GenSecurityStatus(BYTE value);
BYTE *GenProcessorType(BYTE value);
BYTE *GenSRAMType(WORD value);
/*
BYTE *GenProcessorUpgrade(BYTE value);
BYTE *GenErrCorrectionType(BYTE value);
BYTE *GenSysCacheType(BYTE value);
BYTE *GenAssociativity(BYTE value);
*/
BYTE *GenSlotType(BYTE value);
BYTE *GenSlotDataBusWidth(BYTE value);
BYTE *GenCurrentUsage(BYTE value);
/*BYTE *GenSlotLength(BYTE value);*/
/*BYTE *GenFormFactor(BYTE value);*/
BYTE *GenMemoryType(BYTE value);
/*BYTE *GenMemTypeDetail(WORD value);*/
BYTE *GenPortConType(BYTE value);
BYTE *GenPortType(BYTE value);
BYTE *GenDeviceType(BYTE value);

WORD CalCacheSize(WORD value);
=======
	atexit(test);
>>>>>>> a7f1d1762da8bab763177fc9fc8d3f66dffe648f

DWORD *GetSupportedTypeHandles(DWORD TableAddress, WORD StructCount);
void ShowInfo(DWORD *types, WORD StructCount);
void BIOSInfo(DWORD handle);
void SystemInfo(DWORD handle);
void EnclosureInfo(DWORD handle);
void ProcessorInfo(DWORD handle);
void CacheInfo(DWORD handle);
void SystemSlots(DWORD handle);
void MemoryDevice(DWORD handle);
void BaseBoardInfo(DWORD handle);
void PortConInfo(DWORD handle);
void OnBoardDevInfo(DWORD handle);
#endif

/*
 sbios.c
  Compile command: tcc -mh sbios.c
   by Blue.J 2008.10.25
   */
#include <stdio.h>
#include <dos.h>
#include <stdlib.h>
//#include "smbios.h"


void main()
{
	DWORD addr, data, handle, pos = GetFlagPos("_SM_", 0xe0000000, 0xf0000000);
	BYTE max;
	SMBStructTableEntryPoint *entry;

	entry = (SMBStructTableEntryPoint*)pos;
	addr = (DWORD)((entry->StructTableAddr & 0xf0000) * 4096 + (entry->StructTableAddr & 0x0ffff));
	max = entry->NumOfSMBStructs;
	printf("System Management BIOS(SMBIOS) Version:%d.%d/n", entry->SMBMajorVer, entry->SMBMinorVer);
	ShowInfo(GetSupportedTypeHandles(addr, max), max);
}

DWORD GetFlagPos(BYTE *flag, DWORD start, DWORD end)
{
	DWORD addr;
	WORD i, j, pos = 0xfffe;
	WORD len = strlen(flag);
	for (addr = start;; addr += 0x10000000)
	{
		for (i = 0; i<0xffff; i++)
		{
			if (*((BYTE *)(addr + i)) == flag[0])
				if (!strncmp((BYTE *)(addr + i), flag, len))
				{
					pos = i;
					break;
				}
		}
		if (addr == end)break;
	}
	return (DWORD)(pos + addr);
}

DWORD GetStruct(DWORD TableAddress, WORD StructCount, BYTE Type, BYTE n)
{
	WORD i;
	DWORD addr;
	BYTE lasttype, no = 1;

	i = 0;
	addr = 0xFFFFFFFF;
	while (i < StructCount && addr == 0xFFFFFFFF)
	{
		i++;
		lasttype = ((SMBStructsHeader *)TableAddress)->Type;
		if (lasttype == Type)
			if (no == n)
				addr = (DWORD)TableAddress;
			else no++;
		else
		{
			TableAddress += ((SMBStructsHeader *)TableAddress)->Length;
			while (*((WORD *)TableAddress) != 0)
				TableAddress++;
			TableAddress += 2;
		}
	}
	return addr;
}

BYTE *ReadStr(DWORD handle, BYTE n)
{
	WORD i;
	BYTE no = 1;
	while (no != n)
	{
		while (*((BYTE*)handle) != 0)handle++;
		handle++;
		no++;
	}
	return (BYTE*)handle;
}

DWORD *GetSupportedTypeHandles(DWORD TableAddress, WORD StructCount)
{
	WORD i = 0, n = 1;
	DWORD *handles = (DWORD *)malloc(sizeof(DWORD));
	while (i < StructCount)
	{
		i++;
		handles = (DWORD *)realloc(handles, sizeof(DWORD)*(++n));
		handles[n - 2] = TableAddress;
		TableAddress += ((SMBStructsHeader *)TableAddress)->Length;
		while (*((WORD *)TableAddress) != 0)
			TableAddress++;
		TableAddress += 2;
	}
	return handles;
}

void ShowInfo(DWORD *types, WORD StructCount)
{
	WORD i;
	BYTE type;
	DWORD handle;
	for (i = 0; i<StructCount; i++)
	{
		handle = types[i];
		type = ((SMBStructsHeader *)handle)->Type;
		switch (type)
		{
		case 0:BIOSInfo(handle); break;
		case 1:SystemInfo(handle); break;
		case 2:BaseBoardInfo(handle); break;
		case 4:ProcessorInfo(handle); break;
		case 7:CacheInfo(handle); break;
		case 8:PortConInfo(handle); break;
		case 9:SystemSlots(handle); break;
		case 10:OnBoardDevInfo(handle); break;
		case 17:MemoryDevice(handle); break;
		default:break;
		}
	}
}

BYTE CheckBit(WORD value, BYTE bit)
{
	return (((value&(0x1 << bit)) == (0x1 << bit)) ? TRUE : FALSE);
}

BYTE *GenProcessorType(BYTE value)
{
	switch (value)
	{
	case 0x01:return "Other";
	case 0x02:return "Unknown";
	case 0x03:return "Central Processor";
	case 0x04:return "Math Processor";
	case 0x05:return "DSP Processor";
	case 0x06:return "Video Processor";
	default:return "(none)";
	}
}

BYTE *GenSRAMType(WORD value)
{
	BYTE i;
	for (i = 0; i <= 6; i++)
	{
		if (CheckBit(value, i))
		{
			switch (i)
			{
			case 0:return "Other";
			case 1:return "Unknown";
			case 2:return "Non-Burst";
			case 3:return "Burst";
			case 4:return "Pipeline Burst";
			case 5:return "Synchronous";
			case 6:return "Asynchronous";
			default:return "(none)";
			}
		}
	}
}

BYTE *GenSlotType(BYTE value)
{
	switch (value)
	{
	case 0x01:return "Other";
	case 0x02:return "Unknown";
	case 0x03:return "ISA";
	case 0x04:return "MCA";
	case 0x05:return "EISA";
	case 0x06:return "PCI";
	case 0x07:return "PC Card (PCMCIA)";
	case 0x08:return "VL-VESA";
	case 0x09:return "Proprietary";
	case 0x0A:return "Processor Card Slot";
	case 0x0B:return "Proprietary Memory Card Slot";
	case 0x0C:return "I/O Riser Card Slot";
	case 0x0D:return "NuBus";
	case 0x0E:return "PCI-66MHz Capable";
	case 0x0F:return "AGP";
	case 0x10:return "AGP 2X";
	case 0x11:return "AGP 4X";
	case 0x12:return "PCI-X";
	case 0x13:return "AGP 8X";
	case 0xA0:return "PC-98/C20";
	case 0xA1:return "PC-98/C24";
	case 0xA2:return "PC-98/E";
	case 0xA3:return "PC-98/Local Bus";
	case 0xA4:return "PC-98/Card";
	case 0xA5:return "PCI Express";
	case 0xA6:return "PCI Express X1";
	case 0xA7:return "PCI Express X2";
	case 0xA8:return "PCI Express X4";
	case 0xA9:return "PCI Express X8";
	case 0xAA:return "PCI Express X16";
	default:return "(none)";
	}
}

BYTE *GenSlotDataBusWidth(BYTE value)
{
	switch (value)
	{
	case 0x01:return "Other";
	case 0x02:return "Unknown";
	case 0x03:return "8 bit";
	case 0x04:return "16 bit";
	case 0x05:return "32 bit";
	case 0x06:return "64 bit";
	case 0x07:return "128 bit";
	case 0x08:return "1x or x1";
	case 0x09:return "2x or x2";
	case 0x0A:return "4x or x4";
	case 0x0B:return "8x or x8";
	case 0x0C:return "12x or x12";
	case 0x0D:return "16x or x16";
	case 0x0E:return "32x or x32";
	default:return "(none)";
	}
}

BYTE *GenCurrentUsage(BYTE value)
{
	switch (value)
	{
	case 0x01:return "Other";
	case 0x02:return "Unknown";
	case 0x03:return "Available";
	case 0x04:return "In use";
	default:return "(none)";
	}
}

BYTE *GenMemoryType(BYTE value)
{
	switch (value)
	{
	case 0x01:return "Other";
	case 0x02:return "Unknown";
	case 0x03:return "DRAM";
	case 0x04:return "EDRAM";
	case 0x05:return "VRAM";
	case 0x06:return "SRAM";
	case 0x07:return "RAM";
	case 0x08:return "ROM";
	case 0x09:return "FLASH";
	case 0x0A:return "EEPROM";
	case 0x0B:return "FEPROM";
	case 0x0C:return "EPROM";
	case 0x0D:return "CDRAM";
	case 0x0E:return "3DRAM";
	case 0x0F:return "SDRAM";
	case 0x10:return "SGRAM";
	case 0x11:return "RDRAM";
	case 0x12:return "DDR";
	case 0x13:return "DDR2";
	case 0x14:return "DDR2 FB-DIMM";
	default:return "(none)";
	}
}

void BIOSInfo(DWORD handle)
{
	SMBBIOSInfo *bios;
	DWORD data;
	bios = (SMBBIOSInfo*)handle;
	data = handle + bios->Header.Length;
	printf("/n[BIOS Information]/n");
	printf("Vendor:%s/n", ReadStr(data, bios->Vendor));
	printf("Version:%s/n", ReadStr(data, bios->Version));
}

void SystemInfo(DWORD handle)
{
	SMBSysInfo *sys;
	DWORD data;
	sys = (SMBSysInfo *)handle;
	data = handle + sys->Header.Length;
	printf("/n[System Information]/n");
	printf("Manufacturer:%s/n", ReadStr(data, sys->Manufacturer));
	printf("Product Name:%s/n", ReadStr(data, sys->ProductName));
	printf("Version:%s/n", ReadStr(data, sys->Version));
}

void ProcessorInfo(DWORD handle)
{
	SMBProcessorInfo *cpu;
	DWORD data;
	cpu = (SMBProcessorInfo *)handle;
	data = handle + cpu->Header.Length;
	printf("/n[Processor Information]/n");
	printf("Socket:%s/n", ReadStr(data, cpu->SocketDesignation));
	printf("Type:%s/n", GenProcessorType(cpu->Type));
	printf("Family:%x/n", cpu->Family);
	printf("Manufacturer:%s/n", ReadStr(data, cpu->Manufacturer));
	printf("ID:%08lx%08lx/n", *((DWORD *)cpu->ID), *((DWORD *)(cpu->ID + 4)));
	printf("Version:%s/n", ReadStr(data, cpu->Version));
	printf("External Clock:%dMHz (0 for Unknown)/n", cpu->ExternalClock);
	printf("Max Speed:%dMHz (0 for Unknown)/n", cpu->MaxSpeed);
	printf("Current Speed:%dMHz (0 for Unknown)/n", cpu->CurrentSpeed);
	printf("Status:%x/n", cpu->Status);
	if (cpu->Header.Length <= 0x23) return;
	printf("Core Count:%d/n", cpu->CoreCount);
	printf("Core Enabled:%d/n", cpu->CoreEnabled);
	printf("Thread Count:%d/n", cpu->ThreadCount);
}

WORD CalCacheSize(WORD value)
{
	WORD result;
	if (CheckBit(value, 15)) result = 16;
	else result = 1;
	return result *= (value & 0x7fff);
}

void CacheInfo(DWORD handle)
{
	SMBCacheInfo *cache;
	DWORD data;
	cache = (SMBCacheInfo *)handle;
	data = handle + cache->Header.Length;
	printf("/n[Cache Information]/n");
	printf("Socket:%s/n", ReadStr(data, cache->SocketDesignation));
	printf("Maximum Cache Size:%dKB/n", CalCacheSize(cache->MaxCacheSize));
	printf("Installed Size:%dKB/n", CalCacheSize(cache->InstalledSize));
	printf("Supported SRAM Type:%s/n", GenSRAMType(cache->SupportedSRAMType));
	printf("Current SRAM Type:%s/n", GenSRAMType(cache->CurrentSRAMType));
}

void SystemSlots(DWORD handle)
{
	SMBSystemSlots *slot;
	DWORD data;
	slot = (SMBSystemSlots *)handle;
	data = handle + slot->Header.Length;
	printf("/n[System Slots]/n");
	printf("Slot:%s/n", ReadStr(data, slot->Designation));
	printf("Type:%s/n", GenSlotType(slot->Type));
	printf("Data Bus Width:%s/n", GenSlotDataBusWidth(slot->DataBusWidth));
	printf("Current Usage:%s/n", GenCurrentUsage(slot->CurrentUsage));
	printf("ID:%d/n", slot->ID);
}


void MemoryDevice(DWORD handle)
{
	SMBMemDevice *mem = (SMBMemDevice *)handle;
	DWORD data = handle + mem->Header.Length;
	printf("/n[Memory Device]/n");
	if ((mem->Size & 0x7fff) == 0)
	{
		printf("Device Locator:%s/n", ReadStr(data, mem->DeviceLocator));
		printf("Memory Type:%s/n", GenMemoryType(mem->MemoryType));
		printf("Status:Not Installed!/n");
		return;
	}
	printf("Total Width:%dbit/n", mem->TotalWidth);
	printf("Data Width:%dbit/n", mem->DataWidth);

	if (CheckBit(mem->Size, 15))
		printf("Size:%dKB/n", mem->Size & 0x7fff);
	else
		printf("Size:%dMB/n", mem->Size & 0x7fff);
	printf("Device Locator:%s/n", ReadStr(data, mem->DeviceLocator));
	printf("Memory Type:%s/n", GenMemoryType(mem->MemoryType));
	if (mem->Header.Length <= 0x15)return;
	printf("Speed:%dMHz/n", mem->Speed);
	printf("Manufacturer:%s/n", ReadStr(data, mem->Manufacturer));
	printf("Serial Number:%s/n", ReadStr(data, mem->SerialNumber));
}

void BaseBoardInfo(DWORD handle)
{
	SMBBaseBoardInfo *board = (SMBBaseBoardInfo *)handle;
	DWORD data = handle + board->Header.Length;
	printf("/n[Base Board Information]/n");
	printf("Manufacturer:%s/n", ReadStr(data, board->Manufacturer));
	printf("Product:%s/n", ReadStr(data, board->Product));
	printf("Version:%s/n", ReadStr(data, board->Version));
	printf("Serial Number:%s/n", ReadStr(data, board->SerialNumber));
}

BYTE *GenPortConType(BYTE value)
{
	switch (value)
	{
	case 0x00:return "None";
	case 0x01:return "Centronics";
	case 0x02:return "Mini Centronics";
	case 0x03:return "Proprietary";
	case 0x04:return "DB-25 pin male";
	case 0x05:return "DB-25 pin female";
	case 0x06:return "DB-15 pin male";
	case 0x07:return "DB-15 pin female";
	case 0x08:return "DB-9 pin male";
	case 0x09:return "DB-9 pin female";
	case 0x0A:return "RJ-11";
	case 0x0B:return "RJ-45";
	case 0x0C:return "50 Pin MiniSCSI";
	case 0x0D:return "Mini-DIN";
	case 0x0E:return "Micro-DIN";
	case 0x0F:return "PS/2";
	case 0x10:return "Infrared";
	case 0x11:return "HP-HIL";
	case 0x12:return "Access Bus (USB)";
	case 0x13:return "SSA SCSI";
	case 0x14:return "Circular DIN-8 male";
	case 0x15:return "Circular DIN-8 female";
	case 0x16:return "On Board IDE";
	case 0x17:return "On Board Floppy";
	case 0x18:return "9 Pin Dual Inline (pin 10 cut)";
	case 0x19:return "25 Pin Dual Inline (pin 26 cut)";
	case 0x1A:return "50 Pin Dual Inline";
	case 0x1B:return "68 Pin Dual Inline";
	case 0x1C:return "On Board Sound Input from CD-ROM";
	case 0x1D:return "Mini-Centronics Type-14";
	case 0x1E:return "Mini-Centronics Type-26";
	case 0x1F:return "Mini-jack (headphones)";
	case 0x20:return "BNC";
	case 0x21:return "1394";
	case 0x22:return "SAS/SATA Plug Receptacle";
	case 0xA0:return "PC-98";
	case 0xA1:return "PC-98Hireso";
	case 0xA2:return "PC-H98";
	case 0xA3:return "PC-98Note";
	case 0xA4:return "PC-98Full";
	case 0xFF:return "Other";
	default:return "(none)";
	}
}

BYTE *GenPortType(BYTE value)
{
	switch (value)
	{
	case 0x00:return "None";
	case 0x01:return "Parallel Port XT/AT Compatible";
	case 0x02:return "Parallel Port PS/2";
	case 0x03:return "Parallel Port ECP";
	case 0x04:return "Parallel Port EPP";
	case 0x05:return "Parallel Port ECP/EPP";
	case 0x06:return "Serial Port XT/AT Compatible";
	case 0x07:return "Serial Port 16450 Compatible";
	case 0x08:return "Serial Port 16550 Compatible";
	case 0x09:return "Serial Port 16550A Compatible";
	case 0x0A:return "SCSI Port";
	case 0x0B:return "MIDI Port";
	case 0x0C:return "Joy Stick Port";
	case 0x0D:return "Keyboard Port";
	case 0x0E:return "Mouse Port";
	case 0x0F:return "SSA SCSI";
	case 0x10:return "USB";
	case 0x11:return "FireWire (IEEE P1394)";
	case 0x12:return "PCMCIA Type I";
	case 0x13:return "PCMCIA Type II";
	case 0x14:return "PCMCIA Type III";
	case 0x15:return "Cardbus";
	case 0x16:return "Access Bus Port";
	case 0x17:return "SCSI II";
	case 0x18:return "SCSI Wide";
	case 0x19:return "PC-98";
	case 0x1A:return "PC-98Hireso";
	case 0x1B:return "PC-H98";
	case 0x1C:return "Video Port";
	case 0x1D:return "Audio Port";
	case 0x1E:return "Modem Port";
	case 0x1F:return "Network Port";
	case 0x20:return "SATA";
	case 0x21:return "SAS";
	case 0xA0:return "8251 Compatible";
	case 0xA1:return "8251 FIFO Compatible";
	case 0xFF:return "Other";
	default:return "(none)";
	}
}


void PortConInfo(DWORD handle)
{
	SMBPortInfo *port = (SMBPortInfo *)handle;
	DWORD data = handle + port->Header.Length;
	printf("/n[Port Connector Information]/n");
	printf("Internal Reference Designator:%s/n", ReadStr(data, port->InterRefDesignator));
	printf("Internal Connector Type:%s/n", GenPortConType(port->InterConType));
	printf("External Reference Designator:%s/n", ReadStr(data, port->ExterRefDesignator));
	printf("External Connector Type:%s/n", GenPortConType(port->ExterConType));
	printf("Port Type:%s/n", GenPortType(port->PortType));
}

BYTE *GenDeviceType(BYTE value)
{
	switch (value)
	{
	case 0x01:return "Other";
	case 0x02:return "Unknown";
	case 0x03:return "Video";
	case 0x04:return "SCSI Controller";
	case 0x05:return "Ethernet";
	case 0x06:return "Token Ring";
	case 0x07:return "Sound";
	case 0x08:return "PATA Controller";
	case 0x09:return "SATA Controller";
	case 0x0A:return "SAS Controller";
	default:return "(none)";
	}
}

void OnBoardDevInfo(DWORD handle)
{
	OnBoardDeviceInfo *dev = (OnBoardDeviceInfo *)handle;
	DWORD data = handle + dev->Header.Length;
	BYTE count = (dev->Header.Length - 4) / 2;
	BYTE i = 1;
	printf("/n[On Board Device Information]/n");
	for (; i <= count; i++)
	{
		printf("Device Type:%s/n", GenDeviceType(0x7f & (*(BYTE *)(handle + 4 + 2 * (i - 1)))));
		printf("Description:%s/n", ReadStr(data, *(BYTE *)(handle + 5 + 2 * (i - 1))));
	}
}