#include "stdafx.h"
#include "GPIOTest.h"
#include "GPIOTestDlg.h"

void CGPIOTestDlg::QSI_MAIN()
{
	const CString str_high = "1";
	const CString str_low = "0";
	const CString str_hr = "=============================================";

	CString str;
	int comPort = -1;
	for (int i = 0; i < __argc; ++i)
	{
		str.Format(_T("(%d) %s"), i, __argv[i]);
		OutputLog(str);

		if (i == 1)
		{
			comPort = atoi(__argv[i]) - 1;
		}
	}

	for (int i = 1; i <= 28; ++i)
	{
		if ((i > 4) && (m_COM_Status == FALSE))
		{
			if ((comPort + 1) > 0)
			{
				str.Format(_T("COM%d IS WRONG PORT OR DISCONNECTED"), (comPort + 1));
			}
			else
			{
				str.Format(_T("WRONG COM PORT"));
			}
			AfxMessageBox(str, MB_ICONSTOP);
			break;
		}
		str.Format(_T("[%02d] "), i);
		switch (i)
		{
		case 1:
			OutputLog("[Pahse A] USB3x Switch Test Fixture Connection " + str_hr);
			str += _T("Test Fixture DC Power Jack connected a 12V power adapter");
			break;
		case 2:
			str += _T("Test Fixture USB MicroB connects to Host");
			break;
		case 3:
			str += _T("Test Fixture USB Type-C UFP connects to Host.");
			break;
		case 4:
			OutputLog("[Pahse B] USB3x switch Test Fixture Initialization " + str_hr);

			((CComboBox *)GetDlgItem(IDC_COMBO_COM))->SetCurSel(comPort);
			OnBnClickedBtnOpenport();
			if (m_hCOM == INVALID_HANDLE_VALUE)
			{
				if ((comPort + 1) > 0)
				{
					str.Format(_T("COM%d IS WRONG PORT OR DISCONNECTED"), (comPort + 1));
				}
				else
				{
					str.Format(_T("WRONG COM PORT"));
				}
				AfxMessageBox(str, MB_ICONSTOP);
				EndDialog(IDCANCEL);
				return;
			}
			m_COM_Status = TRUE;

			OnBnClickedChkGp1Enable();
			OnBnClickedChkGp2Enable();
			OnBnClickedChkGp3Enable();

			OnBnClickedBtnGp0Get();
			OnBnClickedBtnGp1Get();
			OnBnClickedBtnGp2Get();
			OnBnClickedBtnGp3Get();
			str += _T("Set GPIO1 to \"Output\" and  \"Low\". Disconnect CC2 to DUT.");
			break;
		case 5:
			str += _T("Set GPIO2 to \"Output\" and \"Low\". Disconnect CC1 to DUT.");
			break;
		case 6:
			str += _T("Set GPIO3 to \"Output\" and \"High\". Cut off 5VBUS to DUT.");
			break;
		case 7:case 17:case 26:
			str += _T("Read GPIO0 and get \"High\". It means DUT disconnected.");
			break;
		case 8:
			OutputLog("[Pahse C] USB3x Channel 1 Connection & DUT Plug-In " + str_hr);
			str += _T("Test Fixture USB Type-C DFP connects to DUT, Cable Plug-In DUT.");
			break;
		case 9:
			str += _T("Set GPIO2 to \"High\". Connect CC1 to DUT.");
			break;
		case 10:case 19:
			str += _T("Read GPIO0 and get \"Low\". It means DUT connected.");
			break;
		case 11:case 20:
			str += _T("Set GPIO3 to \"Low\". Deliver 5VBUS to DUT.");
			break;
		case 12:case 21:
			str += _T("Wait USB link complete.");
			break;
        case 13:case 22:
            str += _T("Run USB testing");
            break;
        case 14:
            OutputLog("[Pahse D] USB3x Channel 1 Disconnection (Estimate DUT disconnected) " + str_hr);
            str += _T("Set GPIO1 to \"Low\". Disconnect CC2 to DUT.");
            break;
		case 15:case 24:
            str += _T("Set GPIO2 to \"Low\". Disconnect CC1 to DUT.");
            break;
		case 16:case 25:
            str += _T("Set GPIO3 to \"High\". Cut off 5VBUS to DUT.");
            break;
        case 18:
            OutputLog("[Pahse E] USB3x Channel 2 Connection " + str_hr);
            str += _T("Set GPIO1 to \"High\". Connect CC2 to DUT.");
            break;
        case 23:
            OutputLog("[Pahse F] USB3x Channel 2 Disconnection & DUT Plug-Out " + str_hr);
            str += _T("Set GPIO1 to \"Low\". Disconnect CC2 to DUT.");
            break;
        case 27:
            str += _T("Test Fixture USB Type-C DFP disconnects to DUT, Cable Plug-Out DUT.");
            break;
        case 28:
            str += _T("Go to Phase C \"Connect USB3x Channel 1\"");
            break;
		}
		OutputLog(str);
		switch (i)
		{
		case 7:case 10:case 17:case 19:case 26:
			// Read GPIO 0
			OnBnClickedBtnGp0Get();
			break;
		case 4:case 14:case 23:
			// Set GPIO 1 to "LOW"
			((CEdit *)GetDlgItem(IDC_EDIT_GP1_VAL))->SetWindowText(str_low);
			OnBnClickedBtnGp1Set();
			OnBnClickedBtnGp1Get();
			break;
		case 5:case 15:case 24:
			// Set GPIO 2 to "LOW"
			((CEdit *)GetDlgItem(IDC_EDIT_GP2_VAL))->SetWindowText(str_low);
			OnBnClickedBtnGp2Set();
			OnBnClickedBtnGp2Get();
			break;
		case 11:case 20:
			// Set GPIO 3 to "LOW"
			((CEdit *)GetDlgItem(IDC_EDIT_GP3_VAL))->SetWindowText(str_low);
			OnBnClickedBtnGp3Set();
			OnBnClickedBtnGp3Get();
			break;
		case 18:
			// Set GPIO 1 to "HIGH"
			((CEdit *)GetDlgItem(IDC_EDIT_GP1_VAL))->SetWindowText(str_high);
			OnBnClickedBtnGp1Set();
			OnBnClickedBtnGp1Get();
			break;
		case 9:
			// Set GPIO 2 to "HIGH"
			((CEdit *)GetDlgItem(IDC_EDIT_GP2_VAL))->SetWindowText(str_high);
			OnBnClickedBtnGp2Set();
			OnBnClickedBtnGp2Get();
			break;
		case 6:case 16:case 25:
			// Set GPIO 3 to "HIGH"
			((CEdit *)GetDlgItem(IDC_EDIT_GP3_VAL))->SetWindowText(str_high);
			OnBnClickedBtnGp3Set();
			OnBnClickedBtnGp3Get();
			break;
		}
		if (i == 28)
		{
			if (IDYES == AfxMessageBox(str, MB_YESNO | MB_ICONQUESTION))
			{
				i = 8 - 1; // continue step 8
			}
			// NO, end test flow
		}
		else
		{
			if (IDCANCEL == AfxMessageBox(str, MB_OKCANCEL | MB_ICONINFORMATION))
			{
				break;
			}
		}
	}

	OnBnClickedBtnCloseport();

	EndDialog(IDCANCEL);
}