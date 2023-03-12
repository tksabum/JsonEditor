#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>
#include <chrono>

#include <iostream>

using namespace std;

class JsonEditor
{
public:
	enum class Result
	{
		Changed,
		NotChanged,
		Fail,
		DirectoryNotFound
	};

	bool LoadSetting();

	Result UpdateJsonList();
	void ShowJsonList(int selectednumber);
	int GetJsonListSize();
	
	bool OpenJsonFile(int filenumber);
	bool CheckLastestDirectory();
	bool CheckLastestFile();


private:
	string directoryPath;

	vector<string> jsonList;
	vector<filesystem::file_time_type> lastChangedTimeList;

	filesystem::file_time_type dirLastChangedTime;
	int lastFileNumber;

	const static string SETTING_FILE_DEFAULT;
};

