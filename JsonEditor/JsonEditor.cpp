#include "JsonEditor.h"

const string JsonEditor::SETTING_FILE_DEFAULT = "setting.ini";

//template <typename TP>
//std::time_t to_time_t(TP tp)
//{
//	std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
//	std::chrono::milliseconds millis = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch());
//
//	// 변환된 밀리초 시간 값 출력
//	std::cout << millis.count() << std::endl;
//
//
//	using namespace std::chrono;
//	auto sctp = time_point_cast<system_clock::duration>(tp - TP::clock::now()
//		+ system_clock::now());
//	return system_clock::to_time_t(sctp);
//}

long long file_time_to_milliseconds(const filesystem::file_time_type& fileTime)
{
	using namespace std::chrono;
	// 시스템 기본 시계 타입에 대한 duration 타입
	using file_clock = filesystem::file_time_type::clock;
	// file_time_type 시간 값을 duration 타입으로 변환
	using duration_type = file_clock::duration;

	// file_time_type 시간 값을 duration 타입으로 변환
	duration_type dur = fileTime.time_since_epoch();
	// 시간 값을 밀리초로 변환
	long long millis = duration_cast<milliseconds>(dur).count();

	return millis;
}

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

		dirLastChangedTime = filesystem::last_write_time(directoryPath);

		fs.close();
	}
	else
	{
		return false;
	}

	return true;
}

JsonEditor::Result JsonEditor::UpdateJsonList()
{
	if (filesystem::exists(directoryPath))
	{
		jsonList.clear();
		lastChangedTimeList.clear();

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
				lastChangedTimeList.push_back(filesystem::last_write_time(jsonfilename));
				jsonList.push_back(jsonfilename);
			}
		}
	}
	else
	{
		return Result::DirectoryNotFound;
	}

	filesystem::file_time_type dirCurrentChangedTime = filesystem::last_write_time(directoryPath);
	
	// 디렉토리가 수정됐는지 확인
	if (dirLastChangedTime == dirCurrentChangedTime)
	{
		return Result::NotChanged;
	}
	else
	{
		dirLastChangedTime = dirCurrentChangedTime;
		return Result::Changed;
	}
}

void JsonEditor::ShowJsonList(int selectednumber)
{
	cout << "Directory : " << directoryPath << "\n\n";

	for (int i = 0; i < jsonList.size(); i++)
	{
		if (i == selectednumber)
		{
			cout << "-> ";
		}
		else
		{
			cout << "   ";
		}
		cout << i << ". " << "[" << lastChangedTimeList[i] << "] " << jsonList[i] << '\n';
	}

	cout << '\n';
}

int JsonEditor::GetJsonListSize()
{
	return jsonList.size();
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

bool JsonEditor::CheckLastestDirectory()
{
	return dirLastChangedTime == filesystem::last_write_time(directoryPath);
}

bool JsonEditor::CheckLastestFile()
{
	string filepath = jsonList[lastFileNumber];

	return lastChangedTimeList[lastFileNumber] == filesystem::last_write_time(filepath);
}
