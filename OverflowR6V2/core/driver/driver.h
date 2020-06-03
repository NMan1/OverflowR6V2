#pragma once
#include "imports.h"
#include <string_view>

typedef struct _copy_memory
{
	BOOLEAN read;
	BOOLEAN read_string;
	void* buffer_address;
	UINT_PTR  address;
	ULONGLONG size;
	void* output;

	BOOLEAN   write;
	BOOLEAN write_string;

	BOOLEAN  get_base;
	ULONG64 base_address;
	const char* module_name;

	BOOLEAN get_pid;
	ULONG pid_of_source;

	BOOLEAN alloc_memory;
	ULONG	alloc_type;

	BOOLEAN		change_protection;
	ULONG		protection;
	ULONG		protection_old;
}copy_memory;

template<typename ... A>
uint64_t call_hook(const A ... arguments)
{
	void* control_function = GetProcAddress(LoadLibrary("win32u.dll"), "NtDxgkGetTrackedWorkloadStatistics");
	const auto control = static_cast<uint64_t(__stdcall*)(A...)>(control_function);

	return control(arguments ...);
}

namespace driver
{

	static void get_process_id(std::string_view process_name)
	{
		copy_memory m = { 0 };
		m.get_pid = TRUE;
		m.get_base = FALSE;
		m.read = FALSE;
		m.read_string = FALSE;
		m.write = FALSE;
		m.write_string = FALSE;
		m.change_protection = FALSE;
		m.alloc_memory = FALSE;
		call_hook(&m);
	}

	static ULONG64 change_protection(uint64_t address, uint32_t page_protection, std::size_t size)
	{
		copy_memory m = { 0 };
		m.address = address;
		m.protection = page_protection;
		m.size = size;
		m.change_protection = TRUE;
		m.get_pid = FALSE;
		m.get_base = FALSE;
		m.read = FALSE;
		m.read_string = FALSE;
		m.write = FALSE;
		m.write_string = FALSE;
		m.alloc_memory = FALSE;
		m.protection_old = 0;

		call_hook(&m);
		return m.protection_old;
	}

	static ULONG64 get_module_base_address(const char* module_name)
	{

		copy_memory m = { 0 };
		m.get_base = TRUE;
		m.read = FALSE;
		m.get_pid = FALSE;
		m.read_string = FALSE;
		m.write = FALSE;
		m.write_string = FALSE;
		m.module_name = module_name;
		m.change_protection = FALSE;
		m.alloc_memory = FALSE;
		call_hook(&m);

		ULONG64 base = NULL;
		base = m.base_address;
		return base;
	}

	template <class T>
	T read(UINT_PTR ReadAddress)
	{

		T response{};

		copy_memory m;
		m.size = sizeof(T);
		m.address = ReadAddress;
		m.read = TRUE;
		m.get_pid = FALSE;
		m.read_string = FALSE;
		m.write_string = FALSE;
		m.write = FALSE;
		m.get_base = FALSE;
		m.change_protection = FALSE;
		m.alloc_memory = FALSE;
		m.output = &response;

		call_hook(&m);

		return response;
	}

	static bool WriteVirtualMemoryRaw(UINT_PTR WriteAddress, UINT_PTR SourceAddress, SIZE_T WriteSize);

	template<typename S>
	bool write(UINT_PTR WriteAddress, const S& value)
	{
		return WriteVirtualMemoryRaw(WriteAddress, (UINT_PTR)&value, sizeof(S));
	}
	bool WriteVirtualMemoryRaw(UINT_PTR WriteAddress, UINT_PTR SourceAddress, SIZE_T WriteSize)
	{
		copy_memory m;
		m.address = WriteAddress;
		m.pid_of_source = GetCurrentProcessId();
		m.write = TRUE;
		m.get_pid = FALSE;
		m.read = FALSE;
		m.read_string = FALSE;
		m.get_base = FALSE;
		m.write_string = FALSE;
		m.change_protection = FALSE;
		m.buffer_address = (void*)SourceAddress;
		m.size = WriteSize;
		m.alloc_memory = FALSE;

		call_hook(&m);

		return true;
	}

	static void read_string(UINT_PTR String_address, void* buffer, SIZE_T size)
	{
		copy_memory m;
		m.read_string = TRUE;
		m.read = FALSE;
		m.get_pid = FALSE;
		m.get_base = FALSE;
		m.write = FALSE;
		m.write_string = FALSE;
		m.address = String_address;
		m.buffer_address = buffer;
		m.size = size;
		m.change_protection = FALSE;
		m.alloc_memory = FALSE;

		call_hook(&m);
	}

	static void write_string(UINT_PTR String_address, void* buffer, SIZE_T size)
	{
		copy_memory m;
		m.write_string = TRUE;
		m.read = FALSE;
		m.get_pid = FALSE;
		m.read_string = FALSE;
		m.get_base = FALSE;
		m.write = FALSE;
		m.address = String_address;
		m.buffer_address = buffer;
		m.size = size;
		m.change_protection = FALSE;
		m.alloc_memory = FALSE;

		call_hook(&m);
	}

	static UINT_PTR virtual_alloc(UINT_PTR ReadAddress, ULONG alloc_type, SIZE_T size)
	{
		copy_memory m;
		m.address = ReadAddress;
		m.read = FALSE;
		m.get_pid = FALSE;
		m.read_string = FALSE;
		m.write_string = FALSE;
		m.write = FALSE;
		m.get_base = FALSE;
		m.change_protection = FALSE;
		m.alloc_memory = TRUE;
		m.alloc_type = alloc_type;
		m.size = size;

		call_hook(&m);
		return *(UINT_PTR*)&m.output;
	}
}