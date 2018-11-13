#include "stdafx.h"

#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <vector>

std::string exec(const char* cmd) {
	std::array<char, 128> buffer;
	std::string result;
	std::shared_ptr<FILE> pipe(_popen(cmd, "r"), _pclose);
	if (!pipe) throw std::runtime_error("popen() failed!");
	while (!feof(pipe.get())) {
		if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
			result += buffer.data();
	}
	return result;
}

std::vector<CString> GetDisks(std::vector<CString> excepts)
{
	CString output(exec("wmic diskdrive get caption").c_str());

	int nTokenPos = 0;
	CString strToken = output.Tokenize(_T("\n"), nTokenPos);
	std::vector<CString> disks;

	if (!strToken.IsEmpty())
	{
		// skip first line
		strToken = output.Tokenize(_T("\n"), nTokenPos);
	}

	while (!strToken.IsEmpty())
	{
		if (strToken.GetLength() == 1)
		{
			break;
		}
		strToken.Trim();

		BOOL isExcept = FALSE;
		for (std::vector<CString>::iterator iter = excepts.begin(); iter != excepts.end(); ++iter)
		{
			if (-1 != strToken.Find(*iter))
			{
				isExcept = TRUE;
				break;
			}
		}

		if (isExcept == FALSE)
		{
			disks.push_back(strToken);
		}

		strToken = output.Tokenize(_T("\n"), nTokenPos);
	}

	return disks;
}

int GetDisksNum(int exceptsNum)
{
	CString output(exec("wmic diskdrive get caption").c_str());

	int disksCnt = 0;
	int nTokenPos = 0;
	CString strToken = output.Tokenize(_T("\n"), nTokenPos);

	if (!strToken.IsEmpty())
	{
		// skip first line
		strToken = output.Tokenize(_T("\n"), nTokenPos);
	}

	while (!strToken.IsEmpty())
	{
		if (strToken.GetLength() == 1)
		{
			break;
		}
		++disksCnt;

		strToken = output.Tokenize(_T("\n"), nTokenPos);
	}

	return (disksCnt - exceptsNum);
}