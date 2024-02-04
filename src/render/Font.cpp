#include <stdio.h>
#include <stdarg.h>

#include <MinHook.h>

#include "Font.h"
#include "util/Hooking.h"

static std::function<void()> s_callback;

// Font::Flush
static void(* s_Flush)();

static void Flush()
{
	s_callback();
	s_Flush();
}

char Font::s_formatted[1024];

Font* Font::GetMainFont()
{
	return *(Font**)GET_ADDRESS(0x107F680, 0x7D1800, 0x9DE6D4);
}

void Font::SetCursor(float x, float y)
{
	auto addr = GET_ADDRESS(0x431670, 0x433C70, 0x474C90);

	Hooking::Call(addr, x, y);
}

void Font::GetCursor(float* x, float* y)
{
	auto addr = GET_ADDRESS(0x431690, 0x433C90, 0x474CB0);

	Hooking::Call(addr, x, y);
}

void Font::SetScale(float scaleX, float scaleY)
{
	auto addr = GET_ADDRESS(0x431860, 0x433E60, 0x000000);

	Hooking::Call(addr, scaleX, scaleY);
}

void Font::Print(const char* fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	vsprintf_s(s_formatted, fmt, va);
	va_end(va);

	PrintFormatted(s_formatted);
}

void Font::PrintCentered(const char* fmt, ...)
{
	va_list va;

	va_start(va, fmt);
	vsprintf_s(s_formatted, fmt, va);
	va_end(va);

	float x, y;
	GetCursor(&x, &y);

	auto width = GetTextWidth(s_formatted);
	SetCursor(x - width / 2, y);

	PrintFormatted(s_formatted);
}

void Font::PrintFormatted(const char* formatted, int backdrop)
{
	auto addr = GET_ADDRESS(0x4323D0, 0x434A70, 0x476BC0);

	Hooking::ThisCall(addr, this, formatted, backdrop);
}

float Font::GetTextWidth(const char* text)
{
	auto addr = GET_ADDRESS(0x431EA0, 0x434510, 0x000000);

	return Hooking::ThisCallReturn<float>(addr, this, text);
}

float Font::GetHeight()
{
	auto addr = GET_ADDRESS(0x431E20, 0x434440, 0x000000);

	return Hooking::ThisCallReturn<float>(addr, this);
}

void Font::OnFlush(std::function<void()> callback)
{
	if (!s_callback)
	{
		// TODO pattern
		auto addr = GET_ADDRESS(0x432570, 0x434C40, 0x476D80);

		MH_CreateHook((void*)addr, Flush, (void**)&s_Flush);
		MH_EnableHook(MH_ALL_HOOKS);
	}

	s_callback = callback;
}