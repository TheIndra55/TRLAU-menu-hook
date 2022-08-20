#pragma once

void  FONT_Init();
void  FONT_Print(const char* fmt, ...);
void  FONT_PrintFormatted(const char* formatted, bool backdrop = false);
void  FONT_SetAlpha(char alpha);
void  FONT_SetScale(float scaleX, float scaleY);
void  FONT_SetRGBA(unsigned int rgba);
float FONT_GetTextWidth(const char* text);
float FONT_GetHeight();
void  FONT_SetCursor(float x, float y);

#if TRAE
	#define gMainFont *(void**)0x7D1800
#elif TR7
	#define gMainFont *(void**)ADDR(0x1088A2C, 0x107F680)
#elif TR8
	#define gMainFont *(void**)0x9DE6D4
#endif
