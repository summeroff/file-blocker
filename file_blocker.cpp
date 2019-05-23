// file_blocker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <Windows.h>

bool use_extended_locking = false;
int  locking_time_out = 30;
bool print_help = false;
bool exit_after_help = false;

int main(int argc, char * argv[])
{
	HANDLE h_file = 0;
	BOOL lock_success = false;

	//get options 
	int checked_params = 1;
	while (argc > checked_params)
	{
		std::string param_name(argv[checked_params]);
		if (param_name.compare("-l") == 0)
		{
			use_extended_locking = true;
			checked_params += 1;
			continue;
		}
		else if (param_name.compare("-t") == 0)
		{
			if (argc > checked_params + 1)
			{
				std::string param_value(argv[checked_params + 1]);
				try
				{
					locking_time_out = std::stoi(param_value);
					checked_params += 2;
					continue;
				}
				catch (...) {}
			}
			checked_params += 1;
			continue;
		}
		else if (param_name.compare("-h") == 0)
		{
			print_help = true;
			exit_after_help = true;
			checked_params += 1;
			continue;
		}
		else
		{
			std::cout << "Unknown param: " << param_name << " \n";
			print_help = true;
			checked_params += 1;
			continue;
		}
	}
	
	if(print_help)
	{
		std::cout << "Application will run and block its exe file from change. \n\n";
		std::cout << "Options: \n";
		std::cout << "\t -l : use LockFileEx to lock file \n";
		std::cout << "\t -t N : set timeout for app in seconds \n";
		std::cout << "\t -h : print help \n";

		if (exit_after_help)
		{
			return 0;
		}
	}

	//to locking 
	std::cout << "Hello there! Gonna lock this file for " << locking_time_out << "sec \n";

	OVERLAPPED overlapvar = { 0 };
	wchar_t app_path_buffer[MAX_PATH + 1];
	if (GetModuleFileName(NULL, app_path_buffer, MAX_PATH) == 0)
	{
		std::cout << "Error: Failed get file path\n";
		return 1;
	}

	std::wstring app_path_wstring = app_path_buffer;

	if (use_extended_locking)
	{
		std::cout << "Run file: \"" << std::string(app_path_wstring.begin(), app_path_wstring.end()) << "\" (block writing and deleting )\n";
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
		else
		{
			std::cout << "Error: Failed to lock file\n";
		}
	}
	else
	{
		std::cout << "Run file: \"" << std::string(app_path_wstring.begin(), app_path_wstring.end()) << "\" (only block writing and deleting )\n";
	}

	//wait 
	int wait_steps = locking_time_out;

	while (wait_steps--)
	{
		Sleep(1000);
		std::cout << "Still blocking file: \"" << argv[0] << "\"\n";
	}

	//unlock and exit 
	if (use_extended_locking)
	{
		UnlockFileEx(h_file, 0, MAXDWORD, MAXDWORD, &overlapvar);
		CloseHandle(h_file);
	}

	return 0;
}
