#include "JsonEditor.h"

const string JsonEditor::SETTING_FILE_DEFAULT = "setting.ini";

bool JsonEditor::LoadSetting()
{
	fstream fs;
	fs.open(SETTING_FILE_DEFAULT);

	if (fs.is_open())
	{
		directoryPath.clear();

		string str;

		while (getline(fs, str))
		{
			directoryPath.append(str);
		}

		fs.close();
	}
	else
	{
		return false;
	}

	return true;
}

bool JsonEditor::UpdateJsonList()
{
	if (filesystem::exists(directoryPath))
	{
		jsonList.clear();

		for (const auto& file : filesystem::directory_iterator(directoryPath))
		{
			string jsonfilename = file.path().generic_string();
			string filenameextension = "";


			for (int i = jsonfilename.size() - 1; i >= 0; i--)
			{
				if (jsonfilename[i] == '.')
				{
					i++;
					while (i < jsonfilename.size())
					{
						filenameextension += jsonfilename[i];
						i++;
					}
					break;
				}
			}

			if (filenameextension == "json")
			{
				jsonList.push_back(jsonfilename);
			}
		}
	}
	else
	{
		return false;
	}

	return true;
}

void JsonEditor::ShowJsonList()
{
	cout << "Directory : " << directoryPath << "\n\n";

	for (int i = 0; i < jsonList.size(); i++)
	{
		cout << i << ". " << jsonList[i] << '\n';
	}
	cout << '\n';
}

bool JsonEditor::OpenJsonFile(int filenumber)
{
	if ((filenumber < 0) || (jsonList.size() <= filenumber))
	{
		return false;
	}
	else
	{
		string filepath = jsonList[filenumber];
		fstream fs;
		fs.open(filepath);

		if (fs.is_open())
		{
			lastFileNumber = filenumber;
			lastOpenedTime = filesystem::last_write_time(filepath);

			string str;

			while (getline(fs, str))
			{
				cout << str << '\n';
			}

			fs.close();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool JsonEditor::CheckLastestFile()
{
	string filepath = jsonList[lastFileNumber];

	return lastOpenedTime == filesystem::last_write_time(filepath);
}
