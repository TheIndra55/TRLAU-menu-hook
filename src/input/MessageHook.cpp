#include "MessageHook.h"

#include "cdc/render/PCDeviceManager.h"

static std::function<void(HWND, UINT, WPARAM, LPARAM)> s_callback;
static WNDPROC s_original;

static LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	s_callback(hWnd, msg, wParam, lParam);

	return CallWindowProc(s_original, hWnd, msg, wParam, lParam);
}

void MessageHook::OnMessage(std::function<void(HWND, UINT, WPARAM, LPARAM)> callback)
{
	// Get the window handle
	auto hWnd = cdc::PCDeviceManager::s_pInstance->GetWindow();

	// Set the new message handler to our handler
	s_original = (WNDPROC)SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

	s_callback = callback;
}