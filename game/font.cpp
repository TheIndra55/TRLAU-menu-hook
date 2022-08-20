#include <stdio.h>
#include <stdarg.h>
#include "font.hpp"

// required for ADDR macro
#include "../Game.hpp"

// orginal function pointers

void(__thiscall* Font__PrintFormatted)(void* font, const char* formatted, bool backdrop);
void(__cdecl* Font__SetAlpha)(char alpha);
void(__cdecl* Font__SetScale)(float scaleX, float scaleY);
void(__cdecl* Font__SetRGBA)(unsigned int rgba);
float(__thiscall* Font__GetTextWidth)(void* font, const char* text);
float(__thiscall* Font__GetHeight)(void* font);
void(__cdecl* Font__SetCursor)(float x, float y);

// not related to the real Font::Init but for us to define our functions pointers
void FONT_Init()
{
#if TRAE
	Font__PrintFormatted = reinterpret_cast<void(__thiscall*)(void*, const char*, bool)>(0x434A70);
	Font__SetAlpha = reinterpret_cast<void(__cdecl*)(char)>(0x433F30);
	Font__SetScale = reinterpret_cast<void(__cdecl*)(float, float)>(0x433E60);
	Font__SetRGBA = reinterpret_cast<void(__cdecl*)(unsigned int)>(0x433F20);
	Font__GetTextWidth = reinterpret_cast<float(__thiscall*)(void*, const char*)>(0x434510);
	Font__GetHeight = reinterpret_cast<float(__thiscall*)(void*)>(0x434440);
	Font__SetCursor = reinterpret_cast<void(__cdecl*)(float, float)>(0x433C70);
#elif TR7
	Font__PrintFormatted = reinterpret_cast<void(__thiscall*)(void*, const char*, bool)>(ADDR(0x434E80, 0x4323D0));
	Font__SetAlpha = reinterpret_cast<void(__cdecl*)(char)>(ADDR(0x4343E0, 0x431930));
	Font__SetScale = reinterpret_cast<void(__cdecl*)(float, float)>(ADDR(0x4342E0, 0x431860));
	Font__SetRGBA = reinterpret_cast<void(__cdecl*)(unsigned int)>(ADDR(0x4343D0, 0x431920));
	Font__GetTextWidth = reinterpret_cast<float(__thiscall*)(void*, const char*)>(ADDR(0x434950, 0x431EA0));
	Font__GetHeight = reinterpret_cast<float(__thiscall*)(void*)>(ADDR(0x4348D0, 0x431E20));
	Font__SetCursor = reinterpret_cast<void(__cdecl*)(float, float)>(ADDR(0x4340F0, 0x431670));
#endif
}

// font function wrappers around orginal ones, for both readability and sake of not having to pass font instance

void FONT_Print(const char* fmt, ...)
{
	va_list va;
	char formatted[1024];

	va_start(va, fmt);
	vsprintf_s(formatted, fmt, va);
	va_end(va);

	Font__PrintFormatted(gMainFont, formatted, false);
}

void FONT_PrintFormatted(const char* formatted, bool backdrop)
{
	Font__PrintFormatted(gMainFont, formatted, backdrop);
}

void FONT_SetAlpha(char alpha)
{
	Font__SetAlpha(alpha);
}

void FONT_SetScale(float scaleX, float scaleY)
{
	Font__SetScale(scaleX, scaleY);
}

void FONT_SetRGBA(unsigned int rgba)
{
	Font__SetRGBA(rgba);
}

float FONT_GetTextWidth(const char* text)
{
	return Font__GetTextWidth(gMainFont, text);
}

float FONT_GetHeight()
{
	return Font__GetHeight(gMainFont);
}

void FONT_SetCursor(float x, float y)
{
	return Font__SetCursor(x, y);
}