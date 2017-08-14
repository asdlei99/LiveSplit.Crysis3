DWORD getProcessIdByName(char *name) {
  PROCESSENTRY32 entry = { .dwSize = sizeof(PROCESSENTRY32) };
  HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

  if (Process32First(snapshot, &entry)) {
    do {
      if (strcmp(entry.szExeFile, name) == 0) {
        CloseHandle(snapshot);
        return entry.th32ProcessID;
      }
    } while (Process32Next(snapshot, &entry));
  }

  CloseHandle(snapshot);

  return -1;
}

int getThreadCount(DWORD pid) {
  THREADENTRY32 entry = { .dwSize = sizeof(THREADENTRY32) };
  HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
  int count = 0;

  if (Thread32First(snapshot, &entry)) {
    do {
      if (entry.th32OwnerProcessID == pid) {
        count++;
      }
    } while(Thread32Next(snapshot, &entry));
  }

  CloseHandle(snapshot);

  return count;
}

int processHasModule(DWORD pid, char *name) {
  HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pid);

  if (snapshot != INVALID_HANDLE_VALUE) {
    MODULEENTRY32 entry = { .dwSize = sizeof(MODULEENTRY32) };

    if (Module32First(snapshot, &entry)) {
      do {
        if (strcmp(entry.szModule, name) == 0) {
          CloseHandle(snapshot);
          return 1;
        }
      } while (Module32Next(snapshot, &entry));
    }
  }

  CloseHandle(snapshot);
  return 0;
}

int injectDll(DWORD pid, char *dllPath) {
  if (getThreadCount(pid) < 20) {
    return 0;
  }

  HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
  if (!process) {
    CloseHandle(process);
    return 0;
  }

  char dll[MAX_PATH];

	if (!GetFullPathName(dllPath, MAX_PATH, dll, NULL)) {
    CloseHandle(process);
		return 0;
	}

	unsigned int dllLen = strlen(dll) + 1;

	LPVOID loadLibAddr = (LPVOID)GetProcAddress(GetModuleHandle("kernel32.dll"),
																					    "LoadLibraryA");

	if (!loadLibAddr) {
    CloseHandle(process);
		return 0;
	}

	LPVOID memoryAddr = (LPVOID)VirtualAllocEx(process, NULL, dllLen,
																						 MEM_RESERVE | MEM_COMMIT,
																						 PAGE_READWRITE);

	if (!memoryAddr) {
    CloseHandle(process);
		return 0;
	}

	if (!WriteProcessMemory(process, (LPVOID)memoryAddr, dll, dllLen,
                          NULL))
  {
    CloseHandle(process);
		return 0;
	}

	if (!CreateRemoteThread(process, NULL, 0,
                          (LPTHREAD_START_ROUTINE)loadLibAddr,
			                    (LPVOID)memoryAddr, 0, NULL))
	{
    CloseHandle(process);
		return 0;
	}

	CloseHandle(process);
	VirtualFreeEx(process, (LPVOID)memoryAddr, 0, MEM_RELEASE);

  printf("Dll injected\n");

  return 1;
}
