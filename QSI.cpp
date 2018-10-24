#include "stdafx.h"
#include "GPIOTest.h"
#include "GPIOTestDlg.h"

void CGPIOTestDlg::QSI_MAIN()
{
	CString str;
	for (int i = 0; i < __argc; ++i)
	{
		str.Format(_T("[%d] %s"), i, __argv[i]);
		OutputLog(str);
	}

	EndDialog(IDCANCEL);
}