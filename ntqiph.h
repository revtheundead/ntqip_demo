#pragma once

#include <windows.h>
#include <winternl.h>
#include <stdio.h>

#define okay(msg, ...) printf("[+] " msg "\n", ##__VA_ARGS__)
#define info(msg, ...) printf("[*] " msg "\n", ##__VA_ARGS__)
#define warn(msg, ...) printf("[-] " msg "\n", ##__VA_ARGS__)

#ifndef NT_SUCCESS
#define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#endif

#ifndef NT_ERROR
#define NT_ERROR(Status) ((NTSTATUS)(Status) >> 30 == 3)
#endif
