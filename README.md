# ntqip-remote-anti-debug
This is a small example of how the NT API can be used to check for debugger presence remotely. The code uses `NtQueryInformationProcess` function from `ntdll.dll` module to receive the PEB (process information block) of another process provided that the program has the remote process' PID.

This small project started when I took an interest in how `NtQueryInformationProcess` might be used for anti-debugging purposes. I realized there was a lack of practical examples regarding this topic so I decided to publish this here. Hope it helps!

`Disclaimer:` I advise you to check out the [official documentation](https://learn.microsoft.com/en-us/windows/win32/api/winternl/nf-winternl-ntqueryinformationprocess) (especially if you're going to use `PROCESS_BASIC_INFORMATION` and experiment before attempting to use the program. The structures and functions used in the code are subject to change. I might be working on this further to improve overally quality but the latest version was built for `Windows 11 Home 22621` with a `x64` processor.
