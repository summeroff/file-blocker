// file_blocker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
 
#include <iostream>
#include <string>
#include <Windows.h>

bool use_extended_locking = false;

int main(int argc, char * argv[])
{
	HANDLE h_file = 0;	
	BOOL lock_success = false;
	std::cout << "Hello there! Gonna lock this file\n";

	if (argc >= 2)
	{
		std::string first_param(argv[1]);
		if (first_param.compare("-l") == 0)
		{
			use_extended_locking = true;
		}
	}
	
	if (use_extended_locking)
	{
		wchar_t app_path_buffer[MAX_PATH + 1];
		if (GetModuleFileName(NULL, app_path_buffer, MAX_PATH) == 0)
		{
			std::cout << "Error: Failed get file path\n";
			return;
		}

		std::wstring app_path_wstring = app_path_buffer;
		std::cout << "Run file: \"" << std::string(app_path_wstring.begin(), app_path_wstring.end()) << "\" (only block writing and deleting )\n";

		OVERLAPPED overlapvar = { 0 };
		
		std::cout << "Lock file: \"" << std::string(app_path_wstring.begin(), app_path_wstring.end()) << "\" (block reading too)\n";

		h_file = CreateFile(app_path_buffer, GENERIC_READ, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		if (h_file)
		{
			lock_success = LockFileEx(h_file, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY, 0, MAXDWORD, MAXDWORD, &overlapvar);
			if (!lock_success)
			{
				std::cout << "Error: Failed to lock file\n";
			}
		}
		else {
			std::cout << "Error: Failed to lock file\n";
		}
	}

	while (1)
	{
		Sleep(500);
		std::cout << "Still blocking file: \"" << argv[0] << "\"\n";
	}

	CloseHandle(h_file);
}
 