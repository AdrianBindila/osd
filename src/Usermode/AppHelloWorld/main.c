#include "common_lib.h"
#include "syscall_if.h"
#include "um_lib_helper.h"

FUNC_ThreadStart _LightProjectApp;

STATUS
__main(
	DWORD       argc,
	char** argv
)
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);
	STATUS status;
	TID tid;

	status = SyscallThreadGetTid(UM_INVALID_HANDLE_VALUE, &tid);
	if (!SUCCEEDED(status))
	{
		LOG_FUNC_ERROR("SyscallThreadGetTid", status);
		return status;
	}

	LOG("Hello from thread with ID 0x%X\n", tid);

	char threadName[256];
	int threadNameMaxLen = 0;
	int stop = 0;
	while (stop < 2)//stop at thread length + 1
	{
		status = SyscallThreadGetName(threadName, threadNameMaxLen);
		if (!SUCCEEDED(status))//truncated thread name
		{
			LOG_FUNC_ERROR("SyscallThreadGetName", status);
			return status;
		}
		
		stop++;

		LOG("Hello from thread with NAME %s\n", threadName);
		threadNameMaxLen++;
	}

	QWORD threadNo = 0;
	status = SyscallGetTotalThreadNo(&threadNo);
	if (!SUCCEEDED(status))
	{
		LOG_FUNC_ERROR("SyscallGetTotalThreadNo", status);
		return status;
	}
	LOG("Total number of threads:%d", threadNo);

	PVOID stackAdress;
	status = SyscallGetThreadUmStackAddress(&stackAdress);
	if (!SUCCEEDED(status))
	{
		LOG_FUNC_ERROR("SyscallGetThreadUmStackAddress", status);
		return status;
	}
	LOG("Stack Address is: %p", stackAdress);

	QWORD stackSize;
	status = SyscallGetThreadUmStackSize(&stackSize);
	if (!SUCCEEDED(status))
	{
		LOG_FUNC_ERROR("SyscallGetThreadUmStackSize", status);
		return status;
	}
	LOG("Stack size is: %d", stackSize);

	PVOID entryPoint;
	status = SyscallGetThreadUmEntryPoint(&entryPoint);
	if (!SUCCEEDED(status))
	{
		LOG_FUNC_ERROR("SyscallGetThreadUmEntryPoint", status);
		return status;
	}
	LOG("Usermode entry point is %p", entryPoint);

	return STATUS_SUCCESS;
}

STATUS
(__cdecl _LightProjectApp)(
	IN_OPT      PVOID       Context
	)
{
	STATUS status;
	TID tid;

	ASSERT(Context != NULL);

	status = SyscallThreadGetTid(UM_INVALID_HANDLE_VALUE, &tid);
	if (!SUCCEEDED(status))
	{
		LOG_FUNC_ERROR("SyscallThreadGetTid", status);
		return status;
	}

	LOG("Hello from thread with ID 0x%X\n", tid);
	LOG("Context is 0x%X\n", Context);

	return status;
}
