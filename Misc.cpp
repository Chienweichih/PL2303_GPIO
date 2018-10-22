#include "stdafx.h"
#include "GPIOTest.h"
#include "GPIOTestDlg.h"

void CGPIOTestDlg::AddResponseString(CString& str)
{
	CListBox *lb = (CListBox *)GetDlgItem(IDC_LIST_COMMAND_RESPONSE);
	int nIndex = lb->AddString(str);
	if (nIndex != LB_ERR || nIndex != LB_ERRSPACE)
		lb->SetCurSel(nIndex);
}

void CGPIOTestDlg::CheckWindowsVersion()
{
	DWORD dwVersion = GetVersion();
 
	// Get the Windows version.
	DWORD dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
	DWORD dwWindowsMinorVersion = (DWORD)(HIBYTE(LOWORD(dwVersion)));

	// Get the build number for Windows NT/Windows 2000 or Win32s.
	if(dwVersion < 0x80000000)									// Windows NT/2000
		m_bWindows2000 = TRUE;
	else if (dwWindowsMajorVersion < 4)					// Win32s
		m_bWindows2000 = FALSE;
	else																				// Windows 95/98 -- No build number
		m_bWindows2000 = FALSE;
}

void CGPIOTestDlg::OpenSymbolicPort()
{
	if(m_bWindows2000) {
		m_hDrv = m_hCOM;
	} else {
		m_hDrv = CreateFile("\\\\.\\USBCOM-1", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE,
				NULL, OPEN_EXISTING, 0, NULL);
	}
}