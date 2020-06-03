#include <ntifs.h>
#include "imports.h"
#include "cleaner.h"
#include "helpers.h"

HANDLE pid;
ULONG64 base_addy = NULL;

NTSTATUS hookedfunc(PVOID called_param)
{
	static BOOLEAN do_once = TRUE;
	if (do_once)
	{
		DbgPrintEx(0, 0, "Hooked");
		do_once = FALSE;
	}

	copy_memory* m = (copy_memory*)called_param;

	if (m->get_pid != FALSE)
		GetPid(&pid);
	else if (m->change_protection != FALSE)
		protect_virtual_memory(pid, (PVOID)m->address, m->size, m->protection, m->protection_old);
	else if (m->get_base != FALSE)
	{
		ANSI_STRING AS;
		UNICODE_STRING ModuleNAme;

		RtlInitAnsiString(&AS, m->module_name);
		RtlAnsiStringToUnicodeString(&ModuleNAme, &AS, TRUE);

		PEPROCESS process;
		PsLookupProcessByProcessId((HANDLE)pid, &process);
		base_addy = get_module_base_x64(process, ModuleNAme);
		DbgPrintEx(0, 0, "Base aquired: %p\n", base_addy);
		m->base_address = base_addy;
		RtlFreeUnicodeString(&ModuleNAme);
	}
	else if (m->write != FALSE) 
	{
		PVOID kernelBuff = ExAllocatePool(NonPagedPool, m->size);

		if (!kernelBuff)
			return STATUS_UNSUCCESSFUL;

		if (!memcpy(kernelBuff, m->buffer_address, m->size))
			return STATUS_UNSUCCESSFUL;

		PEPROCESS Process;
		PsLookupProcessByProcessId((HANDLE)pid, &Process);
		write_kernel_memory((HANDLE)pid, m->address, kernelBuff, m->size);
		ExFreePool(kernelBuff);
	}
	else if (m->read != FALSE)
	{
		read_kernel_memory((HANDLE)pid, m->address, m->output, m->size);
	}
	else if (m->read_string != FALSE) 
	{
		PVOID kernelBuffer = ExAllocatePool(NonPagedPool, m->size);

		if (!kernelBuffer)
			return STATUS_UNSUCCESSFUL;


		if (!memcpy(kernelBuffer, m->buffer_address, m->size))
			return STATUS_UNSUCCESSFUL;

		read_kernel_memory((HANDLE)pid, m->address, kernelBuffer, m->size);

		RtlZeroMemory(m->buffer_address, m->size);

		if (!memcpy(m->buffer_address, kernelBuffer, m->size))
			return STATUS_UNSUCCESSFUL;

		ExFreePool(kernelBuffer);
	}
	else if (m->write_string != FALSE) 
	{
		PVOID kernelBuffer1 = ExAllocatePool(NonPagedPool, m->size);

		if (!kernelBuffer1)
			return STATUS_UNSUCCESSFUL;

		if (!memcpy(kernelBuffer1, m->buffer_address, m->size))
			return STATUS_UNSUCCESSFUL;

		PEPROCESS Process;
		PsLookupProcessByProcessId((HANDLE)pid, &Process);
		write_kernel_memory((HANDLE)pid, m->address, kernelBuffer1, m->size);

		ExFreePool(kernelBuffer1);
	}
	else if (m->alloc_type != FALSE)
	{
		PVOID AllocatedMemory = virtual_alloc(m->address, MEM_COMMIT, m->alloc_type, m->size, pid);
		m->output = AllocatedMemory;
		DbgPrintEx(0, 0, "\nAllocated at: %p\n", AllocatedMemory);
	}
	return STATUS_SUCCESS;
}

NTSTATUS real_entry()
{
	DbgPrintEx(0, 0, "Real Entry Called.\n");

	clean_piddbcachetalbe();
	call_kernel_function(&hookedfunc, "NtDxgkGetTrackedWorkloadStatistics");

	return STATUS_SUCCESS;
}

NTSTATUS DriverEntry(_In_ PDRIVER_OBJECT DriverObj, _In_ PUNICODE_STRING RegistryPath)
{
	DbgPrintEx(0, 0, "Driver Created.\n");

	// Fix Paramms
	UNREFERENCED_PARAMETER(RegistryPath);
	UNREFERENCED_PARAMETER(DriverObj);

	real_entry();

	return STATUS_SUCCESS;
}