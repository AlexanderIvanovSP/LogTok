#include <iostream>
#include <set>
#include <iterator>
#include <fstream>
#include <future>
#include <string>
#include <chrono>
#include <ctime>

#ifdef __cplusplus
extern "C" {
#endif

#include "tokenWatcher.h"

#ifdef __cplusplus
}
#endif

#define NAME_OUT_FILE "rtids_"
using namespace std;

vector<string> vID;
/// <summary>
/// Функция записывает лог файл при закрытиии приложения
/// </summary>
/// <param name="fdwCtrlType"> - </param>
/// <returns> - </returns>
BOOL WINAPI CtrlHandler(DWORD fdwCtrlType)
{

	struct tm newtime;
	__time64_t long_time;
	char out[sizeof("2011-10-08T07:07:09Z")] = { 0 };

	_time64(&long_time);
	_localtime64_s(&newtime, &long_time);
	strftime(out, sizeof("2011-10-08T07:07:09Z"), "%FT%TZ", &newtime);
	std::string path(out);
	path.replace(path.find(":"), 1, "-");
	path.replace(path.find(":"), 1, "-");
	path = NAME_OUT_FILE + path + ".txt";

	ofstream out_file(path);
	ostream_iterator<string> out_stream(out_file, "\n");
	std::copy(vID.begin(), vID.end(), out_stream);

	return FALSE;
}
/// <summary>
/// Главная функция приложения
/// </summary>
/// <returns> - </returns>
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

				string sID = to_string(serialNumber);
				unsigned int number_of_zeros = 10 - (unsigned int)sID.length();
				sID.insert(0, number_of_zeros, '0');
				if (find(vID.begin(), vID.end(), sID) == vID.end())
				{
					vID.push_back(sID);
					cout << "\033[1;32m" << "Количество токенов " << vID.size() << "\033[0m \n";
					copy(vID.begin(), vID.end(), ostream_iterator<string>(cout, "\n"));
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