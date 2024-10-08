#pragma once
#include "supermode_comm.h"
#include <windows.h>
#include <winternl.h>
#include <stdio.h>
#include <vector>
#include <tlhelp32.h>
#include <stdio.h>
#include <fstream>
#include <filesystem>
#include <random>
#include <string>
#include <direct.h>
#include <unordered_map>

// Purchase PREMIUM source @Cazzstar Display Name: Thermal5000
// Also selling undetected Fortnite, Apex, Valorant usermodes
// Crypto only ~~ Come money ready

#pragma comment( lib, "ntdll.lib" )

#define USE_PROCESS_CR3 -1

struct SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX
{
	PVOID Object;
	ULONG UniqueProcessId;
	ULONG HandleValue;
	ULONG GrantedAccess;
	USHORT CreatorBackTraceIndex;
	USHORT ObjectTypeIndex;
	ULONG HandleAttributes;
	ULONG Reserved;
};

struct SYSTEM_HANDLE_INFORMATION_EX
{
	ULONG NumberOfHandles;
	ULONG Reserved;
	SYSTEM_HANDLE_TABLE_ENTRY_INFO_EX Handles[1];
};

namespace supermode {
	static std::unordered_map<uintptr_t, uintptr_t> tlb;

	static uintptr_t EP_DIRECTORYTABLE = 0x028;
	static uintptr_t EP_UNIQUEPROCESSID = 0;
	static uintptr_t EP_ACTIVEPROCESSLINK = 0;
	static uintptr_t EP_VIRTUALSIZE = 0;
	static uintptr_t EP_SECTIONBASE = 0;
	static uintptr_t EP_IMAGEFILENAME = 0;

	static uintptr_t attached_cr3 = 0;
	static uint64_t attached_proc = 0;

	void get_eprocess_offsets();

	uintptr_t leak_kprocess();
	bool leak_kpointers(std::vector<uintptr_t>& pointers);

	bool read_virtual_memory(uintptr_t address, uint64_t* output, unsigned long size, uint64_t cr3 = USE_PROCESS_CR3);
	bool write_virtual_memory(uintptr_t address, uint64_t* data, unsigned long size, uint64_t cr3 = USE_PROCESS_CR3);

	static uintptr_t convert_virtual_to_physical(uintptr_t virtual_address, uint64_t cr3 = USE_PROCESS_CR3);

	uint32_t find_self_referencing_pml4e();
	uintptr_t find_dtb_from_base(uintptr_t base);

	template<typename T>
	T read_virtual_memory(uintptr_t address, bool* success, uint64_t cr3 = USE_PROCESS_CR3)
	{
		T buffer;

		*success = read_virtual_memory(address, (uintptr_t*)&buffer, sizeof(T), cr3);
		return buffer;
	}

	template<typename T>
	void write_virtual_memory(uintptr_t address, T val, uint64_t cr3 = USE_PROCESS_CR3) {
		write_virtual_memory(address, (uintptr_t*) & val, sizeof(T), cr3);
	}

	uint64_t get_process_id(const char* image_name);
	uintptr_t get_process_base_um(uint64_t pid, const char* name);
	uintptr_t get_dtb_from_kprocess(uintptr_t kprocess);
	uintptr_t attach(const char* image_name, uintptr_t* out_cr3, uintptr_t* out_kprocess);
}