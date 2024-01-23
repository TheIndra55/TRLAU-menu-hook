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
	return *(Font**)0x7D1800;
}

void Font::SetCursor(float x, float y)
{
	Hooking::Call(0x433C70, x, y);
}

void Font::GetCursor(float* x, float* y)
{
	Hooking::Call(0x433C90, x, y);
}

void Font::SetScale(float scaleX, float scaleY)
{
	Hooking::Call(0x433E60, scaleX, scaleY);
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
	Hooking::ThisCall(0x434A70, this, formatted, backdrop);
}

float Font::GetTextWidth(const char* text)
{
	return Hooking::ThisCallReturn<float>(0x434510, this, text);
}

void Font::OnFlush(std::function<void()> callback)
{
	if (!s_callback)
	{
		MH_CreateHook((void*)0x434C40, Flush, (void**)&s_Flush);
		MH_EnableHook(MH_ALL_HOOKS);
	}

	s_callback = callback;
}