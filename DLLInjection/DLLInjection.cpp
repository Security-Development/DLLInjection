#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>
#include <tchar.h>
#include <direct.h>

int main(void){
	
	char pName[64];
	char dName[64];
	char dPath[256] = {0,};
	char DLLPath[264];
	
	printf("예) ABC.dll \n"); 
	printf("DLL 인젝션의 DLL 이름을 적어주세요. : ");
	scanf("%s", dName); 
	
	sprintf(DLLPath, "%s\\%s", getcwd(dPath, 256), dName);
	
	printf("DLL Path : %s \n\n", DLLPath);
	
	printf("예) notepad.exe \n"); 
	printf("DLL 인젝션 할 프로세스 이름을 적어주세요. : ");
	scanf("%s", pName); 
	
	HANDLE hProcess, hMod, hThread;
	DWORD pid = 0;
	LPVOID DllBuf, hModAddr;
	PROCESSENTRY32 pe32 = {sizeof(PROCESSENTRY32)};
	pe32.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	
	
	if(Process32First(hSnapshot, &pe32)){
		do{
			if(!_tcsicmp(pe32.szExeFile, TEXT(pName)))
			{
				printf("%s를 찾았습니다. \n", pName);
				pid = pe32.th32ProcessID;
				printf("%s pid : %d \n", pName, pid);
				break;
			}
		}while (Process32Next(hSnapshot, &pe32));
			CloseHandle(hSnapshot);
	}
	
	if(!(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid)));
	if(!(DllBuf = VirtualAllocEx(hProcess, NULL, lstrlen(DLLPath) + 1, MEM_COMMIT, PAGE_READWRITE)));
	if(!(WriteProcessMemory(hProcess, DllBuf, (LPVOID)DLLPath, lstrlen((LPCTSTR)DLLPath) +1, NULL)));
	if(!(hMod = GetModuleHandle(TEXT("KERNEL32.DLL"))));
	if(!(hModAddr = (void(*))GetProcAddress((HMODULE)hMod, "LoadLibraryA")));
	if(!(hThread = CreateRemoteThread(hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)hModAddr, DllBuf, 0, NULL)));
	
	printf("핸들 값 :  0x%X \n", hProcess);
	printf("메모리 공간 : %x \n", DllBuf);
	printf("KERNEL32.DLL : %x \n", hMod);
			 
	WaitForSingleObject(hThread, INFINITE);
	printf("스레드 : %x \n", hThread);
			
	CloseHandle(hThread);
	CloseHandle(hProcess);
			
	system("PAUSE");
			
	return(FALSE);
}
