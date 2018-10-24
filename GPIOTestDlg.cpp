// GPIOTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GPIOTest.h"
#include "GPIOTestDlg.h"

#include "GPIO.h"
#include "Ioctl.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGPIOTestDlg dialog



CGPIOTestDlg::CGPIOTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGPIOTestDlg::IDD, pParent)
	, m_bGP0Enable(FALSE)
	, m_bGP1Enable(FALSE)
	, m_bGP2_Enable(FALSE)
	, m_bGP3_Enable(FALSE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_hCOM = m_hDrv = INVALID_HANDLE_VALUE;

	DeleteFile(logName);
}

void CGPIOTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGPIOTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_OPENPORT, OnBnClickedBtnOpenport)
	ON_BN_CLICKED(IDC_BTN_CLOSEPORT, OnBnClickedBtnCloseport)
	ON_BN_CLICKED(IDC_BTN_SETSTATUS, OnBnClickedBtnSetstatus)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_GETSTATUS, OnBnClickedBtnGetstatus)
	ON_BN_CLICKED(IDC_CHK_GP0_ENABLE, OnBnClickedChkGp0Enable)
	ON_BN_CLICKED(IDC_BTN_GP0_GET, OnBnClickedBtnGp0Get)
	ON_BN_CLICKED(IDC_BTN_GP0_SET, OnBnClickedBtnGp0Set)
	ON_BN_CLICKED(IDC_CHK_GP1_ENABLE, OnBnClickedChkGp1Enable)
	ON_BN_CLICKED(IDC_BTN_GP1_GET, OnBnClickedBtnGp1Get)
	ON_BN_CLICKED(IDC_BTN_GP1_SET, OnBnClickedBtnGp1Set)
	
	ON_BN_CLICKED(IDC_CHK_GP2_ENABLE, &CGPIOTestDlg::OnBnClickedChkGp2Enable)
	ON_BN_CLICKED(IDC_CHK_GP3_ENABLE, &CGPIOTestDlg::OnBnClickedChkGp3Enable)
	ON_BN_CLICKED(IDC_BTN_GP2_GET, &CGPIOTestDlg::OnBnClickedBtnGp2Get)
	ON_BN_CLICKED(IDC_BTN_GP3_GET, &CGPIOTestDlg::OnBnClickedBtnGp3Get)
	ON_BN_CLICKED(IDC_BTN_GP2_SET, &CGPIOTestDlg::OnBnClickedBtnGp2Set)
	ON_BN_CLICKED(IDC_BTN_GP3_SET, &CGPIOTestDlg::OnBnClickedBtnGp3Set)
END_MESSAGE_MAP()


// CGPIOTestDlg message handlers

BOOL CGPIOTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	CheckWindowsVersion();

	CComboBox* cb = (CComboBox *)GetDlgItem(IDC_COMBO_COM);
	cb->SetCurSel(2);
	cb = (CComboBox *)GetDlgItem(IDC_COMBO_SIZE);
	cb->SetCurSel(3);
	cb = (CComboBox *)GetDlgItem(IDC_COMBO_STOPBIT);
	cb->SetCurSel(0);
	cb = (CComboBox *)GetDlgItem(IDC_COMBO_PARITY);
	cb->SetCurSel(0);
	cb = (CComboBox *)GetDlgItem(IDC_COMBO_BAUDRATE);
	cb->SetCurSel(17);		// 115200 bps
	
	SetDlgItemText(IDC_GP2_CFGVAL, "2");     

	QSI_MAIN();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGPIOTestDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
	ShowWindow(SW_HIDE);
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGPIOTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGPIOTestDlg::OnBnClickedBtnOpenport()
{
	// TODO: Add your control notification handler code here
	HANDLE hCOM;
/*
	if(m_hCOM != INVALID_HANDLE_VALUE)
		OnBnClickedBtnCloseport();
*/
	CString comStr, tmpStr;
	CComboBox* cb = (CComboBox *)GetDlgItem(IDC_COMBO_COM);
	int nIndex = cb->GetCurSel();
	if (nIndex != CB_ERR) {
		cb->GetLBText(nIndex, comStr);

		tmpStr = "\\\\.\\" + comStr;
		hCOM = ::CreateFile(tmpStr, GENERIC_READ | GENERIC_WRITE,
				0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

		CString strResponse = "OPEN " + comStr + " port ";
		if (hCOM == INVALID_HANDLE_VALUE)
			strResponse += "FAILED.";
		else {
			strResponse += "Successfully!";
			m_hCOM = hCOM;
			m_comStr = comStr;
		}
		AddResponseString(strResponse);

		if(m_hCOM != INVALID_HANDLE_VALUE)
			OnBnClickedBtnSetstatus();

		OpenSymbolicPort();
	}
}

void CGPIOTestDlg::OnBnClickedBtnCloseport()
{
	// TODO: Add your control notification handler code here
	CString str;

	if(m_hCOM != INVALID_HANDLE_VALUE) {
		::CloseHandle(m_hCOM);
		m_hCOM = INVALID_HANDLE_VALUE;

		str.Format("Close %s port Successfully!", m_comStr);
		AddResponseString(str);
	}

	if(m_hDrv != INVALID_HANDLE_VALUE) {
		if(!m_bWindows2000)
			::CloseHandle(m_hDrv);

		m_hDrv = INVALID_HANDLE_VALUE;
	}
}

void CGPIOTestDlg::OnBnClickedBtnSetstatus()
{
	// TODO: Add your control notification handler code here
	CString str;
	BOOL bSuccess;

	if(m_hCOM == INVALID_HANDLE_VALUE) {
		str = "Choose a COM port to set status.";
		AddResponseString(str);
		return;
	}

	bSuccess = ::GetCommState(m_hCOM, &m_DCB);
	if (!bSuccess) {
		str = "SET port status FAILED because cannot get status.";
		AddResponseString(str);
		return;
  }

	CComboBox* cb = (CComboBox *)GetDlgItem(IDC_COMBO_BAUDRATE);
	int nIndex = cb->GetCurSel();
	cb->GetLBText(nIndex, str);
	m_DCB.BaudRate = atol(str);

	cb = (CComboBox *)GetDlgItem(IDC_COMBO_SIZE);
	nIndex = cb->GetCurSel();
	cb->GetLBText(nIndex, str);
	m_DCB.ByteSize = atoi(str);

	cb = (CComboBox *)GetDlgItem(IDC_COMBO_STOPBIT);
	m_DCB.StopBits = cb->GetCurSel();

	cb = (CComboBox *)GetDlgItem(IDC_COMBO_PARITY);
	m_DCB.Parity = cb->GetCurSel();

	if((m_DCB.ByteSize == 6 || m_DCB.ByteSize == 7 || m_DCB.ByteSize == 8) && (m_DCB.StopBits == 1)) {
		str = "ERROR - CANNOT set such combination.";
		AddResponseString(str);
		return;
	}
	if((m_DCB.ByteSize == 5) && (m_DCB.StopBits == 2)) {
		str = "ERROR - CANNOT set such combination.";
		AddResponseString(str);
		return;
	}
	if(m_DCB.ByteSize == 5)
		m_DCB.StopBits++;

	bSuccess = ::SetCommState(m_hCOM, &m_DCB);
  if(!bSuccess) {
		str = "SET port status FAILED.";
		AddResponseString(str);
		return;
  }
	str = "SET port status Successfully!";
	AddResponseString(str);
}

void CGPIOTestDlg::OnBnClickedBtnGetstatus()
{
	// TODO: Add your control notification handler code here
	CString str;
  const char* parityStr[] = {"None","Odd","Even","Mark","Space"};
  const char* stopBitsStr[] = {"1", "1.5", "2"};

	if(m_hCOM == INVALID_HANDLE_VALUE) {
		str = "Choose a COM port to get status.";
		AddResponseString(str);
		return;
	}

	BOOL bSuccess = ::GetCommState(m_hCOM, &m_DCB);
	if (!bSuccess) {
		str = "GET port status FAILED.";
		AddResponseString(str);
    return;
  }

	if(m_DCB.ByteSize == 5 && m_DCB.StopBits > 0)
		m_DCB.StopBits--;

	str.Format("BaudRate: %ld", m_DCB.BaudRate);
	AddResponseString(str);

	str.Format("Parity: %s", parityStr[m_DCB.Parity]);
	AddResponseString(str);

	str.Format("ByteSize: %d", m_DCB.ByteSize);
	AddResponseString(str);

	str.Format("StopBits: %s", stopBitsStr[m_DCB.StopBits]);
	AddResponseString(str);

	str = "------------------------------------------";
	AddResponseString(str);
}

void CGPIOTestDlg::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: Add your message handler code here
	if(m_hCOM != INVALID_HANDLE_VALUE) {
		::CloseHandle(m_hCOM);
		m_hCOM = INVALID_HANDLE_VALUE;
	}
}

void CGPIOTestDlg::OnBnClickedChkGp0Enable()
{
	// TODO: Add your control notification handler code here
	CString str;

	if(m_hDrv == INVALID_HANDLE_VALUE) {
		str = "USB port is not opened.";
		AddResponseString(str);
		return;
	}

	m_bGP0Enable = m_bGP0Enable ? false : true;

	CButton *btn = (CButton *)GetDlgItem(IDC_CHK_GP0_ENABLE);
	btn->SetCheck(m_bGP0Enable);

	btn = (CButton *)GetDlgItem(IDC_BTN_GP0_SET);
	btn->EnableWindow(m_bGP0Enable);

	CEdit *ed = (CEdit *)GetDlgItem(IDC_EDIT_GP0_VAL);
	ed->EnableWindow(m_bGP0Enable);

	BOOL bSuccess = PL2303_GP0_Enalbe(m_hDrv, m_bGP0Enable);		// GP0 Output Enable
	if(!bSuccess) {
		str = "GP0 output enable FAILED.";
		AddResponseString(str);
	}
}

void CGPIOTestDlg::OnBnClickedBtnGp0Get()
{
	// TODO: Add your control notification handler code here
	CString str;
	BYTE val = 0;

	if(m_hDrv == INVALID_HANDLE_VALUE) {
		str = "USB port is not opened.";
		AddResponseString(str);
		return;
	}

	BOOL bSuccess = PL2303_GP0_GetValue(m_hDrv, &val);				// Get GP0 value
	if(bSuccess)
		str.Format("GP0 value: %d", val);
	else
		str = "GET GP0 value FAILED.";

	AddResponseString(str);
}

void CGPIOTestDlg::OnBnClickedBtnGp0Set()
{
	// TODO: Add your control notification handler code here
	CString str;

	CEdit* eb = (CEdit *)GetDlgItem(IDC_EDIT_GP0_VAL);
	eb->GetWindowText(str);	
	if(str.IsEmpty()) {
		str = "Please fill in 1 or 0.";
		AddResponseString(str);
		return;
	}

	BYTE val = (BYTE)atoi(str);

	BOOL bSuccess = PL2303_GP0_SetValue(m_hDrv, val);					// Set GP0 value
	if(bSuccess)
		str = "SET GP0 value Successfully!";
	else
		str = "SET GP0 value FAILED.";

	AddResponseString(str);
}

// For GP1
void CGPIOTestDlg::OnBnClickedChkGp1Enable()
{
	// TODO: Add your control notification handler code here
	CString str;

	if(m_hDrv == INVALID_HANDLE_VALUE) {
		str = "USB port is not opened.";
		AddResponseString(str);
		return;
	}

	m_bGP1Enable = m_bGP1Enable ? false : true;

	CButton *btn = (CButton *)GetDlgItem(IDC_CHK_GP1_ENABLE);
	btn->SetCheck(m_bGP1Enable);

	btn = (CButton *)GetDlgItem(IDC_BTN_GP1_SET);
	btn->EnableWindow(m_bGP1Enable);

	CEdit *ed = (CEdit *)GetDlgItem(IDC_EDIT_GP1_VAL);
	ed->EnableWindow(m_bGP1Enable);

	BOOL bSuccess = PL2303_GP1_Enalbe(m_hDrv, m_bGP1Enable);		// GP1 Output Enable
	if(!bSuccess) {
		str = "GP1 output enable FAILED.";
		AddResponseString(str);
	}
}

void CGPIOTestDlg::OnBnClickedBtnGp1Get()
{
	// TODO: Add your control notification handler code here
	CString str;
	BYTE val = 0;

	if(m_hDrv == INVALID_HANDLE_VALUE) {
		str = "USB port is not opened.";
		AddResponseString(str);
		return;
	}

	BOOL bSuccess = PL2303_GP1_GetValue(m_hDrv, &val);				// Get GP1 value
	if(bSuccess)
		str.Format("GP1 value: %d", val);
	else
		str = "GET GP1 value FAILED.";

	AddResponseString(str);
}

void CGPIOTestDlg::OnBnClickedBtnGp1Set()
{
	// TODO: Add your control notification handler code here
	CString str;

	CEdit* eb = (CEdit *)GetDlgItem(IDC_EDIT_GP1_VAL);
	eb->GetWindowText(str);	
	if(str.IsEmpty()) {
		str = "Please fill in 1 or 0.";
		AddResponseString(str);
		return;
	}

	BYTE val = (BYTE)atoi(str);

	BOOL bSuccess = PL2303_GP1_SetValue(m_hDrv, val);					// Set GP1 value
	if(bSuccess)
		str = "SET GP1 value Successfully!";
	else
		str = "SET GP1 value FAILED.";

	AddResponseString(str);
}




void CGPIOTestDlg::OnBnClickedChkGp2Enable()
{
	// TODO: Add your control notification handler code here
   CString str;
	
	if(m_hDrv == INVALID_HANDLE_VALUE) {
		str = "USB port is not opened.";
		AddResponseString(str);
		return;
	}

	m_bGP2_Enable = m_bGP2_Enable ? false : true;

	CButton *btn = (CButton *)GetDlgItem(IDC_CHK_GP2_ENABLE);
	btn->SetCheck(m_bGP2_Enable);

	btn = (CButton *)GetDlgItem(IDC_BTN_GP2_SET);
	btn->EnableWindow(m_bGP2_Enable);

	btn = (CButton *)GetDlgItem(IDC_BTN_GP2_GET);
//	btn->EnableWindow(!m_bGP2_Enable);

	CEdit *ed = (CEdit *)GetDlgItem(IDC_EDIT_GP2_VAL);
	ed->EnableWindow(m_bGP2_Enable);

    BOOL bSuccess = PL2303_GP2_Enalbe(m_hDrv, m_bGP2_Enable);		
	if(!bSuccess) {
		str = "GP2 output enable FAILED.";
		AddResponseString(str);
	}
}

void CGPIOTestDlg::OnBnClickedChkGp3Enable()
{
	// TODO: Add your control notification handler code here
	CString str;

	if(m_hDrv == INVALID_HANDLE_VALUE) {
		str = "USB port is not opened.";
		AddResponseString(str);
		return;
	}

	m_bGP3_Enable = m_bGP3_Enable ? false : true;

	CButton *btn = (CButton *)GetDlgItem(IDC_CHK_GP3_ENABLE);
	btn->SetCheck(m_bGP3_Enable);

	btn = (CButton *)GetDlgItem(IDC_BTN_GP3_SET);
	btn->EnableWindow(m_bGP3_Enable);

	btn = (CButton *)GetDlgItem(IDC_BTN_GP3_GET);
	//btn->EnableWindow(!m_bGP3_Enable);

	CEdit *ed = (CEdit *)GetDlgItem(IDC_EDIT_GP3_VAL);
	ed->EnableWindow(m_bGP3_Enable);
	
    BOOL bSuccess = PL2303_GP3_Enalbe(m_hDrv, m_bGP3_Enable);		
	if(!bSuccess) {
		str = "GP3 output enable FAILED.";
		AddResponseString(str);
	}
}

void CGPIOTestDlg::OnBnClickedBtnGp2Get()
{
	// TODO: Add your control notification handler code here
	   CString str;
	BYTE val = 0;

	if(m_hDrv == INVALID_HANDLE_VALUE) {
		str = "USB port is not opened.";
		AddResponseString(str);
		return;
	}
	/*
	if(m_bGP2_Enable) {
    	str = "No support when output-->enable.";
	    AddResponseString(str);
	    return;
	}
*/

	BOOL bSuccess = PL2303_GP2_GetValue(m_hDrv, &val);				
	if(bSuccess)
		str.Format("GP2 value: %d", val);
	else
		str = "GET GP2 value FAILED.";

	AddResponseString(str);
}

void CGPIOTestDlg::OnBnClickedBtnGp3Get()
{
	// TODO: Add your control notification handler code here
		   CString str;
	BYTE val = 0;

	if(m_hDrv == INVALID_HANDLE_VALUE) {
		str = "USB port is not opened.";
		AddResponseString(str);
		return;
	}
	/*
	if(m_bGP2_Enable) {
    	str = "No support when output-->enable.";
	    AddResponseString(str);
	    return;
	}
*/

	BOOL bSuccess = PL2303_GP3_GetValue(m_hDrv, &val);				
	if(bSuccess)
		str.Format("GP3 value: %d", val);
	else
		str = "GET GP3 value FAILED.";

	AddResponseString(str);


}

void CGPIOTestDlg::OnBnClickedBtnGp2Set()
{
	// TODO: Add your control notification handler code here
		CString str;

	CEdit* eb = (CEdit *)GetDlgItem(IDC_EDIT_GP2_VAL);
	eb->GetWindowText(str);	
	if(str.IsEmpty()) {
		str = "Please fill in 1 or 0.";
		AddResponseString(str);
		return;
	}

	BYTE val = (BYTE)atoi(str);

	BOOL bSuccess = PL2303_GP2_SetValue(m_hDrv, val);					
	if(bSuccess)
		str = "SET GP2 value Successfully!";
	else
		str = "SET GP2 value FAILED.";

	AddResponseString(str);
}

void CGPIOTestDlg::OnBnClickedBtnGp3Set()
{
	// TODO: Add your control notification handler code here
	CString str;

	CEdit* eb = (CEdit *)GetDlgItem(IDC_EDIT_GP3_VAL);
	eb->GetWindowText(str);	
	if(str.IsEmpty()) {
		str = "Please fill in 1 or 0.";
		AddResponseString(str);
		return;
	}

	BYTE val = (BYTE)atoi(str);

	BOOL bSuccess = PL2303_GP3_SetValue(m_hDrv, val);					
	if(bSuccess)
		str = "SET GP3 value Successfully!";
	else
		str = "SET GP3 value FAILED.";

	AddResponseString(str);
}
