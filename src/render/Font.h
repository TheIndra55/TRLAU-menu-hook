#pragma once

#include <functional>

class Font
{
private:
	static char s_formatted[1024];

public:
	static Font* GetMainFont();

	static void SetCursor(float x, float y);
	static void SetScale(float scaleX, float scaleY);

	void Print(const char* fmt, ...);
	void PrintFormatted(const char* formatted, int backdrop = 0);

	static void OnFlush(std::function<void()> callback);
};