// UCRemoteConfig.h : UCRemoteConfig DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������



//

class CUCRemoteConfigApp : public CWinApp
{
public:
	CUCRemoteConfigApp();


public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};


#define CONFIG_API extern "C" __declspec(dllexport) 


CONFIG_API long CFG_UC_Init(char *pDeviceIP, int iPort, char *pUser, char *pPassword, char *pLan, LPVOID pParent);



CONFIG_API long CFG_UC_UnInit(long lConfigID);



CONFIG_API long CFG_UC_ShowModelessDlg(long lConfigID, RECT rcPos);

CONFIG_API long CFG_UC_ShowDlg(LPVOID pParent);
CONFIG_API long CFG_UC_CloseDlg();