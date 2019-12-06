// BJCAmaintainTool.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "BJCAmaintainTool.h"
#include "MainWnd.h"
void InitResource();

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	HANDLE hMutex = CreateMutex(NULL,FALSE,_T("BJCAmaintainTool"));
	if(GetLastError() == ERROR_ALREADY_EXISTS){
		CloseHandle(hMutex);
		return 0;
	}
	HRESULT Hr = ::CoInitialize(NULL);
	if(FAILED(Hr)) return 0;
	HRESULT hRes = ::OleInitialize(NULL);
	CPaintManagerUI::SetInstance(hInstance);
	InitResource();

	::SetCurrentDirectory(CPaintManagerUI::GetInstancePath());
	CMainWnd *pFrame = new CMainWnd();
	if(pFrame == NULL)return 0;
	pFrame->Create(NULL,_T("BJCAmaintainTool"),UI_WNDSTYLE_FRAME,0L,0,0,990,690);
	pFrame->CenterWindow();
	pFrame->ShowModal();
	CPaintManagerUI::MessageLoop();
	OleUninitialize();
	::CoUninitialize;
	CloseHandle(hMutex);
	return 0;
}

void InitResource()
{
#ifdef _DEBUG
	CPaintManagerUI::SetResourceType(UILIB_FILE);
#else
	CPaintManagerUI::SetResourceType(UILIB_ZIPRESOURCE);
#endif
	CDuiString strResourcePath = CPaintManagerUI::GetInstancePath();
	switch (CPaintManagerUI::GetResourceType())
	{
	case UILIB_FILE:
		{
			strResourcePath += _T("Skin\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
			CResourceManager::GetInstance()->LoadResource(_T("res.xml"),NULL);
			break;
		}
	case UILIB_RESOURCE:
		{
			strResourcePath += _T("Skin\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
			CResourceManager::GetInstance()->LoadResource(_T("IDR_RES"),_T("xml"));
			break;
		}
	case UILIB_ZIP:
		{
			strResourcePath += _T("Skin\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
			CPaintManagerUI::SetResourceZip(_T("BJCAmaintainTool.zip"),true);
			CResourceManager::GetInstance()->LoadResource(_T("res.xml"),NULL);
			break;
		}
	case UILIB_ZIPRESOURCE:
		{
			strResourcePath += _T("Skin\\");
			CPaintManagerUI::SetResourcePath(strResourcePath.GetData());
			HRSRC hResource = ::FindResource(CPaintManagerUI::GetResourceDll(),_T("IDR_ZIPRES"),_T("ZIPRES"));
			if(hResource!=NULL){
				DWORD dwSize = 0;
				HGLOBAL hGlobal = ::LoadResource(CPaintManagerUI::GetResourceDll(),hResource);
				if (hGlobal!=NULL){
					dwSize = ::SizeofResource(CPaintManagerUI::GetResourceDll(),hResource);
					if (dwSize>0){
						CPaintManagerUI::SetResourceZip((LPBYTE)::LockResource(hGlobal),dwSize);
						CResourceManager::GetInstance()->LoadResource(_T("res.xml"),NULL);
					}
				}
				::FreeResource(hResource);
			}
			break;
		}
	}
}