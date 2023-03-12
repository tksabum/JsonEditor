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

	State lastState = selectFile;
	State state = selectFile;
	int lastSelectedNumber = 0;
	int selectedNumber = 0;
	int minSelected = 0;
	int maxSelected = 0;

	while (true)
	{
		bool isChanged = false;

		static bool isFirstLoop = true;
		if (isFirstLoop)
		{
			isChanged = true;
		}

		// input
		switch (state)
		{
			case selectFile:
				if (GetAsyncKeyState(VK_UP) & 0x0001)
				{
					selectedNumber = selectedNumber - 1;
				}
				if (GetAsyncKeyState(VK_DOWN) & 0x0001)
				{
					selectedNumber = selectedNumber + 1;
				}

				if (GetAsyncKeyState(VK_SPACE) & 0x0001)
				{
					state = opened;
				}

				break;

			case opened:
				if (GetAsyncKeyState(VK_SPACE) & 0x0001)
				{
					lastSelectedNumber = 0;
					selectedNumber = 0;
					state = selectFile;
				}
				
				break;

			default:
				break;
		}

		switch (state)
		{
		case selectFile:
			if (!jsonEditor.CheckLastestDirectory())
			{
				isChanged = true;
				jsonEditor.UpdateJsonList();
			}

			break;

		case opened:
			if (!jsonEditor.CheckLastestFile())
			{
				isChanged = true;
				jsonEditor.UpdateJsonList();
			}

			break;

		default:
			break;
		}

		maxSelected = max (minSelected + jsonEditor.GetJsonListSize() - 1, minSelected);

		// check state
		if (lastState != state)
		{
			isChanged = true;
		}
		lastState = state;

		// check selectedNumber
		selectedNumber = max(selectedNumber, minSelected);
		selectedNumber = min(selectedNumber, maxSelected);

		if (lastSelectedNumber != selectedNumber)
		{
			isChanged = true;
		}

		lastSelectedNumber = selectedNumber;

		// show
		if (isChanged)
		{
			system("cls");

			switch (state)
			{
			case selectFile:
				jsonEditor.ShowJsonList(selectedNumber);

				break;

			case opened:
				jsonEditor.OpenJsonFile(selectedNumber);

				break;

			default:
				break;
			}
		}

		isFirstLoop = false;
	}
	

	return 0;
}