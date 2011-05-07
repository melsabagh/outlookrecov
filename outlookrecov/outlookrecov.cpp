// outlookrecov.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include <iostream>
#include <windows.h>
#include <wincrypt.h>

using namespace std;

int main()
{
    HKEY hKey = 0;
    const wchar_t* subkey = L"Software\\Microsoft\\Windows NT\\CurrentVersion\\Windows Messaging Subsystem\\Profiles\\Outlook\\9375CFF0413111d3B88A00104B2A6676\\00000004";

    if (RegOpenKey(HKEY_CURRENT_USER, subkey, &hKey) == ERROR_SUCCESS) {
        BYTE buf[255] = {0};
        DWORD dwType = REG_BINARY;
        DWORD dwBufSize = sizeof(buf);

        if (RegQueryValueEx(hKey, L"IMAP Password", 0, &dwType, buf, &dwBufSize) == ERROR_SUCCESS) {
        
            DATA_BLOB blobEncrypted;
            DATA_BLOB blobDecrypted;
            blobEncrypted.pbData = &buf[1]; 
            blobEncrypted.cbData = dwBufSize -1;

            if (CryptUnprotectData(&blobEncrypted, 0, 0, 0, 0, CRYPTPROTECT_UI_FORBIDDEN, &blobDecrypted)) {
                wcout << "Decrypted Password: " << (wchar_t*)blobDecrypted.pbData;
            } else {
                cout << "CryptUnprotectData error: " << GetLastError() << endl;
            }

            LocalFree(&blobEncrypted);
            LocalFree(&blobDecrypted);

        } else {
            cout << "RegQueryValueEx error:" << GetLastError() << endl;
        }

        RegCloseKey(hKey);

    } else {
        cout << "RegOpenKey error: " << GetLastError() << endl;
    }

    cin.ignore();

	return 0;
}
