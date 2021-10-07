#include <iostream>
#include <set>
#include <iterator>
#include <fstream>
#include <future>

#ifdef __cplusplus
extern "C" {
#endif

#include "tokenWatcher.h"

#ifdef __cplusplus
}
#endif

#define NAME_OUT_FILE "rtids.txt"
using namespace std;

vector<long> vID;

BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{
	ofstream out_file(NAME_OUT_FILE);
	ostream_iterator<long> out_stream(out_file, "\n");
	std::copy(vID.begin(), vID.end(), out_stream);

	return FALSE;
}

int main()
{

	if (!init_pkcs11(PKCS11_LIBRARY_NAME))
	{
		setlocale(LC_ALL, "Russian");
		system("cls");
		SetConsoleCtrlHandler(CtrlHandler, TRUE);
		HANDLE hStdout;
		COORD coordScreen = { 0, 0 };
		hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

		CONSOLE_FONT_INFOEX fontInfo;
		fontInfo.cbSize = sizeof(fontInfo);
		GetCurrentConsoleFontEx(hStdout, TRUE, &fontInfo);
		wcscpy_s(fontInfo.FaceName, L"Lucida Console");
		fontInfo.dwFontSize.X = 14;
		fontInfo.dwFontSize.Y = 24;
		SetCurrentConsoleFontEx(hStdout, TRUE, &fontInfo);

		long serialNumber = -1;
		cout << "\033[1;32m" << "\rКоличество токенов " << vID.size() << "\033[0m" << std::flush;
		while (1)
		{
			serialNumber = monitorSlotEvent();
			SetConsoleCursorPosition(hStdout, coordScreen);
			if (serialNumber > 0)
			{
				if (find(vID.begin(), vID.end(), serialNumber) == vID.end())
				{
					vID.push_back(serialNumber);
					cout << "\033[1;32m" << "Количество токенов " << vID.size() << "\033[0m \n";
					copy(vID.begin(), vID.end(), ostream_iterator<long>(cout, "\n"));
				}
				else
				{
					cout << "\033[1;31m" << "Количество токенов " << vID.size() << "\033[0m \n";
				}
			}
			else
			{
				cout << "\033[1;33m" << "Количество токенов " << vID.size() << "\033[0m \n";
			}

			cout << std::flush;
		}
		free_pkcs11();
	}
	std::system("pause");
	return 0;
}