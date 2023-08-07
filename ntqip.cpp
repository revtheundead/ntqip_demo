#include "ntqiph.h"

typedef LONG NTSTATUS;

typedef NTSTATUS(NTAPI* TNtQueryInformationProcess) (
	IN HANDLE			ProcessHandle,
	IN PROCESSINFOCLASS	ProcessInformationClass,
	OUT PVOID			ProcessInformation,
	IN ULONG			ProcessInformationLength,
	OUT PULONG			ReturnLength
);

int main(int argc, char* argv[]) {
	if (argc < 2) {
		warn("Usage: ntqip_demo.exe <pid>");
		return EXIT_FAILURE;
	}

	DWORD pid = atoi(argv[1]);

	info("Trying to open a handle to the process with PID %ld", pid);
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	if (hProcess == NULL) {
		warn("Could not get a handle to the process with PID %ld, error: %ld", pid, GetLastError());
		return EXIT_FAILURE;
	}

	okay("Got a handle to the process with PID %ld\n\\__[ HANDLE\n\t\\__0x%p]", pid, hProcess);

	info("Trying to get a handle to ntdll.dll...");
	HMODULE hNtdll = LoadLibraryA("ntdll.dll");

	if (hNtdll == NULL) {
		warn("Could not get a handle to ntdll.dll, error: %ld", GetLastError());
		return EXIT_FAILURE;
	}

	okay("Got a handle to ntdll.dll");

	info("Trying to get function 'NtQueryInformationProcess'...");
	auto pfnNtQueryInformationProcess = (TNtQueryInformationProcess)GetProcAddress(
		hNtdll, "NtQueryInformationProcess"
	);

	if (pfnNtQueryInformationProcess == NULL) {
		warn("Could not get function 'NtQueryInformationProcess', error: %ld", GetLastError());
		return EXIT_FAILURE;
	}

	PROCESS_BASIC_INFORMATION pbi;
	ULONG returnLength;

	info("Executing external function for process with PID %ld", pid);
	NTSTATUS status = pfnNtQueryInformationProcess(
		hProcess,
		ProcessBasicInformation,
		&pbi,
		sizeof(pbi),
		&returnLength
	);

	if (NT_SUCCESS(status)) {
		PPEB pPeb = pbi.PebBaseAddress;

		if (pPeb) {
			BYTE bBeingDebugged = 0;

			if (ReadProcessMemory(hProcess, &pPeb->BeingDebugged, &bBeingDebugged, sizeof(bBeingDebugged), nullptr)) {
				if (bBeingDebugged) {
					warn("Debugger detected! Exiting...");
				}
				else {
					okay("No debugger detected, executing payload...");
					MessageBoxW(NULL, L"NIGHTMARE", L"NIGHTMARENIGHTMARENIGHTMARE", MB_ICONEXCLAMATION);
				}
			}
			else {
				warn("Error reading process memory. Error: %ld", GetLastError());
			}
		}
	}
	else if (NT_ERROR(status)) {
		warn("'NtQueryInformationProcess' failed. Status code: 0x%x", status);
	}
	else {
		warn("Unexpected behaviour. Exiting...");
	}

	CloseHandle(hProcess);
	CloseHandle(hNtdll);

	return EXIT_SUCCESS;
}