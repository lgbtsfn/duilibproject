#include <ShellAPI.h>

#define WM_SHOWTASK WM_USER+1001
#define WM_ONOPEN  WM_USER+1002
#define WM_ONCLOSE  WM_USER+1003

class CMainWnd:public WindowImplBase
{
public:
	CMainWnd(void);
	~CMainWnd();
public:
	virtual void InitWindow();
	virtual void OnFinalMessage( HWND hWnd );
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void)const;
	DUI_DECLARE_MESSAGE_MAP()
	virtual void OnClick(TNotifyUI&);
	virtual void OnSelectChanged(TNotifyUI&);
	virtual void OnReturn(TNotifyUI&);

	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	LRESULT OnTrayIcon(UINT uMsg,WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	void AddTrayIcon();
private:
	CButtonUI *m_pCloseBtn;
	NOTIFYICONDATA m_trayIcon;
};