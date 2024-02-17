#pragma once

#include <functional>

class Font
{
private:
	static char s_formatted[1024];

public:
	static Font* GetMainFont() noexcept;

	static void SetCursor(float x, float y) noexcept;
	static void GetCursor(float* x, float* y) noexcept;
	static void SetScale(float scaleX, float scaleY) noexcept;

	void Print(const char* fmt, ...) const noexcept;
	void PrintV(const char* fmt, va_list va) const noexcept;
	void PrintCentered(const char* fmt, ...) const noexcept;
	void PrintFormatted(const char* formatted, int backdrop = 0) const noexcept;

	float GetTextWidth(const char* text) const noexcept;
	float GetHeight() const noexcept;

	static void OnFlush(std::function<void()> callback) noexcept;
};

void FONT_Print(char* fmt, ...);