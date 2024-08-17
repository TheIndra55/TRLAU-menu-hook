#include <Windows.h>
#include <Commctrl.h>

#include "ErrorHandler.h"
#include "cdc/render/PCDeviceManager.h"

static void HideWindow()
{
	auto instance = cdc::PCDeviceManager::GetInstance();

	if (instance)
	{
		ShowWindow(instance->GetWindow(), SW_MINIMIZE);
	}
}

bool MainG2_ErrorHandler(const char* message)
{
	// Hide the game window
	HideWindow();

	// Convert to wide string
	wchar_t content[1024];
	mbstowcs(content, message, 1024);

	TASKDIALOGCONFIG config = { };
	config.cbSize = sizeof(TASKDIALOGCONFIG);
	config.hInstance = GetModuleHandleA(NULL);
	config.dwFlags = TDF_EXPAND_FOOTER_AREA | TDF_EXPANDED_BY_DEFAULT;
	config.dwCommonButtons = TDCBF_CLOSE_BUTTON;
	config.pszMainIcon = TD_ERROR_ICON;
	config.pszWindowTitle = L"Tomb Raider Error";
	config.pszMainInstruction = L"Tomb Raider has encountered an error";
	config.pszContent = L"A fatal error has occured while playing Tomb Raider. To resolve this error refer to the details below.";
	config.pszExpandedInformation = content;

	// Show the error dialog and exit the game after
	TaskDialogIndirect(&config, NULL, NULL, NULL);
	exit(1);

	return true;
}