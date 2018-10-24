#include "stdafx.h"
#include "GPIOTest.h"
#include "GPIOTestDlg.h"

void CGPIOTestDlg::AddResponseString(CString& str)
{
#if 0
	CListBox *lb = (CListBox *)GetDlgItem(IDC_LIST_COMMAND_RESPONSE);
	int nIndex = lb->AddString(str);
	if (nIndex != LB_ERR || nIndex != LB_ERRSPACE)
		lb->SetCurSel(nIndex);
#else
	OutputLog(str);
#endif
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

void CGPIOTestDlg::OutputLog(CString msg)
{
	try
	{
		CStdioFile outFile(logName, CFile::modeNoTruncate | CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		outFile.SeekToEnd();
		outFile.WriteString(msg + '\n');
		outFile.Close();
	}
	catch (CFileException *fx)
	{
		fx->Delete();
	}

	// Not (Not found these msg)
	if (!((-1 == msg.Find("USB port is not opened.")) && (-1 == msg.Find("FAILED"))))
	{
		m_COM_Status = FALSE;
	}
}