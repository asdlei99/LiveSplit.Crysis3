#include "stdafx.h"

// #define DEBUG

int(__thiscall *ExecuteCommandOriginal)(DWORD **, char *, char, char);
int(__thiscall *ShowWaitOriginal)(int, int, int, int);
int(__thiscall *ShowMenuOriginal)(int, int);
int(__thiscall *FrameFuncOriginal)(void *this_);
void(*StartOriginal)();
int(__thiscall *EndOriginal)(int this_, int a2);

bool loading = false, level_load = false;
char last[0xFF] = { 0 };
int level = -1;

void SendThread(char *msg) {
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
	free(msg);
}

void Send(char *msg) {
	printf("%s\n", msg);
	CreateThread(0, 0, (LPTHREAD_START_ROUTINE)SendThread, _strdup(msg), 0, 0);
}

void Loading(bool a) {
	loading = a;
	Send(loading ? "pause\n" : "resume\n");
}

int __fastcall ExecuteCommandHook(DWORD **this_, void *idle_, char *a2, char a3, char a4) {
	if (!loading && (memcmp(a2, "load", 4) == 0 || memcmp(a2, "disconnect", 10) == 0 || memcmp(a2, "map ", 4) == 0)) {
		if (memcmp(a2, "map ", 4) == 0) {
			level_load = true;
			int new_level = -1;
			if (StrStrIA(a2, "map Fields nb")) {
				new_level = 1;
			} else if (StrStrIA(a2, "map Canyon nb")) {
				new_level = 2;
			} else if (StrStrIA(a2, "map Swamp nb")) {
				new_level = 3;
			} else if (StrStrIA(a2, "map River nb")) {
				new_level = 4;
			} else if (StrStrIA(a2, "map Islands nb")) {
				new_level = 5;
			} else if (StrStrIA(a2, "map Cave nb")) {
				new_level = 6;
			} else if (StrStrIA(a2, "map Endgame nb")) {
				new_level = 7;
			}
			
			if (level + 1 == new_level) {
				if (new_level != 7) Send("split\n");
				level = new_level;
			}

			printf("level load\n");
		} else {
			level_load = false;
		}

		strcpy(last, a2);
		Loading(true);
	}

	printf("\t%s\n", a2);

	return ExecuteCommandOriginal(this_, a2, a3, a4);
}

int __fastcall ShowWaitHook(int this_, void *idle_, int a2, int a3, int a4) {
	if (loading && (a2 == 30 || a2 == 41)) {
		Loading(false);
		if (level_load) level_load = false;
	}
	return ShowWaitOriginal(this_, a2, a3, a4);
}

int __fastcall ShowMenuHook(int this_, void *idle_, int a2) {
	bool isMenu = (!*(BYTE *)(this_ + 3033) || *(BYTE *)(this_ + 3032) == 2);
	
	int ret = ShowMenuOriginal(this_, a2);

	if (isMenu && loading && !level_load) {
		Loading(false);
		if (level_load) level_load = false;
	}

	return ret;
}

int __fastcall FrameFuncHook(void *this_, void *idle_) {
	if (loading && !level_load) {
		Loading(false);
	}

	return FrameFuncOriginal(this_);
}

__declspec(naked) void StartHook() {
	__asm {
		push ecx
		push eax
		push esp
	}

	if (!loading && StrStrIA(last, "map Jailbreak nb")) {
		Send("reset\nstart\n");
		*last = level = 0;
	}

	__asm {
		pop esp
		pop eax
		pop ecx
		jmp StartOriginal
	}
}

char __fastcall EndHook(int this_, void *idle_, int a2) {
	if (level == 7 && *(DWORD *)a2 == 0x71536F4E) {
		Send("split\n");
		level = -1;
	}

	return EndOriginal(this_, a2);
}

void MainThread() {
#ifdef DEBUG
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif

	WSADATA wsadata;
	WSAStartup(MAKEWORD(2, 2), &wsadata);

	TrampolineHook(ExecuteCommandHook, (void *)0x37CE1B00, (void **)&ExecuteCommandOriginal);
	TrampolineHook(ShowWaitHook, (void *)0x37D411E0, (void **)&ShowWaitOriginal);
	TrampolineHook(ShowMenuHook, (void *)0x37E745B0, (void **)&ShowMenuOriginal);
	TrampolineHook(FrameFuncHook, (void *)0x3749A580, (void **)&FrameFuncOriginal);
	TrampolineHook(StartHook, (void *)0x37158AC0, (void **)&StartOriginal);
	TrampolineHook(EndHook, (void *)0x37D39D50, (void **)&EndOriginal);
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID reserved) {
	if (reason == DLL_PROCESS_ATTACH) {
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)MainThread, 0, 0, 0);
	}

	return TRUE;
}