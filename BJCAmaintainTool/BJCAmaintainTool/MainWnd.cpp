#include "stdafx.h"
#include "MainWnd.h"
#include "Resource.h"
DUI_BEGIN_MESSAGE_MAP(CMainWnd,WindowImplBase)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_CLICK,OnClick)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_RETURN,OnReturn)
	DUI_ON_MSGTYPE(DUI_MSGTYPE_SELECTCHANGED,OnSelectChanged)
DUI_END_MESSAGE_MAP()
CMainWnd::CMainWnd(void)
{
	
}
CMainWnd::~CMainWnd()
{

}

void CMainWnd::InitWindow()
{
	m_pCloseBtn = static_cast<CButtonUI*>(m_pm.FindControl(_T("closebtn")));
	AddTrayIcon();
}

void CMainWnd::OnFinalMessage( HWND hWnd )
{

}

CDuiString CMainWnd::GetSkinFile()
{
	return _T("XML_MAIN");
}

LPCTSTR CMainWnd::GetWindowClassName()const
{
	return _T("MainWnd");
}

void CMainWnd::OnClick(TNotifyUI&msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
	if(msg.pSender == m_pCloseBtn){
		//::PostQuitMessage(0L);
		ShowWindow(SW_HIDE);
		return ;
	}
}

void CMainWnd::OnSelectChanged(TNotifyUI&msg)
{

}

void CMainWnd::OnReturn(TNotifyUI&msg)
{
	CDuiString sName = msg.pSender->GetName();
	sName.MakeLower();
}

HRESULT CMainWnd::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if (uMsg == WM_DESTROY){
		::PostQuitMessage(0L);
		bHandled = TRUE;
	}else if (uMsg == WM_SHOWTASK){
		OnTrayIcon(uMsg,wParam,lParam, bHandled);
	}
	return 0;
}

HRESULT CMainWnd::OnTrayIcon(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	bHandled = FALSE;
	if (lParam == WM_LBUTTONDOWN){
		ShowWindow(SW_SHOWNORMAL);
		bHandled = TRUE;
	}else if (lParam == WM_RBUTTONDOWN){
		POINT pt;
		GetCursorPos(&pt);
		SetForegroundWindow(m_hWnd);
		HMENU hMenu = CreatePopupMenu();
		AppendMenu(hMenu,MF_STRING,WM_ONOPEN,_T("打开"));
		AppendMenu(hMenu,MF_STRING,WM_ONCLOSE,_T("退出"));
		int cmd = TrackPopupMenu(hMenu,TPM_RETURNCMD,pt.x,pt.y,NULL,m_hWnd,NULL);
		if(cmd == WM_ONCLOSE){
			m_trayIcon.hIcon = NULL;
			Shell_NotifyIcon(NIM_DELETE,&m_trayIcon);
			::PostQuitMessage(0L);
		}else if (cmd == WM_ONOPEN){
			ShowWindow(SW_SHOWNORMAL);
		}
		bHandled = TRUE;
	}
	return 0;
}

void CMainWnd::AddTrayIcon()
{
	memset(&m_trayIcon,0,sizeof(NOTIFYICONDATA));
	m_trayIcon.cbSize = sizeof(NOTIFYICONDATA);
	m_trayIcon.hIcon = ::LoadIcon(GetModuleHandle(NULL),MAKEINTRESOURCE(IDI_TRAYICON));
	m_trayIcon.hWnd = m_hWnd;
	lstrcpy(m_trayIcon.szTip,_T("管理工具"));
	m_trayIcon.uCallbackMessage = WM_SHOWTASK;
	m_trayIcon.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP;
	Shell_NotifyIcon(NIM_ADD,&m_trayIcon);
}