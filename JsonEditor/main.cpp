#include <iostream>
#include <Windows.h>

#include "JsonEditor.h"

using namespace std;

enum State
{
	selectFile,
	opened
};

int main()
{
	JsonEditor jsonEditor;
	jsonEditor.LoadSetting();
	jsonEditor.UpdateJsonList();
	//jsonEditor.OpenJsonFile(0);

	State state = selectFile;
	int selectedNumber;

	while (true)
	{
		switch (state)
		{
			case selectFile:
				system("cls");

				jsonEditor.UpdateJsonList();
				jsonEditor.ShowJsonList();
				cout << "���ϴ� ������ ��ȣ�� �Է��ϼ���: ";
				cin >> selectedNumber;

				system("cls");
				if (jsonEditor.OpenJsonFile(selectedNumber))
				{
					cout << '\n' << "ESC�� ������ �����մϴ�." << '\n';
					state = opened;
				}

				break;

			case opened:
				if (GetAsyncKeyState(VK_ESCAPE) & 0x8000)
				{
					state = selectFile;
				}

				if (!jsonEditor.CheckLastestFile())
				{
					system("cls");
					if (jsonEditor.OpenJsonFile(selectedNumber))
					{
						cout << '\n' << "ESC�� ������ �����մϴ�." << '\n';
						state = opened;
					}
					else
					{
						state = selectFile;
					}
				}
				
				break;

			default:
				break;
		}
	}
	

	return 0;
}