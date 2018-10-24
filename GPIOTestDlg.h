// GPIOTestDlg.h : header file
//

#pragma once


// CGPIOTestDlg dialog
class CGPIOTestDlg : public CDialog
{
	BOOL m_bWindows2000;
	HANDLE m_hCOM, m_hDrv;
	DCB m_DCB;
	BOOL m_bGP0Enable, m_bGP1Enable;
	BOOL m_bGP2_Enable, m_bGP3_Enable;
	CString m_comStr;

	BOOL m_COM_Status;
	const CString logName = CTime::GetCurrentTime().Format("QSI_Type_C_Switch_%Y_%B_%d_%H_%M_%S.log");

// Construction
public:
	CGPIOTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_GPIOTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnOpenport();
	afx_msg void OnBnClickedBtnCloseport();
	afx_msg void OnBnClickedBtnSetstatus();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnGetstatus();
	afx_msg void OnBnClickedChkGp0Enable();
	afx_msg void OnBnClickedBtnGp0Get();
	afx_msg void OnBnClickedBtnGp0Set();
	afx_msg void OnBnClickedChkGp1Enable();
	afx_msg void OnBnClickedBtnGp1Get();
	afx_msg void OnBnClickedBtnGp1Set();

public:
	void AddResponseString(CString& str);
	void CheckWindowsVersion();
	void OpenSymbolicPort();
	void OutputLog(CString);
	void QSI_MAIN();

	afx_msg void OnBnClickedChkGp2Enable();
	afx_msg void OnBnClickedChkGp3Enable();
	afx_msg void OnBnClickedBtnGp2Get();
	afx_msg void OnBnClickedBtnGp3Get();
	afx_msg void OnBnClickedBtnGp2Set();
	afx_msg void OnBnClickedBtnGp3Set();
};
