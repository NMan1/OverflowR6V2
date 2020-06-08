#pragma once
#include "imports.h"


NTSTATUS GetPid(HANDLE* pid, const char* process)
{
	// ZwQuery
	ULONG CallBackLength = 0;
	PSYSTEM_PROCESS_INFO PSI = NULL;
	PSYSTEM_PROCESS_INFO pCurrent = NULL;
	PVOID BufferPid = NULL;
	NTSTATUS Status = STATUS_UNSUCCESSFUL;

	ANSI_STRING AS;
	UNICODE_STRING process_name;

	RtlInitAnsiString(&AS, process);
	RtlAnsiStringToUnicodeString(&process_name, &AS, TRUE);

	DbgPrintEx(0, 0, "\nSearch for %wZ process id", process_name);

here:; // need to loop because new process spawn making our buffer already to small
	if (!NT_SUCCESS(ZwQuerySystemInformation(SystemProcessInformation, NULL, NULL, &CallBackLength)))
	{
		BufferPid = ExAllocatePoolWithTag(NonPagedPool, CallBackLength, 0x616b7963); // aykc 
		if (!BufferPid)
		{
			DbgPrintEx(0, 0, "Failed To Allocate Buffer Notify Routine");
			return Status;
		}

		PSI = (PSYSTEM_PROCESS_INFO)BufferPid;
		Status = ZwQuerySystemInformation(SystemProcessInformation, PSI, CallBackLength, NULL);
		if (!NT_SUCCESS(Status))
		{
			//DbgPrintEx(0, 0, "Failed To Get Query System Process Information List: %p", Status);
			ExFreePoolWithTag(BufferPid, 0x616b7963);
			goto here;
			return Status = STATUS_INFO_LENGTH_MISMATCH;
		}
		DbgPrintEx(0, 0, "\nSearching For PID...");
		do
		{
			if (PSI->NextEntryOffset == 0)
				break;

			if (RtlEqualUnicodeString(&process_name, &PSI->ImageName, FALSE))
			{
				DbgPrintEx(0, 0, "PID %d | NAME %ws\n", PSI->UniqueProcessId, PSI->ImageName.Buffer);
				*pid = PSI->UniqueProcessId;
				Status = STATUS_SUCCESS;
				break;
			}

			PSI = (PSYSTEM_PROCESS_INFO)((unsigned char*)PSI + PSI->NextEntryOffset); // Calculate the address of the next entry.

		} while (PSI->NextEntryOffset);

		// Free Allocated Memory
		ExFreePoolWithTag(BufferPid, 0x616b7963);
	}

	return Status;
}

PVOID get_system_module_base(const char* module_name) {

	ULONG bytes = 0;
	NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, 0, bytes, &bytes);

	if (!bytes)
		return 0;

	PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, bytes, 0x454E4F45); // 'ENON'

	status = ZwQuerySystemInformation(SystemModuleInformation, modules, bytes, &bytes);

	if (!NT_SUCCESS(status))
		return 0;

	PRTL_PROCESS_MODULE_INFORMATION module = modules->Modules;
	PVOID module_base = 0, module_size = 0;

	for (ULONG i = 0; i < modules->NumberOfModules; i++)
	{

		if (strcmp((char*)module[i].FullPathName, module_name) == 0)
		{
			module_base = module[i].ImageBase;
			module_size = (PVOID)module[i].ImageSize;
			break;
		}
	}

	if (modules)
		ExFreePoolWithTag(modules, 0);

	if (module_base <= 0)
		return 0;

	return module_base;
}

PVOID get_system_module_export(const char* module_name, LPCSTR routine_name)
{
	PVOID lpModule = get_system_module_base(module_name);

	if (!lpModule)
		return NULL;

	return RtlFindExportedRoutineByName(lpModule, routine_name);
}

BOOLEAN read_memory(void* address, void* buffer, size_t size) {
	if (!RtlCopyMemory(buffer, address, size)) {
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOLEAN write_memory(void* address, void* buffer, size_t size) {
	if (!RtlCopyMemory(address, buffer, size)) {
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

BOOLEAN write_to_read_only_memory(void* address, void* buffer, size_t size) {

	PMDL Mdl = IoAllocateMdl(address, size, FALSE, FALSE, NULL);

	if (!Mdl)
		return FALSE;

	// Locking and mapping memory with RW-rights:
	MmProbeAndLockPages(Mdl, KernelMode, IoReadAccess);
	PVOID Mapping = MmMapLockedPagesSpecifyCache(Mdl, KernelMode, MmNonCached, NULL, FALSE, NormalPagePriority);
	MmProtectMdlSystemAddress(Mdl, PAGE_READWRITE);

	// Write your buffer to mapping:
	write_memory(Mapping, buffer, size);

	// Resources freeing:
	MmUnmapLockedPages(Mapping, Mdl);
	MmUnlockPages(Mdl);
	IoFreeMdl(Mdl);

	return TRUE;
}

static unsigned __int64 PatternScan(void* start, size_t length, const char* pattern, const char* mask)
{
	const char* data = (const char*)start;
	const auto pattern_length = strlen(mask);

	for (size_t i = 0; i <= length - pattern_length; i++)
	{
		BOOLEAN accumulative_found = TRUE;

		for (size_t j = 0; j < pattern_length; j++)
		{
			if (!MmIsAddressValid((void*)((unsigned __int64)data + i + j)))
			{
				accumulative_found = FALSE;
				break;
			}

			if (data[i + j] != pattern[j] && mask[j] != '?')
			{
				accumulative_found = FALSE;
				break;
			}
		}

		if (accumulative_found)
		{
			return (unsigned __int64)((unsigned __int64)data + i);
		}
	}

	return NULL;
}

BOOLEAN call_kernel_function(void* kernel_function_address, LPCSTR funct_name) {
	if (!kernel_function_address)
		return FALSE;

	PVOID* function = (PVOID*)(get_system_module_export("\\SystemRoot\\System32\\drivers\\dxgkrnl.sys", funct_name));

	if (!function)
		return FALSE;

	uintptr_t our_func = (uintptr_t)(kernel_function_address);
	if (!our_func)
		return FALSE;

	// nops seem like the only way to avoid a beggining function trampoline
	BYTE shell_code[] =
	{
		//0x48, 0x89, 0x5c, 0x24, 0x10, //  mov [], rbx
		//0x48, 0x89, 0x74, 0x24, 0x18, //  mov [], rsi
		0x90,
		0x90,
		0x90, // nop
		0x48, 0xB8, // mov rax, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // [xxx]
		//0x48, 0x81, 0xec, 0x40, 0x01, 0x00, 0x00, // sub rsp, 140h
		0x90,
		0x48, 0xB8, // mov rax, 
		0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // [xxx]
		//0x48, 0x8B, 0xF1, // mov rsi, rcx
		0xFF, 0xE0, // jmp rax // d0 for call
		0xCC, // int3
	};

	DbgPrintEx(0, 0, "\nFunction: %p\n", function);
	DbgPrintEx(0, 0, "Our function: %p\n", our_func);

	memcpy(shell_code + 16, &our_func, sizeof(our_func)); // 6
	write_to_read_only_memory(function, &shell_code, sizeof(shell_code));

	return TRUE;
}

BOOLEAN write_kernel_memory(HANDLE pid, uintptr_t address, void* buffer, SIZE_T size) {
	if (!address || !buffer || !size)
		return FALSE;

	NTSTATUS Status = STATUS_SUCCESS;
	PEPROCESS process;
	PsLookupProcessByProcessId(pid, &process);

	KAPC_STATE state;
	KeStackAttachProcess((PKPROCESS)process, &state);

	MEMORY_BASIC_INFORMATION info;
	DbgPrintEx(0, 0, "Write-in: %p\n", address);

	Status = ZwQueryVirtualMemory(ZwCurrentProcess(), (PVOID)address, MemoryBasicInformation, &info, sizeof(info), NULL);
	if (!NT_SUCCESS(Status)) {
		KeUnstackDetachProcess(&state);
		return FALSE;
	}

	if (((uintptr_t)info.BaseAddress + info.RegionSize) < (address + size))
	{
		KeUnstackDetachProcess(&state);
		return FALSE;
	}

	if (!(info.State & MEM_COMMIT) || (info.Protect & (PAGE_GUARD | PAGE_NOACCESS)))
	{
		KeUnstackDetachProcess(&state);
		return FALSE;
	}

	if ((info.Protect & PAGE_EXECUTE_READWRITE) || (info.Protect & PAGE_EXECUTE_WRITECOPY) || (info.Protect & PAGE_READWRITE) || (info.Protect & PAGE_WRITECOPY))
	{
		RtlCopyMemory((void*)address, buffer, size);
	}
	KeUnstackDetachProcess(&state);
	ObDereferenceObject(process);
	return TRUE;
}

NTSTATUS protect_virtual_memory(HANDLE pid, PVOID address, ULONG size, ULONG protection, ULONG* protection_out)
{
	if (!pid || !address || !size || !protection)
		return STATUS_INVALID_PARAMETER;

	NTSTATUS status = STATUS_SUCCESS;
	PEPROCESS target_process = NULL;

	if (!NT_SUCCESS(PsLookupProcessByProcessId(pid, &target_process)))
	{
		return STATUS_NOT_FOUND;
	}

	ULONG protection_old = 0;

	KAPC_STATE state;
	KeStackAttachProcess(target_process, &state);

	status = ZwProtectVirtualMemory(NtCurrentProcess(), &address, &size, protection, &protection_old);

	KeUnstackDetachProcess(&state);

	if (NT_SUCCESS(status))
		*protection_out = protection_old;

	ObDereferenceObject(target_process);
	return status;
}

BOOLEAN read_kernel_memory(HANDLE pid, uintptr_t address, void* buffer, SIZE_T size) {
	if (!address || !buffer || !size)
		return FALSE;
	SIZE_T bytes = 0;
	NTSTATUS status = STATUS_SUCCESS;
	PEPROCESS process;
	PsLookupProcessByProcessId((HANDLE)pid, &process);
	DbgPrintEx(0, 0, "Read-in: %p\n", address);
	status = MmCopyVirtualMemory(process, (void*)address, (PEPROCESS)PsGetCurrentProcess(), buffer, size, KernelMode, &bytes);
	if (!NT_SUCCESS(status))
	{
		DbgPrintEx(0, 0, "Read-failed, bytes copied: %x\n", bytes);
		return FALSE;
	}
	else
	{
		DbgPrintEx(0, 0, "Read-out: %p\n\n", buffer);
		return TRUE;
	}
	ObDereferenceObject(process);
}

ULONG64 get_module_base_x64(PEPROCESS proc, UNICODE_STRING module_name) {
	PPEB pPeb = PsGetProcessPeb(proc);

	if (!pPeb) {
		return 0; // failed
	}

	KAPC_STATE state;

	KeStackAttachProcess(proc, &state);

	PPEB_LDR_DATA pLdr = (PPEB_LDR_DATA)pPeb->Ldr;

	if (!pLdr) {
		KeUnstackDetachProcess(&state);
		return 0; // failed
	}

	// loop the linked list
	for (PLIST_ENTRY list = (PLIST_ENTRY)pLdr->ModuleListLoadOrder.Flink;
		list != &pLdr->ModuleListLoadOrder; list = (PLIST_ENTRY)list->Flink) {
		PLDR_DATA_TABLE_ENTRY pEntry =
			CONTAINING_RECORD(list, LDR_DATA_TABLE_ENTRY, InLoadOrderModuleList);
		if (RtlCompareUnicodeString(&pEntry->BaseDllName, &module_name, TRUE) ==
			0) {
			ULONG64 baseAddr = (ULONG64)pEntry->DllBase;
			KeUnstackDetachProcess(&state);
			return baseAddr;
		}
	}
	KeUnstackDetachProcess(&state);

	return 0;
}

DWORD64 virtual_alloc(DWORD64 BaseAddress, ULONG AllocType, ULONG Protection, SIZE_T Size, HANDLE pid)
{
	NTSTATUS Status = STATUS_SUCCESS;
	KAPC_STATE apc;
	ULONG old_protection;
	DWORD64 Addy = BaseAddress;
	PEPROCESS process;

	PsLookupProcessByProcessId(pid, &process);

	KeStackAttachProcess(process, &apc);
	Status = ZwAllocateVirtualMemory(ZwCurrentProcess(), &Addy, 0, &Size, AllocType, Protection);
	KeUnstackDetachProcess(&apc);

	if (!NT_SUCCESS(Status))
	{
		DbgPrintEx(0, 0, "ZwAllocateVirtualMemory Failed:%p\n", Status);
		return Status;
	}
	ObDereferenceObject(process);

	return Addy;
}
