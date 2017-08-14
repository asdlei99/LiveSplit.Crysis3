#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>

#include "utils.c"

int main() {
  DWORD pid = getProcessIdByName("Crysis3.exe");
  if (!processHasModule(pid, "c3nl.dll") && getThreadCount(pid) > 50) {
    injectDll(getProcessIdByName("Crysis3.exe"), "c3nl.dll");
  }
}
