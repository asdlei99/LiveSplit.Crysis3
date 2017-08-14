#include "stdafx.h"

int(__thiscall *ExecuteCommandOriginal)(DWORD **, char *, char, char);
int(__thiscall *ShowWaitOriginal)(int, int, int, int);
int(__thiscall *ShowMenuOriginal)(int, int);
int(__thiscall *FrameFuncOriginal)(void *this_);

bool loading = false;

void Send(char *msg) {
	SOCKADDR_IN target;
	target.sin_family = AF_INET;
	target.sin_port = htons(41573);
	target.sin_addr.s_addr = inet_addr("127.0.0.1");

	SOCKET s;

	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		return;
	}

	if (connect(s, (SOCKADDR *)&target, sizeof(SOCKADDR_IN)) == SOCKET_ERROR) {
		return;
	}

	send(s, msg, strlen(msg) + 1, 0);

	closesocket(s);
}

void Loading(bool a) {
	printf(a ? "loading\n" : "done loading\n");
	loading = a;
	Send(loading ? "pause\n" : "resume\n");
}

int __fastcall ExecuteCommandHook(DWORD **this_, void *idle_, char *a2, char a3, char a4) {
	if (!loading && (memcmp(a2, "loadLastSave", 12) == 0 || memcmp(a2, "disconnect", 10) == 0 || memcmp(a2, "map ", 4) == 0)) {
		Loading(true);
	}

	return ExecuteCommandOriginal(this_, a2, a3, a4);
}

int __fastcall ShowWaitHook(int this_, void *idle_, int a2, int a3, int a4) {
	if (loading && (a2 == 30 || a2 == 41)) {
		Loading(false);
	}
	return ShowWaitOriginal(this_, a2, a3, a4);
}

int __fastcall ShowMenuHook(int this_, void *idle_, int a2) {
	bool isMenu = (!*(BYTE *)(this_ + 3033) || *(BYTE *)(this_ + 3032) == 2);
	
	int ret = ShowMenuOriginal(this_, a2);

	if (isMenu && loading) {
		Loading(false);
	}

	return ret;
}

int __fastcall FrameFuncHook(void *this_, void *idle_) {
	if (loading) {
		Loading(false);
	}
	return FrameFuncOriginal(this_);
}

void MainThread() {
	/* AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr); */

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	TrampolineHook(ExecuteCommandHook, (void *)0x37CE1B00, (void **)&ExecuteCommandOriginal);
	TrampolineHook(ShowWaitHook, (void *)0x37D411E0, (void **)&ShowWaitOriginal);
	TrampolineHook(ShowMenuHook, (void *)0x37E745B0, (void **)&ShowMenuOriginal);
	TrampolineHook(FrameFuncHook, (void *)0x3749A580, (void **)&FrameFuncOriginal);
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, 0, 0, 0);
	}

	return TRUE;
}