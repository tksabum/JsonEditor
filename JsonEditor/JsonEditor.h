#pragma once

#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

#include <iostream>

using namespace std;

class JsonEditor
{
public:
	bool LoadSetting();

	bool UpdateJsonList();
	void ShowJsonList();
	
	bool OpenJsonFile(int filenumber);
	bool CheckLastestFile();


private:
	string directoryPath;
	vector<string> jsonList;

	filesystem::file_time_type lastOpenedTime;
	int lastFileNumber;

	const static string SETTING_FILE_DEFAULT;
};

